from enum import Enum
from dataclasses import dataclass, field

@dataclass
class Target:
    name: str
    defines: list[str] = field(default_factory=list)
    is_distributable: bool = True
    is_test: bool = False

    def __hash__(self) -> int:
        return hash(self.name)

    def __eq__(self, other: object) -> bool:
        if isinstance(other, Target):
            return self.name == other.name
        return False

FirmwareTarget = Target(
    name='firmware',
    defines=['PS2PLUS_FIRMWARE'])

BootloaderTarget = Target(
    name='bootloader',
    defines=['PS2PLUS_BOOTLOADER'])

TestFirmwareTarget = Target(
    name='test-' + FirmwareTarget.name,
    defines=FirmwareTarget.defines + ['PS2PLUS_TEST'],
    is_distributable=False,
    is_test=True)

TestBootloaderTarget = Target(
    name='test-' + BootloaderTarget.name,
    defines=BootloaderTarget.defines + ['PS2PLUS_TEST'],
    is_distributable=False,
    is_test=True)

ALL_TARGETS: list[Target] = [
    FirmwareTarget,
    BootloaderTarget,
    TestFirmwareTarget,
    # TestBootloaderTarget,
]