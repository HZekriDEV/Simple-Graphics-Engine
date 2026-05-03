set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(TINYOBJLOADER_BUILD_TEST_LOADER OFF CACHE BOOL "" FORCE)
set(TINYOBJLOADER_BUILD_OBJ_STICHER OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(tinyobjloader)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_LIST_DIR}/stb")

find_package(Vulkan REQUIRED)

add_library(ProjectLibraries INTERFACE)

target_compile_definitions(ProjectLibraries INTERFACE
  GLFW_INCLUDE_VULKAN
  VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

target_link_libraries(ProjectLibraries INTERFACE
  glfw
  glm::glm
  stb
  tinyobjloader
  Vulkan::Vulkan
)
