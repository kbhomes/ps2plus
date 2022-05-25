# Build utilities specific to this project, in the `site_scons` folder
import firmware.platforms
import firmware.targets
import os

outputs = []

for platform in firmware.platforms.ALL_PLATFORMS:
    firmware_outputs = {}

    for target in firmware.targets.ALL_TARGETS:
        # Build the `firmware` sub-project, passing in platform and target information
        # (e.g.: `PIC18F46K42`` platform and `bootloader` target)
        firmware_outputs[target] = SConscript('firmware/SConscript', 
            variant_dir=os.path.join('build', platform.name, target), 
            duplicate=False, 
            exports=['platform', 'target']
        )

    for target, output in firmware_outputs.items():
        InstallAs(f'dist/ps2plus-{platform.name}-{target}.hex', output)

    for output in firmware_outputs.values():
        outputs.append(output)
