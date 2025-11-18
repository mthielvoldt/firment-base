cmake_minimum_required(VERSION 3.25)

add_library(fmt_transport
  fmt_transport_test.c
)

target_include_directories(fmt_transport
  PUBLIC
    .
)

target_link_libraries(fmt_transport fmt_queue)