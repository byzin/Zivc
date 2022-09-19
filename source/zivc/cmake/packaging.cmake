# file: packaging.cmake
# author: Sho Ikeda
#
# Copyright (c) 2015-2022 Sho Ikeda
# This software is released under the MIT License.
# http://opensource.org/licenses/mit-license.php
#


cmake_path(SET __zisc_packaging_cmake_path NORMALIZE
  "${CMAKE_CURRENT_LIST_DIR}/../../dependencies/Zisc/source/zisc/cmake/packaging.cmake")


function(Zivc_getSaveDependencyCode target output_dir exe_path dirs rpaths code)
  include("${__zisc_packaging_cmake_path}")
  Zisc_getSaveDependencyCode(${target} "${output_dir}" "${exe_path}" "${dirs}" "${rpaths}" code_out)

  # Output
  set(${code} ${code_out} PARENT_SCOPE)
endfunction(Zivc_getSaveDependencyCode)


function(Zivc_getDeployingNonSystemDependencyCode target output_dir exe_path dirs rpaths code)
  include("${__zisc_packaging_cmake_path}")
  Zisc_getDeployingNonSystemDependencyCode(${target} "${output_dir}" "${exe_path}" "${dirs}" "${rpaths}" code_out)

  # Output
  set(${code} ${code_out} PARENT_SCOPE)
endfunction(Zivc_getDeployingNonSystemDependencyCode)


function(Zivc_getMakeSimpleRunScriptCode target bin_dir lib_dir env_vars script_name output_dir code)
  include("${__zisc_packaging_cmake_path}")
  Zisc_getMakeSimpleRunScriptCode(${target} "${bin_dir}" "${lib_dir}" "${env_vars}" "${script_name}" "${output_dir}" code_out)

  # Output
  set(${code} ${code_out} PARENT_SCOPE)
endfunction(Zivc_getMakeSimpleRunScriptCode)


function(Zivc_getDeployingVulkanDependencyCode lib_dir share_dir code envs)
  set(${code} "" PARENT_SCOPE)
  if((NOT ZIVC_ENABLE_VULKAN_BACKEND) OR Z_WINDOWS)
    return()
  endif()

  set(env_variables "")

  # Find vulkan directory
  if((NOT TARGET Zivc::Vulkan) OR (NOT Vulkan_LIBRARY))
    message(WARNING "Vulkan directory not found.")
    return()
  endif()
  cmake_path(SET vulkan_dir "${Vulkan_LIBRARY}")
  cmake_path(GET vulkan_dir PARENT_PATH vulkan_dir)
  cmake_path(GET vulkan_dir PARENT_PATH vulkan_dir)
  message(STATUS "vulkan root directory: ${vulkan_dir}")

  # List vulkan libraries to be copied
  set(vulkan_libs ${Vulkan_LIBRARY})
  if(Z_MAC)
    # MoltenVK
    cmake_path(SET moltenvk_lib "lib/libMoltenVK.dylib")
    cmake_path(APPEND moltenvk_path "${vulkan_dir}" "${moltenvk_lib}")
    if(NOT EXISTS "${moltenvk_path}")
      message(WARNING "'${moltenvk_lib}' not found in vulkan dir '${vulkan_dir}'.")
      return()
    endif()
    list(APPEND vulkan_libs "${moltenvk_path}")
  endif()
  if(Z_MAC)
    list(APPEND env_variables "DYLD_LIBRARY_PATH=\\\"\\\${VULKAN_SDK}/${lib_dir}:\\\${DYLD_LIBRARY_PATH:-}\\\"")
  elseif(Z_LINUX)
    list(APPEND env_variables "LD_LIBRARY_PATH=\\\"\\\${VULKAN_SDK}/${lib_dir}\\\${LD_LIBRARY_PATH:+:\\\$LD_LIBRARY_PATH}\\\"")
  endif()

  # List vulkan files to be copied
  set(vulkan_files "")
  # Layer files
  if(DEFINED ENV{VK_LAYER_PATH})
    cmake_path(SET vk_layer_path $ENV{VK_LAYER_PATH})
  elseif(Z_MAC)
    cmake_path(SET vk_layer_path "${vulkan_dir}/share/vulkan/explicit_layer.d")
  elseif(Z_LINUX)
    cmake_path(SET vk_layer_path "${vulkan_dir}/etc/vulkan/explicit_layer.d")
  endif()
  if(EXISTS "${vk_layer_path}")
    list(APPEND vulkan_files "${vk_layer_path}")
    list(APPEND env_variables "VK_LAYER_PATH=\\\"\\\${VULKAN_SDK}/${share_dir}/vulkan/explicit_layer.d\\\"")
  else()
    message(WARNING "'${vk_layer_path}' not found in vulkan dir '${vulkan_dir}'.")
  endif()
  # ICD files
  if(DEFINED ENV{VK_ICD_FILENAMES})
    cmake_path(SET vk_icd_path $ENV{VK_ICD_FILENAMES})
  elseif(Z_MAC)
    cmake_path(SET vk_icd_path "${vulkan_dir}/share/vulkan/icd.d/MoltenVK_icd.json")
  endif()
  if(EXISTS "${vk_icd_path}")
    list(APPEND vulkan_files "${vk_icd_path}")
    list(APPEND env_variables "VK_ICD_FILENAMES=\\\"\\\${VULKAN_SDK}/${share_dir}/vulkan/icd.d/MoltenVK_icd.json\\\"")
  endif()


  set(dependency_code "")
  string(APPEND dependency_code
      "include(\"${CMAKE_CURRENT_FUNCTION_LIST_FILE}\")\n"
      "Zivc_deployVulkanDependencies(\"\${CMAKE_INSTALL_PREFIX}\" \"${lib_dir}\" \"${share_dir}\" \"${vulkan_libs}\" \"${vulkan_files}\")\n"
      )


  # Output variables
  set(${code} ${dependency_code} PARENT_SCOPE)
  set(${envs} ${env_variables} PARENT_SCOPE)
endfunction(Zivc_getDeployingVulkanDependencyCode)


function(Zivc_deployVulkanDependencies output_prefix lib_dir share_dir vulkan_libs vulkan_files)
  message("-- Deploy vulkan files.")
  foreach(library IN LISTS vulkan_libs)
    message("--   Install vulkan library: '${library}'.")
    file(INSTALL ${library}
         DESTINATION "${output_prefix}/${lib_dir}"
         FOLLOW_SYMLINK_CHAIN)
  endforeach(library)
  foreach(file IN LISTS vulkan_files)
    message("--   Install vulkan file: '${file}'.")
    file(COPY ${file}
         DESTINATION "${output_prefix}/${share_dir}"
         FOLLOW_SYMLINK_CHAIN)
  endforeach(file)
endfunction(Zivc_deployVulkanDependencies)
