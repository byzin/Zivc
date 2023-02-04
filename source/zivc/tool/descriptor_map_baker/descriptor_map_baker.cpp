/*!
  \file descriptor_map_baker.cpp
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
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "zisc/binary_serializer.hpp"
#include "zisc/hash/fnv_1a_hash_engine.hpp"
#include "zisc/memory/alloc_free_resource.hpp"
#include "zisc/memory/std_memory_resource.hpp"
// Zivc
#include "zivc/internal/shader_desc_map.hpp"

namespace {

using ShaderDescMap = zivc::internal::ShaderDescMap;

/*!
  \brief No brief description

  No detailed description.
  */
struct Parameters
{
  zisc::pmr::memory_resource* mem_resource_ = nullptr;
  std::string_view kernel_set_name_;
};

//! Output the given debug message
template <typename Type>
void printDebugMessageImpl([[maybe_unused]] Type&& message) noexcept
{
#if defined(Z_DEBUG_MODE)
  std::cout << message;
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

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] expected No description.
  \param [out] label No description.
  */
void parseLabel(std::istream& line,
                const std::string_view expected,
                zisc::pmr::string* label) noexcept
{
  const auto result = static_cast<bool>(std::getline(line, *label, ','));
  if (!result || (*label != expected)) {
    std::cerr << "[Error] '" << expected << "' label not found in the line."
              << std::endl;
    std::abort();
  }
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] label No description.
  \param [out] value No description.
  */
void parseValue(std::istream& line,
                const std::string_view label,
                zisc::pmr::string* value) noexcept
{
  const auto result = static_cast<bool>(std::getline(line, *value, ','));
  if (!result) {
    std::cerr << "[Error] '" << label << "' value not found in the line."
              << std::endl;
    std::abort();
  }
}

/*!
  \details No detailed description

  \param [in] str No description.
  \return No description
  */
