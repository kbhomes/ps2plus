from dataclasses import dataclass, field

@dataclass
class VSCodeProperties:
    c_standard: str
    intellisense_mode: str
    additional_defines: list[str] = field(default_factory=list)
    additional_includes: list[str] = field(default_factory=list)