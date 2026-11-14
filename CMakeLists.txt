cmake_minimum_required(VERSION 3.24)
project(SpaceEngine LANGUAGES CXX)

if(NOT WIN32 OR NOT MSVC)
    message(FATAL_ERROR "SpaceEngine requires Windows and the MSVC compiler.")
endif()

include(FetchContent)
FetchContent_Declare(
    DirectXHeaders
    GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
    GIT_TAG 61c7718954ec73dbdd43c723240fe1a0452a3724
)
FetchContent_MakeAvailable(DirectXHeaders)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.inl"
)

add_executable(SpaceEngine src/main.cpp ${PROJECT_HEADERS}
    src/logger.cpp)
target_include_directories(SpaceEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers")
target_compile_options(SpaceEngine PRIVATE /std:c++latest /permissive- /W4 /EHsc)
target_compile_definitions(SpaceEngine PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(SpaceEngine PRIVATE Microsoft::DirectX-Headers d3d12 dxgi d3dcompiler)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT SpaceEngine)

include(CTest)
if(BUILD_TESTING)
    add_test(NAME render_box COMMAND SpaceEngine --test-render)
    set_tests_properties(render_box PROPERTIES TIMEOUT 30)
endif()
