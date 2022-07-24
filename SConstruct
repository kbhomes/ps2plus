# Build utilities specific to this project, in the `site_scons` folder
import firmware.platforms
import firmware.targets
import versions
import os
import subprocess
import sys
from pprint import pprint

# Type signature for platform/target/executable information for generated test runners
TypeTestExecutableInformation = tuple[firmware.platforms.AbstractFirmwarePlatform, firmware.targets.Target, object]

env = Environment(tools=['vscode_c_cpp_properties'])
vscode_configurations = []
test_targets: list[TypeTestExecutableInformation] = []

# Read PS2+ versions from the `ps2plus.json` manifest (with command-line overrides)
# and add them to the C compilation environment
ps2plus_versions = versions.setup_env(env)
pprint(ps2plus_versions)

# Allow a `--platform=PLATFORM` option to specify a single platform to build
AddOption(f'--platform',
    dest=f'platform',
    nargs=1,
    type='str',
    action='store',
    metavar='PLATFORM',
    default=None)

# Allow a `--test` flag to invoke tests on all platforms
AddOption(f'--test',
    dest=f'test',
    action='store_true',
    default=False)

# Build the configurator application (by calling `make`)
if 'dist/configurator' in BUILD_TARGETS:
    env.AlwaysBuild(env.Command(
        target='dist/configurator', 
        source='configurator', 
        action=f'make -C configurator ROOTDIR=`pwd` PROJECTDIR="$SOURCE" DISTDIR="$TARGET" VERSION="{ps2plus_versions.configurator}"',
        ENV=os.environ,
    ))

# Build the firmware/bootloader for each platform
if 'dist/firmware' in BUILD_TARGETS:
    specified_platform = GetOption('platform')

    for platform in firmware.platforms.ALL_PLATFORMS:
        if specified_platform and platform.name != specified_platform:
            continue
        
        firmware_outputs: dict[firmware.targets.Target, object] = {}
        firmware_dists: dict[firmware.targets.Target, object] = {}
        firmware_sources: set[str] = set()

        # Create a construction environment for this platform
        platform_env = env.Clone(tools=(env['TOOLS'] + platform.toolchain.tools))
        platform.toolchain.setup_env(platform_env)

        for target in firmware.targets.ALL_TARGETS:
            # Only build this target if the platform supports it
            if target not in platform.valid_targets:
                continue

            # Create a construction environment for this target
            build_env = platform_env.Clone()
            platform.setup_env_for_target(build_env, target)
            
            # Build the `firmware` sub-project, passing in platform and target information
            # (e.g.: `PIC18F46K42`` platform and `bootloader` target). Sub-project will build
            # its targets into the `build/{PLATFORM}/{TARGET}` folder.
            output, sources, vscode_configuration = SConscript('firmware/SConscript', 
                variant_dir=os.path.join('build', platform.name, target.name), 
                duplicate=False, 
                exports=['build_env', 'platform', 'target']
            )

            firmware_outputs[target] = output
            firmware_sources |= set([os.path.join('firmware', path) for path in sources])
            vscode_configurations.append(vscode_configuration)

        # Move each target's output file to the `dist` folder
        for target, output in firmware_outputs.items():
            # Move non-distributable targets to another location
            if not target.is_distributable or target.is_test:
                dist_output = env.Install(target=f'dist/firmware/misc-{platform.name}/', source=output)
                
                # Track all generated test targets
                if target.is_test:
                    test_targets.append((platform, target, dist_output))

                continue

            firmware_dists[target] = InstallAs(
                target=f'dist/firmware/ps2plus-{platform.name}-{target.name}-{getattr(ps2plus_versions, target.name)}.hex', 
                source=output
            )

        # Gather all target files to distribute
        # TODO: Make each platform be responsible for how it merges target distributibles, if they want to
        if len(firmware_dists):
            # Merge the different targets into a single combined image
            combined_version = '-'.join([f'{target.name}-{getattr(ps2plus_versions, target.name)}' for target in firmware_dists.keys()])
            combined_target = platform_env.Command(
                target=f'dist/firmware/ps2plus-{platform.name}-combined-{combined_version}.hex',
                source=firmware_dists.values(),
                action='python3 tools/hex-tools/merge.py --overlap=ignore $SOURCES -o $TARGET'
            )

            # ZIP up all platform files
            Zip(
                target=f'dist/firmware/ps2plus-{platform.name}-{combined_version}.zip',
                source=(list(firmware_dists.values()) + [combined_target]),
                ZIPROOT='dist/firmware',
            )

        # Generate the IDE project(s), if any, for this platform
        platform.generate_ide_project(platform_env, f'firmware/build-projects/{platform.name}', firmware_sources)

# If the `test` flag was passed, run any generated test targets
if GetOption('test'):
    for platform, test_target, output in test_targets:
        # Runs executable nodes, exiting the build if the executable fails
        def run_test_action(target, source, env):
            for executable in env.Flatten([target]):
                source_paths = env.Flatten([source])

                # Quick diagnostic check -- we expect a single source element
                if len(source_paths) > 1:
                    print(f'Warning: expected a single built test binary for platform "{platform.name}" and target "{test_target.name}, ' + 
                          f'but received "{list(map(str, source_paths))}"; executing the first test binary')

                build_folder = os.path.dirname(str(source_paths[0]))
                ret = platform.execute_test_for_target(env, test_target, build_folder, executable)
                if ret:
                    env.Exit(ret)

        test_action = env.Action(run_test_action, f'Running test: platform "{platform.name}" for target "{test_target.name}"\n')
        post_action = env.AddPostAction(output, test_action)
        env.AlwaysBuild(post_action)

# Generate VS Code autocompletion
env.AlwaysBuild(env.VSCodeCCppProperties('.vscode/c_cpp_properties.json', vscode_configurations))