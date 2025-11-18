#!/usr/bin/env python3

import sys
from pathlib import Path
from google.protobuf.compiler.plugin_pb2 import CodeGeneratorResponse, CodeGeneratorRequest
from google.protobuf.descriptor_pb2 import FileDescriptorProto, DescriptorProto
from google.protobuf.descriptor import FieldDescriptor

CASES_MARKER = '/*--CASE BLOCKS GENERATED CONTENT--*/'
INCLUDES_MARKER = '/*--MODULE INCLUDES GENERATED CONTENT--*/'

def get_case_str(message: DescriptorProto):
  return f'''#ifdef USE_{message.name}
    case Top_{message.name}_tag:
    handle{message.name}(incomingMessage.sub.{message.name});
    break;
#endif
'''
  
def digest_proto(proto: FileDescriptorProto):
  ret = ""
  excluded = ("Top" "Ack")
  for message in proto.message_type:
    if message.name not in excluded:
      case_str = get_case_str(message)
      ret += case_str
  return ret

def include_line_from_file(file_name: str) -> str:
  file_name_stripped = file_name.removesuffix('.proto')
  return f'#include <{file_name_stripped}.h>\n'

def digest_all_protos(request: CodeGeneratorRequest) -> str:
  case_content, include_content = "", ""
  for file_name in request.file_to_generate:
    proto = next(file for file in request.proto_file if file.name == file_name)
    case_content += digest_proto(proto)
    include_content += include_line_from_file(file_name)
  return case_content, include_content


def generate_code(request: CodeGeneratorRequest) -> str:
  template_path = Path(__file__).parent.parent / "src/fmt_rx.in.c"
  with open(template_path, "r") as template_file:
    static_content_with_markers = template_file.read()

  case_content, include_content = digest_all_protos(request)

  # print(include_content + case_content)

  output = static_content_with_markers.replace(INCLUDES_MARKER, include_content)
  output = output.replace(CASES_MARKER, case_content)

  return output

if __name__ == "__main__":
  request = CodeGeneratorRequest.FromString(sys.stdin.buffer.read())
  response = CodeGeneratorResponse()
  widgets_file = response.file.add(name="fmt_rx.pb.c")
  widgets_file.content = generate_code(request)
  sys.stdout.buffer.write(response.SerializeToString())