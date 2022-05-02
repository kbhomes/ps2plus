**This font was originally created by Marcus Nichols: http://marcusnichols.com/subdomains/playstation/**

---

### Overview

The TTF font included in this project is a modified version of the PlayStation Icon Pack that includes eight new glyphs:
  - Start/Select (outline)
  - Start/Select (filled)
  - Start/Select with text label (outline)
  - Start/Select with text label (filled)

These modifications were created by designing the new glyphs in Inkscape, loading the original TTF in FontForge, and appending
these new glyphs to the end of the original range. The SVG files for the new glyphs can be found in `./svg/`, and the modified
TTF font in `./font/`.

To utilize this icon font in the ImGui UI, it was included in the configurator's resource folder (`configurator/ee/resources/fonts`)
and compiled as an object file as part of the configurator's resource building system (see `configurator/ee/Makefile`). The header file, 
`configurator/ee/ui/fonts/playstation.h`, contains `const char *` literal definitions for all of the glyphs within.

---

### License

The original font and these modifications are licensed under the [SIL Open Font License](https://scripts.sil.org/cms/scripts/page.php?item_id=OFL_web)
and [CC0 License](https://creativecommons.org/publicdomain/zero/1.0/), which can be found in this directory.