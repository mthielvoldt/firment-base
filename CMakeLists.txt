# Creates a virtual environment (venv) for protobuf generation if not present. 
include(cmake/nanopb_py_env.cmake)

set(PROJECT_PROTO ${PROJECT_CONFIG_DIR}/project_msg.proto) # TODO: mv to config? 
set(NANOPB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/nanopb)
set(PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pb-plugins)
set(PROTOC ${NANOPB_DIR}/generator/protoc)
set(PB_OUT_DIR ${PROJECT_BINARY_DIR}/pb)

add_library(firment-base
  ${PB_OUT_DIR}/project_msg.pb.c 
  ${PB_OUT_DIR}/fmt_base_msg.pb.c
  ${PB_OUT_DIR}/fmt_rx.pb.c
  src/fmt_comms.c
)

target_include_directories(firment-base
  PUBLIC
    inc # fmt_rx.h used by fmt_rx.pb.c
  PRIVATE
    src # fmt_base_msg.h
    ${PROJECT_CONFIG_DIR} # project_msg.h
    ${PB_OUT_DIR} # project_msg.pb.h
    ${NANOPB_DIR} # pb.h
)

target_link_libraries(firment-base 
  fmt_transport
  fmt_queue
  fmt_crc
)

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/web-ui)
set(UI_DIST_DIR      ${UI_DIR}/dist)
set(UI_SRC_DIR       ${UI_DIR}/src)
set(UI_GENERATED_DIR ${UI_DIR}/src/generated)

add_custom_command(
  OUTPUT
    ${PB_OUT_DIR}/project_msg.pb.c 
    ${PB_OUT_DIR}/fmt_base.pb.c 
    ${PB_OUT_DIR}/fmt_rx.pb.c 
    ${UI_GENERATED_DIR}/widgets.pb.tsx
  COMMAND 
    ${VENV_ACTIVATE} &&
    mkdir -p ${UI_GENERATED_DIR} ${PB_OUT_DIR} &&
    ${PROTOC}
      -I ${PROJECT_CONFIG_DIR} # PROJECT_PROTO
      -I ${CMAKE_CURRENT_SOURCE_DIR}/src # src/fmt_base.proto
      --plugin=protoc-gen-fmt_rx=${PLUGIN_DIR}/gen-fmt_rx.py
      --plugin=protoc-gen-widgets=${PLUGIN_DIR}/gen-widgets.py
      --fmt_rx_out=${PB_OUT_DIR}
      --widgets_out=${UI_GENERATED_DIR}
      --nanopb_out=${PB_OUT_DIR}
      ${PROJECT_PROTO} fmt_base_msg.proto ${COMPONENT_PROTOS} 
  DEPENDS
    ${VENV_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fmt_rx.in.c
    ${PLUGIN_DIR}/gen-fmt_rx.py
    ${PLUGIN_DIR}/gen-widgets.py
    ${COMPONENT_PROTOS}
    ${PROJECT_PROTO}
    src/fmt_base_msg.proto
)

