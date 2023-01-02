/*!
  \file kernel_baker.cpp
  \author Sho Ikeda
  \brief No brief description

  \details
  No detailed description.

  \copyright
  Copyright (c) 2015-2023 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
// Zisc
#include "zisc/binary_serializer.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/internal/zstd.hpp"

namespace {

/*!
  \brief No brief description

  No detailed description.
  */
struct Parameters
{
  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::string_view kernel_set_name_;
  std::int32_t compression_level_ = 0; // 19: default compression level
  zisc::Padding<4> pad_;
};

//! Output the given debug message
template <typename Type>
void printDebugMessageImpl(Type&& message) noexcept
{
#if defined(Z_DEBUG_MODE)
  std::cout << message;
#else // Z_DEBUG_MODE
  static_cast<void>(message);
#endif // Z_DEBUG_MODE
}

//! Output the given debug message
template <typename Type, typename ...Types>
void printDebugMessage(Type&& message, Types&&... rest) noexcept
{
  printDebugMessageImpl(std::forward<Type>(message));
  if constexpr (0 < sizeof...(Types))
    printDebugMessage(std::forward<Types>(rest)...);
  else
    std::cout << std::endl;
}

//! Load the given SPIR-V file
zisc::pmr::vector<std::byte> loadSpirVFile(std::string_view file_path,
                                           Parameters& params) noexcept
{
  std::ifstream spv_file{file_path.data(), std::ios_base::binary};
  if (!spv_file.is_open()) {
    std::cerr << "[Error] SPIR-V file '" << file_path << "' not found."
              << std::endl;
    std::abort();
  }
  const std::streamsize spv_size = zisc::BSerializer::getDistance(&spv_file);
  if (spv_size % 4 != 0) {
    std::cerr << "[Error] The size of the SPIR-V file '" << file_path
              << "' is invalid." << std::endl;
    std::abort();
  }

  const zisc::pmr::vector<std::byte>::allocator_type alloc{params.mem_resource_};
  zisc::pmr::vector<std::byte> spv_data{alloc};
  spv_data.resize(static_cast<std::size_t>(spv_size));
  spv_file.read(reinterpret_cast<char*>(spv_data.data()), spv_size);
  return spv_data;
}

//! Allocate a memory
void* allocateMemory(void* opaque, size_t size)
{
  auto* mem_resource = static_cast<zisc::pmr::memory_resource*>(opaque);
  void* address = mem_resource->allocate(size);
  return address;
}

//! Deallocate a memory
void deallocateMemory(void* opaque, void* address)
{
  auto* mem_resource = static_cast<zisc::pmr::memory_resource*>(opaque);
  const std::size_t size = 0; //! \todo Retrieve the actual size of the memory
  mem_resource->deallocate(address, size);
}

zisc::pmr::vector<std::byte> encodeSpirVData(
    const zisc::pmr::vector<std::byte>& data,
    Parameters& params) noexcept
{
  const zisc::pmr::vector<std::byte>::allocator_type alloc{params.mem_resource_};
  zisc::pmr::vector<std::byte> encoded_data{alloc};
  encoded_data.resize(ZSTD_compressBound(data.size()));

  const ZSTD_customMem mem{allocateMemory, deallocateMemory, params.mem_resource_};
  ZSTD_CCtx* context = ZSTD_createCCtx_advanced(mem);
  if (context == nullptr) {
    std::cerr << "ZSTD context creation failed." << std::endl;
    std::abort();
  }

  const std::size_t encode_size = ZSTD_compressCCtx(context,
                                                    encoded_data.data(),
                                                    encoded_data.size(),
                                                    data.data(),
                                                    data.size(),
                                                    params.compression_level_);
  {
    const auto size = static_cast<std::ptrdiff_t>(encode_size);
    encoded_data.erase(encoded_data.begin() + size, encoded_data.end());
  }

  const std::size_t result = ZSTD_freeCCtx(context);
  if (ZSTD_isError(result) != 0) {
    std::cerr << "[ERROR] " << ZSTD_getErrorName(result) << std::endl;
    std::abort();
  }

  return encoded_data;
}

void writeEncodedData(const zisc::pmr::vector<std::byte>& encoded_data,
                      std::ostream* bake_code) noexcept
{
  constexpr std::size_t bytes_per_line = 8;
  const char* indent = "        ";
  std::ostream& code = *bake_code;
  for (std::size_t index = 0; index < encoded_data.size(); ++index) {
    if ((index % bytes_per_line == 0))
      code << indent;
    code << "b(0x" << std::hex << std::setfill('0') << std::setw(2)
              << std::to_integer<int>(encoded_data[index])
              << ")";
    if (index != (encoded_data.size() - 1))
      code << ",";
    if ((index % bytes_per_line) == (bytes_per_line - 1))
      code << std::endl;
  }
  code << std::dec << std::setw(0) << std::endl;
}

