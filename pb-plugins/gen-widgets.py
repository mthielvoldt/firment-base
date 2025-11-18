#!/usr/bin/env python3

import sys
from typing import Dict
from google.protobuf.compiler.plugin_pb2 import CodeGeneratorResponse, CodeGeneratorRequest
from google.protobuf.descriptor_pb2 import FileDescriptorProto, DescriptorProto, EnumDescriptorProto, FieldDescriptorProto
from google.protobuf.descriptor import FieldDescriptor


header = """\
// Generated File, do not track.
// Implements react modules for each message defined in .proto files.
import { useState, useEffect } from "react";
import { setMessageHandler, sendMessage } from "../mqclient";

"""

float_fields = (
  FieldDescriptor.TYPE_FLOAT,
  FieldDescriptor.TYPE_DOUBLE
)
integer_fields = (
  FieldDescriptor.TYPE_INT32,
  FieldDescriptor.TYPE_INT64,
  FieldDescriptor.TYPE_UINT32,
  FieldDescriptor.TYPE_UINT64,
  FieldDescriptor.TYPE_SINT32,
  FieldDescriptor.TYPE_SINT64,
  FieldDescriptor.TYPE_FIXED32,
  FieldDescriptor.TYPE_FIXED64,
  FieldDescriptor.TYPE_SFIXED32,
  FieldDescriptor.TYPE_SFIXED64
)

enums = {}
enum_strings = ""
enums_used = set()

def get_message_widget(message: DescriptorProto, enums: Dict[str, EnumDescriptorProto]):
  if message.name.endswith("Ctl"):
    return get_ctl_widget(message, enums)
  elif message.name.endswith("Tlm"):
    return get_tlm_widget(message, enums)
  else:
    return "" # Perhaps an error?

def get_ctl_widget(message: DescriptorProto, enums: Dict[str, EnumDescriptorProto]):
  field_strings = ""
  initial_state = {}

  for field in message.field:
    if field.type in integer_fields:
      initial_state[field.name] = 0
      field_strings += f'''
      <label>
        <input className="field" type="number" size={{5}} step="1" 
          value={{state.{field.name}}} name="{field.name}"
          onChange={{e => setState({{...state, {field.name}:Number(e.target.value)}})}}/>
        {field.name}
      </label>
      <br/>'''
    if field.type in float_fields:
      initial_state[field.name] = 0
      field_strings += f'''
      <label>
        <input className="field" type="number" size={{5}} step="0.01" 
          value={{state.{field.name}}} name="{field.name}"
          onChange={{e => setState({{...state, {field.name}:Number(e.target.value)}})}}/>
        {field.name}
      </label>
      <br/>'''
    if field.type == FieldDescriptor.TYPE_BOOL:
      initial_state[field.name] = False
      field_strings += f'''
      <label>
        <input className="field" type="checkbox" checked={{state.{field.name}}}
          onChange={{e => setState({{...state, {field.name}:e.target.checked}})}} />
        {field.name}
      </label>
      <br/>'''
    if field.type == FieldDescriptor.TYPE_ENUM:
      initial_state[field.name] = 0
      options = get_options_from_enum(enums, field)
        
      field_strings += f'''
      <label>
        <select name="{field.name}" value={{state.{field.name}}}
          onChange={{e => setState({{
            ...state, {field.name}:Number(e.target.value)}})}}>{options}
        </select>
        {field.name}
      </label>
      <br/>'''

  useState_line = ""
  sentState = "{}"
  if len(initial_state) > 0:
    initial_state_str = str(initial_state).replace("True", "true").replace("False","false")
    useState_line = f"const [state, setState] = useState({initial_state_str});\n"
    sentState = "state"

  return f'''
export function {message.name}({{}}) {{
  {useState_line}
  function handleSubmit(e: React.FormEvent) {{
    e.preventDefault();
    sendMessage("{message.name}", {sentState});
  }}

  return (
    <details className="widget">
      <summary>{message.name}</summary>
      <form aria-label="{message.name}" onSubmit={{handleSubmit}}>
        {field_strings}
        <button type="submit">Send</button>
      </form>
    </details>
  );
}}
'''

