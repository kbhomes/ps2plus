from pprint import pprint
import SCons.Builder
from SCons.Script import *
import json
import os
import copy
import collections
import xml.etree.ElementTree as ET
from mplabx import MPLABXProperties

MAKEFILE_TEXT = '''
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib

build: .build-post
.build-pre:
.build-post: .build-impl

clean: .clean-post
.clean-pre:
.clean-post: .clean-impl

clobber: .clobber-post
.clobber-pre:
.clobber-post: .clobber-impl

all: .all-post
.all-pre:
.all-post: .all-impl

help: .help-post
.help-pre:
.help-post: .help-impl

include nbproject/Makefile-impl.mk
include nbproject/Makefile-variables.mk
'''

PROJECT_XML_TEXT = '''
<project>
    <type>com.microchip.mplab.nbide.embedded.makeproject</type>
    <configuration>
        <data>
            <name />
            <sourceRootList />
            <confList />
        </data>
    </configuration>
</project>
'''

CONFIGURATIONS_XML_TEXT = '''
<configurationDescriptor version="65">
  <logicalFolder name="root" displayName="root" projectFiles="true" />
  <sourceRootList />
  <projectmakefile>Makefile</projectmakefile>
  <confs />
</configurationDescriptor>
'''

CONFIGURATION_ELEMENT_TEXT = '''
<conf type="2">
    <toolsSet>
        <targetDevice />
        <languageToolchain />
        <languageToolchainVersion />
    </toolsSet>
    <HI-TECH-COMP />
    <HI-TECH-LINK />
    <XC8-config-global />
</conf>
'''

def nested_dict():
    return collections.defaultdict(nested_dict)

def merge(destination, source):
    for key, value in source.items():
        if isinstance(value, dict):
            # get node or create one
            node = destination.setdefault(key, {})
            merge(node, value)
        else:
            destination[key] = value

    return destination

def build_mplabx_nbproject_configuration(
    env,
    name: str,
    properties: MPLABXProperties,
    additional_compiler_properties: dict[str, str] = {},
    additional_linker_properties: dict[str, str] = {},
    additional_xc8_properties: dict[str, str] = {},
):
    defines_str = ';'.join(env['CPPDEFINES'])
    includes_str = ';'.join([env.Dir(path).abspath for path in env['CPPPATH']])

    default_compiler_properties = {
        'define-macros': f'{defines_str}',
        'extra-include-directories': f'{includes_str}',
    }

    root = ET.fromstring(CONFIGURATION_ELEMENT_TEXT)
    root.set('name', name)

    root.find('./toolsSet/targetDevice').text = properties.device
    root.find('./toolsSet/languageToolchain').text = properties.toolchain
    root.find('./toolsSet/languageToolchainVersion').text = properties.toolchain_version

    group_properties_mapping = {
        'HI-TECH-COMP': default_compiler_properties | properties.compiler_properties | additional_compiler_properties,
        'HI-TECH-LINK': properties.linker_properties | additional_linker_properties,
        'XC8-config-global': properties.xc8_properties | additional_xc8_properties,
    }

    for group_name, group_properties in group_properties_mapping.items():
        for key, value in group_properties.items():
            root.find(group_name).append(ET.Element('property', key=key, value=value))

    # ET.dump(root)
    return env.Value(root)

def _create_file_hierarchy(source_relpaths: list[str]):
    hierarchy = nested_dict()

    # Put all entries into the hierarchy, keyed from dirname to basename
    for source_relpath in sorted(source_relpaths):
        dirname, basename = os.path.split(source_relpath)
        hierarchy[dirname][basename] = source_relpath

    # Split all directory keys further
    while True:
        found_nested = False
        modified_hierarchy = nested_dict()
        for parent_key, entries in hierarchy.items():
            dirname, basename = os.path.split(parent_key)
            if dirname:
                merge(modified_hierarchy[dirname][basename], entries)
                found_nested = True
            else:
                merge(modified_hierarchy[parent_key], entries)

        hierarchy = modified_hierarchy

        if not found_nested:
            break

    return hierarchy

