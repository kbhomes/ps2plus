from dataclasses import dataclass, field

@dataclass
class MPLABXProperties:
    toolchain: str
    toolchain_version: str
    device: str = None
    compiler_properties: dict[str, str] = field(default_factory=dict)
    linker_properties: dict[str, str] = field(default_factory=dict)
    xc8_properties: dict[str, str] = field(default_factory=dict)