cmake_minimum_required(VERSION 3.25)

include(FetchContent)
FetchContent_Declare(
  fmt-transport-iface
  GIT_REPOSITORY https://github.com/mthielvoldt/firment-transport-iface.git
  GIT_TAG v0.12.2
)
FetchContent_MakeAvailable(fmt-transport-iface)

add_library(fmt_transport
  fmt_transport_test.c
)

target_include_directories(fmt_transport
  PUBLIC
    .
)

target_link_libraries(fmt_transport 
  fmt_queue
  fmt_transport_iface
)