bool isInteger(const std::string_view str) noexcept
{
  bool result = true;
  for (const char c : str)
    result = result && (std::isdigit(c) != 0);
  return result;
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [out] value No description.
  */
void checkLineEof(std::istream& line, zisc::pmr::string* value)
{
  if (!line.eof()) {
    line >> *value;
    std::cerr << "[Error] The line still have some value: '" << *value << "'"
              << std::endl;
    std::abort();
  }
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] desc_map No description.
  \return No description
  */
void parsePushConstantMap(std::istream& line, Parameters& params, ShaderDescMap* desc_map) noexcept
{
  using PushConstantType = ShaderDescMap::PushConstantType;
  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};
  // Parse name
  PushConstantType type = PushConstantType::kMax;
  parseLabel(line, "name", &s);
  parseValue(line, "name", &s);
  type = (s == "global_offset")       ? PushConstantType::kGlobalOffset :
         (s == "enqueued_local_size") ? PushConstantType::kEnqueuedLocalSize :
         (s == "global_size")         ? PushConstantType::kGlobalSize :
         (s == "region_offset")       ? PushConstantType::kRegionOffset :
         (s == "num_workgroups")      ? PushConstantType::kNumWorkgroups :
         (s == "region_group_offset") ? PushConstantType::kRegionGroupOffset
                                      : PushConstantType::kMax;
  if (type == PushConstantType::kMax) {
    std::cerr << "[Error] 'name' has unsupported push constant type: '" << s << "'"
              << std::endl;
    std::abort();
  }

  // Parse offset
  zivc::uint32b offset = 0;
  parseLabel(line, "offset", &s);
  parseValue(line, "offset", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'offset' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  offset = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Parse size
  zivc::uint32b size = 0;
  parseLabel(line, "size", &s);
  parseValue(line, "size", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'size' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  size = static_cast<zivc::uint32b>(std::atoi(s.data()));

  checkLineEof(line, &s);

  // Create a push constant map
  const ShaderDescMap::PushConstantMap map = {offset, size};
  desc_map->setPushConstantMap(type, map);
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] desc_map No description.
  \return No description
  */
void parseKernelDeclaration(std::istream& line, Parameters& params, ShaderDescMap* desc_map) noexcept
{
  using HashEngineT = ShaderDescMap::HashEngineT;
  using HashT = ShaderDescMap::HashT;

  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};
  // Parse name
  parseValue(line, "name", &s);
  const HashT id = HashEngineT::hash(s.data());

  checkLineEof(line, &s);

  // Create a push constant map
  ShaderDescMap::KernelDescMap map{params.mem_resource_};
  desc_map->setKernelDescMap(id, std::move(map));
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] kernels No description.
  */
void parseKernelDescMapBuffer(std::istream& line, Parameters& params, ShaderDescMap::KernelDescMap* kernel) noexcept
{
  using HashEngineT = ShaderDescMap::HashEngineT;
  using HashT = ShaderDescMap::HashT;
  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};

  // Parse descriptor set index
  [[maybe_unused]] zivc::uint32b descriptor_set_id = 0;
  parseValue(line, "descriptorSet", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'descriptorSet' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  descriptor_set_id = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Parse binding
  zivc::uint32b binding = 0;
  parseLabel(line, "binding", &s);
  parseValue(line, "binding", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'binding' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  binding = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Parse offset
  [[maybe_unused]] zivc::uint32b offset = 0;
  parseLabel(line, "offset", &s);
  parseValue(line, "offset", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'offset' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  offset = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Parse argKind
  [[maybe_unused]] zivc::uint32b pod_size = 0;
  parseLabel(line, "argKind", &s);
  parseValue(line, "argKind", &s);
  constexpr HashT global_t = HashEngineT::hash("buffer");
  constexpr HashT pod_t = HashEngineT::hash("pod_ubo");
  const HashT kind_t = HashEngineT::hash(s.data());
  switch (kind_t) {
   case global_t: {
    break;
   }
   case pod_t: {
    parseLabel(line, "argSize", &s);
    parseValue(line, "argSize", &s);
    if (!isInteger(s)) {
      std::cerr << "[Error] 'argSize' has non integer value: '" << s << "'"
                << std::endl;
      std::abort();
    }
    pod_size = static_cast<zivc::uint32b>(std::atoi(s.data()));
    break;
   }
   default: {
    std::cerr << "[Error] Unsupported buffer type '" << s << "'." << std::endl;
    std::abort();
   }
  }

  checkLineEof(line, &s);

  // Create a buffer map
  using BufferMap = ShaderDescMap::KernelDescMap::BufferMap;
  bool has_binding = false;
  for (const BufferMap& map : kernel->buffer_map_list_)
    has_binding = has_binding || (map.binding_ == binding);
  if (!has_binding) {
    const BufferMap map = {binding};
    kernel->buffer_map_list_.emplace_back(map);
  }
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] kernels No description.
  */
void parseKernelDescMapLocal(std::istream& line, Parameters& params, ShaderDescMap::KernelDescMap* kernel) noexcept
{
  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};

  // Parse argKind
  parseValue(line, "argKind", &s);
  if (s != "local") {
    std::cerr << "[Error] Unsupported buffer type '" << s << "'." << std::endl;
    std::abort();
  }

  // Parse element size
  [[maybe_unused]] zivc::uint32b array_elem_size = 0;
  parseLabel(line, "arrayElemSize", &s);
  parseValue(line, "arrayElemSize", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'arrayElemSize' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  array_elem_size = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Parse spec id 
  [[maybe_unused]] zivc::uint32b spec_id = 0;
  parseLabel(line, "arrayNumElemSpecId", &s);
  parseValue(line, "arrayNumElemSpecId", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'arrayNumElemSpecId' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  spec_id = static_cast<zivc::uint32b>(std::atoi(s.data()));

  checkLineEof(line, &s);

  // Create a buffer map
  using LocalMap = ShaderDescMap::KernelDescMap::LocalMap;
  bool has_spec_id = false;
  for (const LocalMap& map : kernel->local_map_list_)
    has_spec_id = has_spec_id || (map.spec_id_ == spec_id);
  if (!has_spec_id) {
    const LocalMap map = {spec_id};
    kernel->local_map_list_.emplace_back(map);
  }
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] desc_map No description.
  \return No description
  */
void parseKernelDescMap(std::istream& line, Parameters& params, ShaderDescMap* desc_map) noexcept
{
  using HashEngineT = ShaderDescMap::HashEngineT;
  using HashT = ShaderDescMap::HashT;

  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};
  // Parse name
  parseValue(line, "name", &s);
  const HashT id = HashEngineT::hash(s.data());
  if (!desc_map->hasKernelDescMap(id)) {
    std::cerr << "[Error] The declaration of kernel '" << s << "' not found."
              << std::endl;
    std::abort();
  }
  ShaderDescMap::KernelDescMap& kernel = desc_map->kernelDescMap(id);

  // Parse arg name
  parseLabel(line, "arg", &s);
  parseValue(line, "arg", &s);

  // Parse arg position
  [[maybe_unused]] zivc::uint32b arg_ordinal = 0;
  parseLabel(line, "argOrdinal", &s);
  parseValue(line, "argOrdinal", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'argOrdinal' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  arg_ordinal = static_cast<zivc::uint32b>(std::atoi(s.data()));

  // Check if the arg is buffer or local
  parseValue(line, "arg", &s);
  constexpr HashT buffer_t = HashEngineT::hash("descriptorSet");
  constexpr HashT local_t = HashEngineT::hash("argKind");
  const HashT input_t = HashEngineT::hash(s.data());
  switch (input_t) {
   case buffer_t: {
    parseKernelDescMapBuffer(line, params, &kernel);
    break;
   }
   case local_t: {
    parseKernelDescMapLocal(line, params, &kernel);
    break;
   }
   default: {
    std::cerr << "[Error] Unsupported kernel arg type '" << s << "'." << std::endl;
    std::abort();
   }
  }

  checkLineEof(line, &s);
}

/*!
  \details No detailed description

  \param [in] line No description.
  \param [in] params No description.
  \param [out] desc_map No description.
  \return No description
  */
void parseSpecConstantMap(std::istream& line, Parameters& params, ShaderDescMap* desc_map) noexcept
{
  using SpecConstantType = ShaderDescMap::SpecConstantType;
  zisc::pmr::string s{zisc::pmr::string::allocator_type{params.mem_resource_}};
  // Parse name
  SpecConstantType type = SpecConstantType::kMax;
  parseValue(line, "name", &s);
  type = (s == "workgroup_size_x") ? SpecConstantType::kWorkgroupSizeX :
         (s == "workgroup_size_y") ? SpecConstantType::kWorkgroupSizeY :
         (s == "workgroup_size_z") ? SpecConstantType::kWorkgroupSizeZ :
         (s == "work_dim")         ? SpecConstantType::kWorkDim
                                   : SpecConstantType::kMax;
  if (type == SpecConstantType::kMax) {
    std::cerr << "[Error] 'name' has unsupported spec constant type: '" << s << "'"
              << std::endl;
    std::abort();
  }

  // Parse ID 
  zivc::uint32b id = 0;
  parseLabel(line, "spec_id", &s);
  parseValue(line, "spec_id", &s);
  if (!isInteger(s)) {
    std::cerr << "[Error] 'spec_id' has non integer value: '" << s << "'"
              << std::endl;
    std::abort();
  }
  id = static_cast<zivc::uint32b>(std::atoi(s.data()));

  checkLineEof(line, &s);

  // Create a push constant map
  const ShaderDescMap::SpecConstantMap map = {id};
  desc_map->setSpecConstantMap(type, map);
}

/*!
  \details No detailed description

  \param [in] map_file No description.
  \param [in] params No description.
  */
ShaderDescMap parse(std::istream& map_file, Parameters& params) noexcept
{
  ShaderDescMap desc_map{params.mem_resource_};
  desc_map.initialize();
  for (zisc::pmr::string l{params.mem_resource_}; std::getline(map_file, l);) {
    zisc::pmr::istringstream line{l};

    // Parse type
    zisc::pmr::string type{zisc::pmr::string::allocator_type{params.mem_resource_}};
    if (!std::getline(line, type, ',')) {
      std::cerr << "[Error] Empty line found in the map file." << std::endl;
      std::abort();
    }
    // Supported types
    using HashEngineT = ShaderDescMap::HashEngineT;
    using HashT = ShaderDescMap::HashT;
    constexpr HashT push_constant_t = HashEngineT::hash("pushconstant");
    constexpr HashT kernel_decl_t = HashEngineT::hash("kernel_decl");
    constexpr HashT kernel_t = HashEngineT::hash("kernel");
    constexpr HashT spec_constant_t = HashEngineT::hash("spec_constant");
    //
    const HashEngineT::ValueT input_t = HashEngineT::hash(type.data());
    switch (input_t) {
     case push_constant_t: {
      parsePushConstantMap(line, params, &desc_map);
      break;
     }
     case kernel_decl_t: {
      parseKernelDeclaration(line, params, &desc_map);
      break;
     }
     case kernel_t: {
      parseKernelDescMap(line, params, &desc_map);
      break;
     }
     case spec_constant_t: {
      parseSpecConstantMap(line, params, &desc_map);
      break;
     }
     default: {
      std::cerr << "[Error] Unsupported type '" << type << "'." << std::endl;
      std::abort();
     }
    }
  }
  return desc_map;
}

//! Load the given map file
std::ifstream loadMapFile(std::string_view file_path) noexcept
{
  std::ifstream map_file{file_path.data()};
  if (!map_file.is_open()) {
    std::cerr << "[Error] Descriptor map '" << file_path << "' not found."
              << std::endl;
    std::abort();
  }

  return map_file;
}

void generateBakeCode(const ShaderDescMap& desc_map,
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
    (*str) += "ZIVC_DESC_MAP_";
    (*str) += params.kernel_set_name_.data();
    (*str) += "_HPP";
  };

  add_line("/*!");
  add_line("  \\brief Baked descriptor map.");
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
  add_line("  \\brief Baked descriptor map.");
  add_line("");
  add_line("  No detailed description.");
  add_line("  */");
  zisc::pmr::string tmp{alloc};
  tmp = "class BakedDescMap_";
  tmp += params.kernel_set_name_.data();
  add_line(tmp);
  add_line("{");
  add_line(" public:");
  add_line("");
  add_line("  //! Set the given shader descriptor map for the kernel set");
  add_line("  static void setDescMap(ShaderDescMap* desc_map)");
  add_line("  {");

  tmp.clear();
  constexpr std::size_t n = 256;
  tmp.resize(n);

  // push constant map
  using PushConstantType = ShaderDescMap::PushConstantType;
  add_line("    using PushConstantType = ShaderDescMap::PushConstantType;");
  for (std::size_t i = 0; i < static_cast<std::size_t>(PushConstantType::kMax); ++i) {
    const auto type = static_cast<PushConstantType>(i);
    if (desc_map.hasPushConstantMpa(type)) {
      const ShaderDescMap::PushConstantMap& map = desc_map.pushConstantMap(type);
      add_line("    desc_map->setPushConstantMap(");
      std::snprintf(tmp.data(), n, "        static_cast<PushConstantType>(%zu),", i);
      add_line(tmp.data());
      std::snprintf(tmp.data(), n, "        {%u, %u}", map.offset_, map.size_);
      add_line(tmp.data());
      add_line("    );");
    }
  }

  // kernel desc map
  const auto& kernel_desc_map_list = desc_map.kernelDescMapList();
  using ValueT = std::remove_cvref_t<decltype(kernel_desc_map_list)>::value_type;
  add_line("    auto& kernel_desc_map_list = desc_map->kernelDescMapList();");
  add_line("    zisc::pmr::memory_resource* mem = kernel_desc_map_list.get_allocator().resource();");
  for (const ValueT& value : kernel_desc_map_list) {
    add_line("    {");
    const auto id = static_cast<unsigned long long>(value.first);
    std::snprintf(tmp.data(), n, "      auto [it, result] = kernel_desc_map_list.emplace(0x%llx, mem);", id);
    add_line(tmp.data());
    const ShaderDescMap::KernelDescMap& map = value.second;
    add_line("      it->second.buffer_map_list_ = {");
    for (auto it = map.buffer_map_list_.begin(); it != map.buffer_map_list_.end(); ) {
      const ShaderDescMap::KernelDescMap::BufferMap& bmap = *it;
      ++it;
      const char delimiter = (it != map.buffer_map_list_.end()) ? ',' : ' ';
      std::snprintf(tmp.data(), n, "      {%u}%c", bmap.binding_, delimiter);
      add_line(tmp.data());
    }
    add_line("      };");
    add_line("      it->second.local_map_list_ = {");
    for (auto it = map.local_map_list_.begin(); it != map.local_map_list_.end(); ) {
      const ShaderDescMap::KernelDescMap::LocalMap& lmap = *it;
      ++it;
      const char delimiter = (it != map.local_map_list_.end()) ? ',' : ' ';
      std::snprintf(tmp.data(), n, "      {%u}%c", lmap.spec_id_, delimiter);
      add_line(tmp.data());
    }
    add_line("      };");
    add_line("    }");
  }

  // spec constant map
  using SpecConstantType = ShaderDescMap::SpecConstantType;
  add_line("    using SpecConstantType = ShaderDescMap::SpecConstantType;");
  for (std::size_t i = 0; i < static_cast<std::size_t>(SpecConstantType::kMax); ++i) {
    const auto type = static_cast<SpecConstantType>(i);
    if (desc_map.hasSpecConstantMpa(type)) {
      const ShaderDescMap::SpecConstantMap& map = desc_map.specConstantMap(type);
      add_line("    desc_map->setSpecConstantMap(");
      std::snprintf(tmp.data(), n, "        static_cast<SpecConstantType>(%zu),", i);
      add_line(tmp.data());
      std::snprintf(tmp.data(), n, "        {%u}", map.spec_id_);
      add_line(tmp.data());
      add_line("    );");
    }
  }

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
    std::cerr << "[Error] Baked mape file'" << file_path << "' creation failed."
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
  ::printDebugMessage("Bake a descriptor map");
  // Input parameters
  const std::string_view exec_path{argv[0]};
  const std::string_view map_file_path{argv[1]};
  const std::string_view baked_map_file_path{argv[2]};
  ::printDebugMessage("    Input parameters info");
  ::printDebugMessage("        Src map file path: ", map_file_path.data());
  ::printDebugMessage("        Dst baked file path: ", baked_map_file_path.data());
  ::Parameters params;
  zisc::AllocFreeResource mem_resource;
  params.mem_resource_ = std::addressof(mem_resource);
  params.kernel_set_name_ = argv[3];
  ::printDebugMessage("        Kernel set name: '", params.kernel_set_name_.data(), "'");

  std::ifstream map_file = ::loadMapFile(map_file_path);
  const ::ShaderDescMap desc_map = ::parse(map_file, params);

  // Generate a hpp file of the bake code
  {
    const zisc::pmr::string::allocator_type alloc{params.mem_resource_};
    const zisc::pmr::string tmp{alloc};
    zisc::pmr::stringstream bake_code{tmp};
    ::generateBakeCode(desc_map, params, std::addressof(bake_code));
    zisc::BSerializer::backToBegin(std::addressof(bake_code));
    ::saveBakeCode(baked_map_file_path, bake_code);
  }

  // Memory usage
  ::printDebugMessage("    Memory usage");
  ::printDebugMessage("        Peak usage: ",
                      ::toMegaBytes(mem_resource.peakMemoryUsage()),
                      " MB.");

  return EXIT_SUCCESS;
}
