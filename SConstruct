# Build utilities specific to this project, in the `site_scons` folder
import firmware.platforms
import firmware.targets
import versions
import os
from pprint import pprint

env = Environment(tools=['vscode_c_cpp_properties'])
vscode_configurations = []

# Read PS2+ versions from the `ps2plus.json` manifest (with command-line overrides)
# and add them to the C compilation environment
ps2plus_versions = versions.setup_env(env)
pprint(ps2plus_versions)

for platform in firmware.platforms.ALL_PLATFORMS:
    firmware_outputs = {}
    firmware_sources = set()

    # Create a construction environment for this platform
    platform_env = env.Clone(tools=(env['TOOLS'] + platform.toolchain.tools))
    platform.toolchain.setup_env(platform_env)

    for target in firmware.targets.ALL_TARGETS:
        # Create a construction environment for this target
        build_env = platform_env.Clone()
        platform.setup_env_for_target(build_env, target)
        
        # Build the `firmware` sub-project, passing in platform and target information
        # (e.g.: `PIC18F46K42`` platform and `bootloader` target). Sub-project will build
        # its targets into the `build/{PLATFORM}/{TARGET}` folder.
        output, sources, vscode_configuration = SConscript('firmware/SConscript', 
            variant_dir=os.path.join('build', platform.name, target), 
            duplicate=False, 
            exports=['build_env', 'platform', 'target']
        )

        firmware_outputs[target] = output
        firmware_sources |= set([os.path.join('firmware', path) for path in sources])
        vscode_configurations.append(vscode_configuration)

    # Move each target's output file to the `dist` folder
    for target, output in firmware_outputs.items():
        InstallAs(f'dist/ps2plus-{platform.name}-{target}-{getattr(ps2plus_versions, target)}.hex', output)

    # Generate the IDE project(s), if any, for this platform
    platform.generate_ide_project(platform_env, f'firmware/build-projects/{platform.name}', firmware_sources)

# Generate VS Code autocompletion
env.AlwaysBuild(env.VSCodeCCppProperties('.vscode/c_cpp_properties.json', vscode_configurations))