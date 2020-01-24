# fcft

**fcft** is a small font loading and glyph rasterization library built
on-top of FontConfig, FreeType2 and pixman.

It can load and cache fonts from a fontconfig-formatted name string,
e.g. `Monospace:size=12`, optionally with user configured fallback fonts.

After a font has been loaded, you can rasterize glyphs. When doing so,
the primary font is first considered. If it does not have the
requested glyph, the user configured fallback fonts (if any) are
considered. If none of the user configured fallback fonts has the
requested glyph, the FontConfig generated list of fallback fonts are
checked.


## Requirements

* fontconfig
* freetype
* pixman
* [tllist](https://codeberg.org/dnkl/tllist), _unless_ built as a subproject


## Features

* Supports all fonts loadable by FreeType2
* Antialiasing
* Subpixel antialiasing
* Color bitmap fonts (_emoji_ fonts)
* Font caching
* Glyph caching
* Kerning[^1]

[^1]: only basic kerning supported (i.e. the old 'kern' tables, not
    the new 'GPOS' tables)


## Not supported

* Subpixel positioning

Remember, this is a _simple_ library, not a full blown layout engine.


## Projects using fcft

* [foot](https://codeberg.org/dnkl/foot)
* [yambar](https://codeberg.org/dnkl/yambar)
* [fuzzel](https://codeberg.org/dnkl/fuzzel)
* [fnott](https://codeberg.org/dnkl/fnott)


## Integrating

You can either install fcft as a system library, or use it as a meson
subproject (assuming your project is meson based, of course).


### Installing

If you install fcft as a system library, you can use `pkg-config` to
get the compiler flags needed to find and link against fcft.


### Meson

If your project is meson based, you can use fcft as a subproject. In
your main project's `meson.build`, do something like:

```meson
fcft = subproject('fcft').get_variable('fcft')
executable('you-executable', ..., dependencies: [fcft])
```

Or, if fcft has been installed as a system library, a regular

```meson
fcft = dependency('fcft')
```

will suffice. Optionally, you can combine the two; search for a system
library first, and fallback to a subproject:

```meson
fcft = dependency('fcft', version: '>=0.4.0', fallback: ['fcft', 'fcft'])
```
