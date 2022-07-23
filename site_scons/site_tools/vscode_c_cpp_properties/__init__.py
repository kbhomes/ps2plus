import SCons.Builder
import json
import os
from vscode import VSCodeProperties

def build_vscode_c_cpp_properties_configuration(
    env,
    name: str,
    properties: VSCodeProperties
):
    include_paths = []
    include_paths += [os.path.join('${workspaceFolder}', env.Dir(path).relpath) for path in env['CPPPATH']]
    include_paths += (properties.additional_includes or [])

    defines = []
    defines += env['CPPDEFINES']
    defines += (properties.additional_defines or [])

    return env.Value({
        "name": name,
        "includePath": include_paths,
        "defines": defines,
        "compilerPath": properties.compiler_path or env['CC'][0],
        "cStandard": properties.c_standard,
        "intelliSenseMode": properties.intellisense_mode,
    })

def build_vscode_c_cpp_properties_file(target, source, env):
    # TODO: Consider merging into existing files?
    with open(str(target[0]), 'w') as f:
        json.dump({
            'configurations': [source_node.read() for source_node in source if source_node]
        }, fp=f, indent=2)

_vscode_c_cpp_properties_file_builder = SCons.Builder.Builder(action=build_vscode_c_cpp_properties_file)

def generate(env):
    env.AddMethod(build_vscode_c_cpp_properties_configuration, 'VSCodeCCppPropertiesConfiguration')
    env['BUILDERS']['VSCodeCCppProperties'] = _vscode_c_cpp_properties_file_builder

def exists(env):
    return 1