def _build_xml_files(project_name: str, project_dir, confs: list, source_files: list[str]):
    # Create the `configurations.xml` and `project.xml` file
    configurations_xml_root = ET.fromstring(CONFIGURATIONS_XML_TEXT)
    
    project_xml_root = ET.fromstring(PROJECT_XML_TEXT)
    project_xml_root.set('xmlns', 'http://www.netbeans.org/ns/project/1')
    project_xml_root.find('./configuration/data').set('xmlns', 'http://www.netbeans.org/ns/make-project/1')
    project_xml_root.find('./configuration/data/name').text = project_name

    # Add each configuration to the two XML files
    for configuration_node in confs:
        # Modify each configuration to make absolute paths relative to the project directory
        modified_node = copy.deepcopy(configuration_node.read())
        for includes_element in modified_node.findall('.//property[@key="extra-include-directories"]'):
            includes_value = includes_element.get('value')
            includes_relative = ';'.join([os.path.relpath(abspath, project_dir.abspath) for abspath in includes_value.split(';')])
            includes_element.set('value', includes_relative)
        configurations_xml_root.find('./confs').append(modified_node)

        # Update the `project.xml` configuration list
        project_conf_list_element = project_xml_root.find('./configuration/data/confList')
        project_conf_elem_element = ET.SubElement(project_conf_list_element, 'confElem')
        project_conf_name_element = ET.SubElement(project_conf_elem_element, 'name')
        project_conf_name_element.text = configuration_node.read().get('name')
        project_conf_text_element = ET.SubElement(project_conf_elem_element, 'text')
        project_conf_text_element.text = '2'

    # Generate the source root list, which will have a single root (common path for all sources)
    common_root_path = os.path.commonpath([os.path.abspath(path) for path in source_files])
    source_root_relpath = os.path.relpath(common_root_path, project_dir.abspath)

    configurations_source_root_element = ET.Element('Elem')
    configurations_source_root_element.text = source_root_relpath
    configurations_xml_root.find('./sourceRootList').append(configurations_source_root_element)

    project_source_root_element = ET.Element('sourceRootElem')
    project_source_root_element.text = os.path.relpath(common_root_path, project_dir.abspath)
    project_xml_root.find('./configuration/data/sourceRootList').append(project_source_root_element)

    # Generate all logical folders and private files
    root_logical_folder = configurations_xml_root.find('./logicalFolder[@name="root"]')
    source_relpaths = [os.path.relpath(source_path, common_root_path) for source_path in source_files]
    source_hierarchy = _create_file_hierarchy(source_relpaths)

    def _walk_tree(parent_element: ET.Element, tree: dict):
        for key, data in tree.items():
            if isinstance(data, dict):
                folder_element = ET.SubElement(parent_element, 'logicalFolder', name=key, displayName=key, projectFiles="true")
                _walk_tree(folder_element, data)
            elif isinstance(data, str):
                item_element = ET.SubElement(parent_element, 'itemPath')
                item_element.text = os.path.relpath(data, project_dir.abspath)
    
    _walk_tree(root_logical_folder, source_hierarchy)

    # Generate an item for the build Makefile
    ET.SubElement(root_logical_folder, 'itemPath').text = 'Makefile'

    return (configurations_xml_root, project_xml_root)

def build_mplabx_nbproject(target, source, env):
    '''
    target - (singleton list) - Directory node to the project folder
    source - (list) - XML value nodes for each project configuration
    '''
    project_dir = target[0]
    nbproject_dir = project_dir.Dir('nbproject')
    
    configurations_xml_file = nbproject_dir.File('configurations.xml')
    project_xml_file = nbproject_dir.File('project.xml')
    makefile_file = project_dir.File('Makefile')

    # Make the directories
    env.Execute(Mkdir(project_dir))
    env.Execute(Mkdir(nbproject_dir))

    # Generate the XML files 
    confs = source
    configurations_xml_root, project_xml_root = _build_xml_files(
        project_name=os.path.basename(str(project_dir)),
        project_dir=project_dir, 
        confs=confs, 
        source_files=env['source_files'])

    with open(str(configurations_xml_file), 'w') as f:
        ET.indent(configurations_xml_root, space='  ')
        ET.ElementTree(configurations_xml_root).write(f, encoding='unicode')

    with open(str(project_xml_file), 'w') as f:
        ET.indent(project_xml_root, space='  ')
        ET.ElementTree(project_xml_root).write(f, encoding='unicode')

    with open(str(makefile_file), 'w') as f:
        f.write(MAKEFILE_TEXT)

_mplabx_nbproject_builder = SCons.Builder.Builder(action=build_mplabx_nbproject)

def generate(env):
    env.AddMethod(build_mplabx_nbproject_configuration, 'MplabxNbprojectConfiguration')
    env['BUILDERS']['MplabxNbproject'] = _mplabx_nbproject_builder

def exists(env):
    return 1