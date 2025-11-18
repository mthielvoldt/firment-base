cmake_minimum_required(VERSION 3.25)

add_library(fmt_queue
  queue_test.c
)

target_include_directories(fmt_queue
  PUBLIC
    .
)