def get_tlm_widget(message: DescriptorProto, enums: Dict[str, EnumDescriptorProto]):
  # spec a div with a name
  message_name = message.name
  field_strings = ""
  initial_state = {}
  setState = f"set{message_name}State"

  # iterate through fields, adding an element for each type.
  for field in message.field:
    if field.type in integer_fields:
      initial_state[field.name] = 0  
      field_strings += f'''
      <dt>{field.name}</dt>
      <dd data-testid="tlm-{field.name}">{{{message_name}State.{field.name}}}</dd>
      '''
    if field.type in float_fields:
      initial_state[field.name] = 0.0  
      field_strings += f'''
      <dt>{field.name}</dt>
      <dd data-testid="tlm-{field.name}">{{{message_name}State.{field.name}.toPrecision(4)}}</dd>
      '''
    if field.type == FieldDescriptor.TYPE_BOOL:
      initial_state[field.name] = False
      field_strings += f'''
      <dt>{field.name}</dt>
      <dd data-testid="tlm-{field.name}">{{{message_name}State.{field.name} ? "x" : ""}}</dd>
      '''
    if field.type == FieldDescriptor.TYPE_ENUM:
      add_table_for_enum(enums, field)
      initial_state[field.name] = 0   
      field_strings += f'''
      <dt>{field.name}</dt>
      <dd data-testid="tlm-{field.name}">{{{field.type_name[1:]}[{message_name}State.{field.name}]}}</dd>
      '''
  # enum_name = ""
  # enum = next(enum for enum in proto.enum_type if enum.name == enum_name)
  initial_state_str = str(initial_state).replace("True", "true").replace("False","false")
  
  return f'''
export function {message_name}({{}}) {{
  const [{message_name}State, {setState}] = useState({initial_state_str});
  useEffect( () => {{
    setMessageHandler("{message_name}", {setState});
  }}, []);

  return (
    <details className="widget">
      <summary>{message_name}</summary>
      <dl className='telemetry'>
        {field_strings}
      </dl>
    </details>
  );
}}
'''

def add_table_for_enum(enums: Dict[str, EnumDescriptorProto], field: FieldDescriptorProto):
  global enum_strings
  enum_name = field.type_name[1:]
  if enum_name in enums_used:
    return
  enums_used.add(enum_name)
  enum_items = enums[enum_name].value

  item_lines = ""
  for item in enum_items:
    item_lines += f"\n  {item.name} = {item.number},"

  enum_strings += f"enum {enum_name} {{{item_lines}\n}};\n"

def get_options_from_enum(enums, field: FieldDescriptorProto):
  options = ""
  for value in enums[field.type_name[1:]].value:
    options += f'''
      <option value="{value.number}">{value.name}</option>'''
  return options
  
def get_widgets_from_proto_file(proto: FileDescriptorProto):
  ret = ""

  for message in proto.message_type:
    widget_str = get_message_widget(message, enums)
    ret += widget_str

  if False:  # Control appending contents of parsed data as a comment
    ret += "/*"
    ret += str(enums)
    # ret += str(type(enums["WaveShape"]))
    # ret += str(proto)
    # ret += str(type(proto))
    # ret += str(type(proto.enum_type))
    ret += "*/"
  return ret

def generate_widgets(request: CodeGeneratorRequest) -> str:
  widget_functions = ""

  # Find all the enums in all files (whether generated or not)
  for file in request.proto_file:
    enums.update({ enum.name: enum for enum in file.enum_type })

  for file_name in request.file_to_generate:
    proto = next(file for file in request.proto_file if file.name == file_name)
    widget_functions += get_widgets_from_proto_file(proto)

  # body += request.source_file_descriptors + "\n"
  return header + enum_strings + widget_functions

if __name__ == "__main__":
  request = CodeGeneratorRequest.FromString(sys.stdin.buffer.read())
  response = CodeGeneratorResponse()
  widgets_file = response.file.add(name="widgets.pb.tsx")
  widgets_file.content = generate_widgets(request)
  sys.stdout.buffer.write(response.SerializeToString())