void generateBakeCode(const zisc::pmr::vector<std::byte>& spv_data,
                      const zisc::pmr::vector<std::byte>& encoded_data,
                      Parameters& params,
                      std::ostream* bake_code)
{
  auto add_line = [bake_code](const std::string_view line) noexcept
  {
    (*bake_code) << line << std::endl;
  };

  auto set_include_guard = [&params](const std::string_view prefix,
                                     zisc::pmr::string* str) noexcept
  {
    (*str) = prefix;
    (*str) += "ZIVC_KERNEL_SET_";
    (*str) += params.kernel_set_name_.data();
    (*str) += "_BAKE_HPP";
  };

  add_line("/*!");
  add_line("  \\brief Baked Kernel set code.");
  add_line("");
  add_line("  \\details");
  add_line("  No detailed description.");
  add_line("  */");
  add_line("");
  const zisc::pmr::string::allocator_type alloc{params.mem_resource_};
  zisc::pmr::string include_guard_name{alloc};
  set_include_guard("#ifndef ", &include_guard_name);
  add_line(include_guard_name);
  set_include_guard("#define ", &include_guard_name);
  add_line(include_guard_name);
  add_line("");
  add_line("// Standard C++ library");
  add_line("// #include <array>");
  add_line("// #include <cstddef>");
  add_line("");
  add_line("/*!");
  add_line("  \\brief Baked kernel set info.");
  add_line("");
  add_line("  No detailed description.");
  add_line("  */");
  zisc::pmr::string tmp{alloc};
  tmp = "class BakedKernelSet_";
  tmp += params.kernel_set_name_.data();
  add_line(tmp);
  add_line("{");
  add_line(" public:");
  add_line("  static constexpr std::size_t kRawSize = " + std::to_string(spv_data.size()) + ";");
  add_line("  static constexpr std::size_t kSize = " + std::to_string(encoded_data.size()) + ";");
  add_line("");
  add_line("  //! Return the encoded SPIR-V code");
  add_line("  static constexpr std::array<std::byte, kSize> encodedData() noexcept");
  add_line("  {");
  add_line("    const std::array<std::byte, kSize> data = encodedDataImpl();");
  add_line("    return data;");
  add_line("  }");
  add_line("");
  add_line("  //! Return the data size of raw SPIR-V code in bytes");
  add_line("  static constexpr std::size_t rawSize() noexcept");
  add_line("  {");
  add_line("    return kRawSize;");
  add_line("  }");
  add_line("");
  add_line("  //! Return the data size of encoded SPIR-V code in bytes");
  add_line("  static constexpr std::size_t size() noexcept");
  add_line("  {");
  add_line("    return kSize;");
  add_line("  }");
  add_line("");
  add_line(" private:");
  add_line("  //! Convert an integer value to a byte");
  add_line("  static constexpr std::byte b(const int value) noexcept");
  add_line("  {");
  add_line("    return std::byte{static_cast<unsigned char>(value)};");
  add_line("  }");
  add_line("");
  add_line("  //! Return the encoded SPIR-V code");
  add_line("  static constexpr std::array<std::byte, kSize> encodedDataImpl() noexcept");
  add_line("  {");
  add_line("    const std::array<std::byte, kSize> data{{");
  writeEncodedData(encoded_data, bake_code);
  add_line("    }};");
  add_line("    return data;");
  add_line("  }");
  add_line("};");

  add_line("");
  set_include_guard("#endif // ", &include_guard_name);
  add_line(include_guard_name);
}

//! Save the bake code
void saveBakeCode(std::string_view file_path, std::istream& bake_code) noexcept
{
  std::ofstream baked_file{file_path.data(), std::ios_base::binary};
  if (!baked_file.is_open()) {
    std::cerr << "[Error] Baked SPIR-V file'" << file_path << "' creation failed."
              << std::endl;
    std::abort();
  }
  baked_file << bake_code.rdbuf();
}

double toMegaBytes(const std::size_t bytes) noexcept
{
  auto size = static_cast<double>(bytes);
  constexpr double k = 1024.0;
  size = size / (k * k);
  return size;
}

} // namespace

int main([[maybe_unused]] int argc, char** argv)
{
  ::printDebugMessage("Bake a kernel set");
  // Input parameters
  const std::string_view exec_path{argv[0]};
  const std::string_view spv_file_path{argv[1]};
  const std::string_view baked_spv_file_path{argv[2]};
  ::printDebugMessage("    Input parameters info");
  ::printDebugMessage("        Src SPIR-V file path: ", spv_file_path.data());
  ::printDebugMessage("        Dst baked file path: ", baked_spv_file_path.data());
  ::Parameters params;
  zisc::AllocFreeResource mem_resource;
  params.mem_resource_ = std::addressof(mem_resource);
  params.kernel_set_name_ = argv[3];
  ::printDebugMessage("        Kernel set name: '", params.kernel_set_name_.data(), "'");
  params.compression_level_ = std::atoi(argv[4]);
  const int min_level = 1; // ZSTD_minCLevel();
  const int max_level = ZSTD_maxCLevel();
  params.compression_level_ = std::clamp(params.compression_level_, min_level, max_level);
  ::printDebugMessage("        Zstd compression level: ", params.compression_level_,
                      " (min=", min_level, ", max=", max_level, ")");

  ::printDebugMessage("    Encode SPIR-V");
  const zisc::pmr::vector<std::byte> spv_data = ::loadSpirVFile(spv_file_path, params);
  ::printDebugMessage("        SPIR-V data size : ",
                      ::toMegaBytes(spv_data.size()),
                      " MB.");

  // Compress the SPIR-V data
  const zisc::pmr::vector<std::byte> encoded_data = ::encodeSpirVData(spv_data, params);
  ::printDebugMessage("        Encoded data size: ",
                      ::toMegaBytes(encoded_data.size()),
                      " MB.");

  // Generate a hpp file of the bake code
  {
    const zisc::pmr::string::allocator_type alloc{params.mem_resource_};
    const zisc::pmr::string tmp{alloc};
    zisc::pmr::stringstream bake_code{tmp};
    ::generateBakeCode(spv_data, encoded_data, params, std::addressof(bake_code));
    zisc::BSerializer::backToBegin(std::addressof(bake_code));
    ::saveBakeCode(baked_spv_file_path, bake_code);
  }

  // Memory usage
  ::printDebugMessage("    Memory usage");
  ::printDebugMessage("        Peak usage: ",
                      ::toMegaBytes(mem_resource.peakMemoryUsage()),
                      " MB.");

  return 0;
}
