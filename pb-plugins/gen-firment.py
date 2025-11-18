#!/usr/bin/env python3

import sys
from pathlib import Path
from google.protobuf.compiler.plugin_pb2 import CodeGeneratorResponse, CodeGeneratorRequest
from google.protobuf.descriptor_pb2 import FileDescriptorProto, DescriptorProto
from google.protobuf.descriptor import FieldDescriptor

MARKER = '/*--GENERATED CONTENT MARKER--*/'

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

def generate_code(request: CodeGeneratorRequest) -> str:
  dynamic_content = ""
  for file_name in request.file_to_generate:
    proto = next(file for file in request.proto_file if file.name == file_name)
    dynamic_content += digest_proto(proto)

  template_path = Path(__file__).parent.parent.parent / "firmware/fmt_rx.in.c"
  
  with open(template_path, "r") as template_file:
    static_content_with_marker = template_file.read()

  return static_content_with_marker.replace(MARKER, dynamic_content)

if __name__ == "__main__":
  request = CodeGeneratorRequest.FromString(sys.stdin.buffer.read())
  response = CodeGeneratorResponse()
  widgets_file = response.file.add(name="fmt_rx.pb.c")
  widgets_file.content = generate_code(request)
  sys.stdout.buffer.write(response.SerializeToString())