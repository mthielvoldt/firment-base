# Creates a virtual environment (venv) for protobuf generation if not present. 
include(cmake/nanopb_py_env.cmake)

set(PROJECT_PROTO_PREFIX project_msg)
set(PROJECT_PROTO ${PROJECT_CONFIG_DIR}/${PROJECT_PROTO_PREFIX}.proto) # TODO: mv to config? 
set(NANOPB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules/nanopb)
set(PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pb-plugins)
set(PROTOC ${NANOPB_DIR}/generator/protoc)
set(PB_OUT_DIR ${PROJECT_BINARY_DIR}/pb)

add_library(fmt_base
  ${PB_OUT_DIR}/project_msg.pb.c 
  ${PB_OUT_DIR}/fmt_base_msg.pb.c
  ${PB_OUT_DIR}/fmt_rx.pb.c
  ${NANOPB_DIR}/pb_encode.c
  ${NANOPB_DIR}/pb_decode.c
  ${NANOPB_DIR}/pb_common.c
  src/fmt_comms.c
)

target_include_directories(fmt_base
  PUBLIC
    inc # fmt_rx.h used by fmt_rx.pb.c
    ${PB_OUT_DIR} # project_msg.pb.h
    ${NANOPB_DIR} # pb.h
  PRIVATE
    src # fmt_base_msg.h
    ${PROJECT_CONFIG_DIR} # project_msg.h
)

target_link_libraries(fmt_base PRIVATE
  fmt_transport
  fmt_crc
)
if(ESP_PLATFORM)
  target_link_libraries(fmt_base PRIVATE idf::fmt_queue)
else()
  target_link_libraries(fmt_base PRIVATE fmt_queue)
endif()

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/web-ui)
set(UI_GENERATED_DIR ${UI_DIR}/src/generated)

add_custom_command(
  OUTPUT
    ${PB_OUT_DIR}/project_msg.pb.c 
    ${PB_OUT_DIR}/fmt_base_msg.pb.c 
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

### From here down in the file is the firment web-UI "library"

set(UI_DIST_DIR      ${UI_DIR}/dist)
set(UI_SRC_DIR       ${UI_DIR}/src)

# A target to identify the end-result of all ui-related custom commands.
add_library(firment_ui INTERFACE 
  ${UI_DIST_DIR}/generated/messages.js  # Result of message-parser codegen chain
  ${UI_DIST_DIR}/src/Reset.js # Output of "Building web-ui..."
)

add_custom_command(
  COMMENT "npm install (building node_modules)"
  OUTPUT ${UI_DIR}/node_modules
  WORKING_DIRECTORY ${UI_DIR}
  COMMAND npm install --no-audit
  DEPENDS ${UI_DIR}/package.json
)

add_custom_command(
  COMMENT "Building firment-ui npm package. (npm run build)"
  OUTPUT ${UI_DIST_DIR}/src/Reset.js
  WORKING_DIRECTORY ${UI_DIR}
  COMMAND npm run build
  DEPENDS
    ${UI_SRC_DIR}/App.css
    ${UI_SRC_DIR}/BrokerAddress.tsx
    ${UI_SRC_DIR}/Reset.tsx
    ${UI_SRC_DIR}/Version.tsx
    ${UI_SRC_DIR}/index.ts
    ${UI_SRC_DIR}/mqclient.ts
)

add_custom_command(
  COMMENT "Generating message parsing JS"
  OUTPUT ${UI_SRC_DIR}/generated/messages.js
  COMMAND 
    ${UI_DIR}/node_modules/protobufjs-cli/bin/pbjs
      -p ${NANOPB_DIR}/generator/proto
      -p ${CMAKE_CURRENT_SOURCE_DIR}/src
      -p ${PROJECT_CONFIG_DIR}  # PROJECT_PROTO
      -t static-module
      -w es6
      -o ${UI_SRC_DIR}/generated/messages.js
      ${PROJECT_PROTO}
  DEPENDS
    ${COMPONENT_PROTOS}
    ${PROJECT_PROTO}
    src/fmt_base_msg.proto
    ${UI_DIR}/node_modules/ # Does this re-run when any package inside changes?
)

add_custom_command(
  COMMENT "Transforming message parsing JS into TS"
  OUTPUT ${UI_SRC_DIR}/generated/messages.d.ts
  COMMAND
    ${UI_DIR}/node_modules/protobufjs-cli/bin/pbts
    -o ${UI_SRC_DIR}/generated/messages.d.ts
    ${UI_SRC_DIR}/generated/messages.js
  DEPENDS
    ${UI_SRC_DIR}/generated/messages.js
    ${UI_DIR}/node_modules/
)

add_custom_command(
  COMMENT "Copy generated message parsing JS/TS to dist/generated."
  OUTPUT 
    ${UI_DIST_DIR}/generated/messages.js 
    ${UI_DIST_DIR}/generated/messages.d.ts
  WORKING_DIRECTORY ${UI_DIR}
  COMMAND 
    mkdir -p ${UI_DIST_DIR}/generated &&
    cp src/generated/messages.* dist/generated/
  DEPENDS 
    ${UI_SRC_DIR}/generated/messages.d.ts
)
