#pragma once

#include <stdbool.h>
#include <wchar.h>

#include <pixman.h>

struct fcft_font {
    /* font extents */
    int height;
    int descent;
    int ascent;

    /* Width/height of font's widest glyph */
    struct {
        int x;
        int y;
    } max_advance;

    /* Width/height of space (0x20), if available, -1 otherwise */
    struct {
        int x;
        int y;
    } space_advance;

    struct {
        int position;
        int thickness;
    } underline;

    struct {
        int position;
        int thickness;
    } strikeout;
};

/* Bitmask of optional capabilities */
enum fcft_capabilities {
    FCFT_CAPABILITY_GRAPHEME_SHAPING = 0x1,
};

FCFT_EXPORT enum fcft_capabilities fcft_capabilities(void);

/* First entry is the main/primary font, the remaining (if any) are
 * custom fallback fonts */
FCFT_EXPORT struct fcft_font *fcft_from_name(
    size_t count, const char *names[static count], const char *attributes);
FCFT_EXPORT struct fcft_font *fcft_clone(const struct fcft_font *font);
FCFT_EXPORT void fcft_destroy(struct fcft_font *font);

/* Returns a *new* font instance */
FCFT_EXPORT struct fcft_font *fcft_size_adjust(const struct fcft_font *font, double amount) __attribute__((deprecated));

/*
 * Defines the subpixel order to use.
 *
 * Note that this is *ignored* if antialiasing has been disabled.
 */
enum fcft_subpixel {
    FCFT_SUBPIXEL_DEFAULT,          /* Use subpixel order from FontConfig */
    FCFT_SUBPIXEL_NONE,             /* Disable subpixel antialiasing (use grayscale antialiasing) */
    FCFT_SUBPIXEL_HORIZONTAL_RGB,
    FCFT_SUBPIXEL_HORIZONTAL_BGR,
    FCFT_SUBPIXEL_VERTICAL_RGB,
    FCFT_SUBPIXEL_VERTICAL_BGR,
};

struct fcft_glyph {
    wchar_t wc;
    int cols;              /* wcwidth(wc) */

    pixman_image_t *pix;

    int x;
    int y;
    int width;
    int height;

    struct {
        int x;
        int y;
    } advance;
};

/* Rasterize 'wc' using 'font'. Use the defined subpixel mode *if*
 * antialiasing is enabled for this font */
FCFT_EXPORT const struct fcft_glyph *fcft_glyph_rasterize(
    struct fcft_font *font, wchar_t wc, enum fcft_subpixel subpixel);

struct fcft_grapheme {
    int cols;  /* wcswidth(grapheme) */

    size_t count;
    const struct fcft_glyph **glyphs;
};

struct fcft_layout_tag {
    char tag[4];
    unsigned value;
};

FCFT_EXPORT const struct fcft_grapheme *fcft_grapheme_rasterize(
    struct fcft_font *font,
    size_t len, const wchar_t grapheme_cluster[static len],
    size_t tag_count, const struct fcft_layout_tag *tags,
    enum fcft_subpixel subpixel);

FCFT_EXPORT bool fcft_kerning(
    struct fcft_font *font, wchar_t left, wchar_t right,
    long *restrict x, long *restrict y);

FCFT_EXPORT wchar_t fcft_precompose(const struct fcft_font *font,
                                    wchar_t base, wchar_t comb,
                                    bool *base_is_from_primary,
                                    bool *comb_is_from_primary,
                                    bool *composed_is_from_primary);

enum fcft_scaling_filter {
    FCFT_SCALING_FILTER_NONE,
    FCFT_SCALING_FILTER_NEAREST,
    FCFT_SCALING_FILTER_BILINEAR,
    FCFT_SCALING_FILTER_CUBIC,
    FCFT_SCALING_FILTER_LANCZOS3,
};

FCFT_EXPORT bool fcft_set_scaling_filter(enum fcft_scaling_filter filter);
