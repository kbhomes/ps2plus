from importlib.metadata import metadata
from SCons.Script import *
from dataclasses import dataclass
from enum import Enum
import json
import os

class VersionMetadata(Enum):
    TYPE_NONE = 0
    TYPE_DEV = 1
    TYPE_LATEST = 2

    def __str__(self) -> str:
        if self == VersionMetadata.TYPE_NONE: return ''
        if self == VersionMetadata.TYPE_DEV: return 'dev'
        if self == VersionMetadata.TYPE_LATEST: return 'latest'

    def __repr__(self) -> str:
        return str(self)

@dataclass
class Version:
    major: int
    minor: int
    patch: int
    metadata: VersionMetadata = VersionMetadata.TYPE_DEV

    def pack(self) -> int:
        return ((self.major & 0xFF) << 56) | ((self.minor & 0xFFFF) << 40) | ((self.patch & 0xFFFFFFFF) << 8) | ((self.metadata.value) & 0xFF)

    def __str__(self) -> str:
        version = f'v{self.major}.{self.minor}.{self.patch}'
        if str(self.metadata):
            version += '-' + str(self.metadata)
        return version

    def __repr__(self) -> str:
        return str(self)

@dataclass
class PS2PlusVersions:
    firmware: Version
    bootloader: Version
    configurator: Version
    configuration: int

def setup_env(env):
    data = None
    with open('ps2plus.json') as f:
        manifest = json.load(f)
        data = manifest['version']

    for target, version in data.items():
        if isinstance(version, dict):
            for field, default in version.items():
                if field == 'patch':
                    # For patch version, use the number of commits since the last tag 
                    tag_name = os.popen(f'git describe --match "{target}-*" --abbrev=0').read().strip()
                    if tag_name:
                        default = int(os.popen(f'git rev-list {tag_name}.. --count').read().strip())

                AddOption(f'--version-{target}-{field}',
                        dest=f'version_{target}_{field}',
                        nargs=1,
                        type='int',
                        action='store',
                        metavar=str(default),
                        default=default)
        else:
            AddOption(f'--version-{target}',
                    dest=f'version_{target}',
                    nargs=1,
                    type='int',
                    action='store',
                    metavar=str(version),
                    default=version)
    
    versions = PS2PlusVersions(
        firmware=Version(
            major=GetOption('version_firmware_major'),
            minor=GetOption('version_firmware_minor'),
            patch=GetOption('version_firmware_patch'),
        ),
        bootloader=Version(
            major=GetOption('version_bootloader_major'),
            minor=GetOption('version_bootloader_minor'),
            patch=GetOption('version_bootloader_patch'),
        ),
        configurator=Version(
            major=GetOption('version_configurator_major'),
            minor=GetOption('version_configurator_minor'),
            patch=GetOption('version_configurator_patch'),
        ),
        configuration=GetOption('version_configuration'),
    )

    env.Append(CPPDEFINES=[
        f'VERSION_FIRMWARE={versions.firmware.pack()}ul',
        f'VERSION_BOOTLOADER={versions.bootloader.pack()}ul',
        f'VERSION_CONFIGURATOR={versions.configurator.pack()}ul',
        f'VERSION_CONFIGURATION={versions.configuration}',
    ])
    
    return versions