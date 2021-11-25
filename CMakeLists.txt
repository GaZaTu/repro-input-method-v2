cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(test VERSION 0.1.0 LANGUAGES C)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
  cmake_policy(SET CMP0074 NEW)
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_library(WAYLAND_CLIENT_LIB wayland-client)
add_library(WAYLAND_CLIENT STATIC SHARED IMPORTED)
set_target_properties(WAYLAND_CLIENT PROPERTIES
  IMPORTED_LOCATION ${WAYLAND_CLIENT_LIB}
)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SRC_FILES
  src/main.c
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(WaylandScanner)
if (${WaylandScanner_FOUND})
  file(
    DOWNLOAD https://gitlab.freedesktop.org/wayland/wayland-protocols/-/raw/main/unstable/input-method/input-method-unstable-v1.xml
    ${PROJECT_SOURCE_DIR}/src/wayland/protocols/unstable/input-method/input-method-unstable-v1.xml
  )

  ecm_add_wayland_client_protocol(SRC_FILES
    PROTOCOL ${PROJECT_SOURCE_DIR}/src/wayland/protocols/unstable/input-method/input-method-unstable-v1.xml
    BASENAME input-method-unstable-v1
  )

  file(
    DOWNLOAD https://gitlab.freedesktop.org/wlroots/wlroots/-/raw/master/protocol/input-method-unstable-v2.xml
    ${PROJECT_SOURCE_DIR}/src/wayland/protocols/wlroots/input-method-unstable-v2.xml
  )

  ecm_add_wayland_client_protocol(SRC_FILES
    PROTOCOL ${PROJECT_SOURCE_DIR}/src/wayland/protocols/wlroots/input-method-unstable-v2.xml
    BASENAME input-method-unstable-v2
  )
endif ()

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  WIN32_EXECUTABLE ON # add WinMain(...) if windows
)

target_link_libraries(${PROJECT_NAME}
  WAYLAND_CLIENT
)
