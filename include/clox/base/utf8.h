#pragma once

/**
 * @file        utf8.h
 *
 * @author      Federico Cristina <federico.cristina@outlook.it>
 *
 * @copyright   Copyright (c) 2024 Federico Cristina
 *
 *              This file is part of the clox programming language project,
 *              under the MIT License. See repo's LICENSE file for license
 *              informations.
 *
 *              Copyright (c) 2014-2021 by Steven G. Johnson, Jiahao Chen,
 *              Tony Kelman, Jonas Fonseca, and other contributors listed
 *              in the git history.
 *
 *              This file (like its .c implementation) is partially derived
 *              from utf8proc library:
 *
 *              https://github.com/JuliaStrings/utf8proc.git
 *
 * @version     1.1
 *
 * @brief       In this header are defined macros, constants and functions
 *              to deal with Unicode characters: Unicode normalization,
 *              case-folding, and other operations for strings in the UTF-8
 *              encoding, supporting up-to-date Unicode versions.
 */

#ifndef CLOX_BASE_UTF8_H_
#define CLOX_BASE_UTF8_H_

#include "clox/base/bool.h"
#include "clox/base/bits.h"

CLOX_C_HEADER_BEGIN

/**
 * Option flags used by several functions in the library.
 */
typedef enum _utf8_option
{
    /** The given UTF-8 input is NULL terminated. */
    UTF8_NULLTERM = (1 << 0),
    /** Unicode Versioning Stability has to be respected. */
    UTF8_STABLE = (1 << 1),
    /** Compatibility decomposition (i.e. formatting information is lost). */
    UTF8_COMPAT = (1 << 2),
    /** Return a result with decomposed characters. */
    UTF8_COMPOSE = (1 << 3),
    /** Return a result with decomposed characters. */
    UTF8_DECOMPOSE = (1 << 4),
    /** Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE. */
    UTF8_IGNORE = (1 << 5),
    /** Return an error, if the input contains unassigned codepoints. */
    UTF8_REJECTNA = (1 << 6),
    /**
     * Indicating that NLF-sequences (LF, CRLF, CR, NEL) are representing a
     * line break, and should be converted to the codepoint for line
     * separation (LS).
     */
    UTF8_NLF2LS = (1 << 7),
    /**
     * Indicating that NLF-sequences are representing a paragraph break, and
     * should be converted to the codepoint for paragraph separation
     * (PS).
     */
    UTF8_NLF2PS = (1 << 8),
    /** Indicating that the meaning of NLF-sequences is unknown. */
    UTF8_NLF2LF = (UTF8_NLF2LS | UTF8_NLF2PS),
    /** Strips and/or convers control characters.
     *
     * NLF-sequences are transformed into space, except if one of the
     * NLF2LS/PS/LF options is given. HorizontalTab (HT) and FormFeed (FF)
     * are treated as a NLF-sequence in this case.  All other control
     * characters are simply removed.
     */
    UTF8_STRIPCC = (1 << 9),
    /**
     * Performs unicode case folding, to be able to do a case-insensitive
     * string comparison.
     */
    UTF8_CASEFOLD = (1 << 10),
    /**
     * Inserts 0xFF bytes at the beginning of each sequence which is
     * representing a single grapheme cluster (see UAX#29).
     */
    UTF8_CHARBOUND = (1 << 11),
    /** Lumps certain characters together.
     *
     * E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-". See lump.md for details.
     *
     * If NLF2LF is set, this includes a transformation of paragraph and
     * line separators to ASCII line-feed (LF).
     */
    UTF8_LUMP = (1 << 12),
    /** Strips all character markings.
     *
     * This includes non-spacing, spacing and enclosing (i.e. accents).
     * @note This option works only with @ref UTF8_COMPOSE or
     *       @ref UTF8_DECOMPOSE
     */
    UTF8_STRIPMARK = (1 << 13),
    /**
     * Strip unassigned codepoints.
     */
    UTF8_STRIPNA = (1 << 14),
} utf8_option_t;

/** @name Error codes
 * Error codes being returned by almost all functions.
 */
/** @{ */
/** Memory could not be allocated. */
#define UTF8_ERROR_NOMEM -1
/** The given string is too long to be processed. */
#define UTF8_ERROR_OVERFLOW -2
/** The given string is not a legal UTF-8 string. */
#define UTF8_ERROR_INVALIDUTF8 -3
/** The @ref UTF8_REJECTNA flag was set and an unassigned codepoint was found. */
#define UTF8_ERROR_NOTASSIGNED -4
/** Invalid options have been used. */
#define UTF8_ERROR_INVALIDOPTS -5
/** @} */

/* @name Types */

/** Struct containing information about a codepoint. */
typedef struct _utf8_property_struct
{
    /**
     * Unicode category.
     * @see utf8_category_t.
     */
    uint16_t category;
    uint16_t combining_class;
    /**
     * Bidirectional class.
     * @see utf8_bidi_class_t.
     */
    uint16_t bidi_class;
    /**
     * @anchor Decomposition type.
     * @see utf8_decomp_type_t.
     */
    uint16_t decomp_type;
    uint16_t decomp_seqindex;
    uint16_t casefold_seqindex;
    uint16_t uppercase_seqindex;
    uint16_t lowercase_seqindex;
    uint16_t titlecase_seqindex;
    uint16_t comb_index;
    uint32_t bidi_mirrored : 1;
    uint32_t comp_exclusion : 1;
    /**
     * Can this codepoint be ignored?
     *
     * Used by utf8_decompose_char() when @ref UTF8_IGNORE is
     * passed as an option.
     */
    uint32_t ignorable : 1;
    uint32_t control_boundary : 1;
    /** The width of the codepoint. */
    uint32_t charwidth : 2;
    /** East Asian width class A */
    uint32_t ambiguous_width : 1;
    uint32_t pad : 1;
    /**
     * Boundclass.
     * @see utf8_boundclass_t.
     */
    uint32_t boundclass : 6;
    uint32_t indic_conjunct_break : 2;
} utf8_property_t;

/** Unicode categories. */
typedef enum
{
    UTF8_CATEGORY_CN = 0,  /**< Other, not assigned */
    UTF8_CATEGORY_LU = 1,  /**< Letter, uppercase */
    UTF8_CATEGORY_LL = 2,  /**< Letter, lowercase */
    UTF8_CATEGORY_LT = 3,  /**< Letter, titlecase */
    UTF8_CATEGORY_LM = 4,  /**< Letter, modifier */
    UTF8_CATEGORY_LO = 5,  /**< Letter, other */
    UTF8_CATEGORY_MN = 6,  /**< Mark, nonspacing */
    UTF8_CATEGORY_MC = 7,  /**< Mark, spacing combining */
    UTF8_CATEGORY_ME = 8,  /**< Mark, enclosing */
    UTF8_CATEGORY_ND = 9,  /**< Number, decimal digit */
    UTF8_CATEGORY_NL = 10, /**< Number, letter */
    UTF8_CATEGORY_NO = 11, /**< Number, other */
    UTF8_CATEGORY_PC = 12, /**< Punctuation, connector */
    UTF8_CATEGORY_PD = 13, /**< Punctuation, dash */
    UTF8_CATEGORY_PS = 14, /**< Punctuation, open */
    UTF8_CATEGORY_PE = 15, /**< Punctuation, close */
    UTF8_CATEGORY_PI = 16, /**< Punctuation, initial quote */
    UTF8_CATEGORY_PF = 17, /**< Punctuation, final quote */
    UTF8_CATEGORY_PO = 18, /**< Punctuation, other */
    UTF8_CATEGORY_SM = 19, /**< Symbol, math */
    UTF8_CATEGORY_SC = 20, /**< Symbol, currency */
    UTF8_CATEGORY_SK = 21, /**< Symbol, modifier */
    UTF8_CATEGORY_SO = 22, /**< Symbol, other */
    UTF8_CATEGORY_ZS = 23, /**< Separator, space */
    UTF8_CATEGORY_ZL = 24, /**< Separator, line */
    UTF8_CATEGORY_ZP = 25, /**< Separator, paragraph */
    UTF8_CATEGORY_CC = 26, /**< Other, control */
    UTF8_CATEGORY_CF = 27, /**< Other, format */
    UTF8_CATEGORY_CS = 28, /**< Other, surrogate */
    UTF8_CATEGORY_CO = 29, /**< Other, private use */
} utf8_category_t;

/** Bidirectional character classes. */
typedef enum
{
    UTF8_BIDI_CLASS_L = 1,    /**< Left-to-Right */
    UTF8_BIDI_CLASS_LRE = 2,  /**< Left-to-Right Embedding */
    UTF8_BIDI_CLASS_LRO = 3,  /**< Left-to-Right Override */
    UTF8_BIDI_CLASS_R = 4,    /**< Right-to-Left */
    UTF8_BIDI_CLASS_AL = 5,   /**< Right-to-Left Arabic */
    UTF8_BIDI_CLASS_RLE = 6,  /**< Right-to-Left Embedding */
    UTF8_BIDI_CLASS_RLO = 7,  /**< Right-to-Left Override */
    UTF8_BIDI_CLASS_PDF = 8,  /**< Pop Directional Format */
    UTF8_BIDI_CLASS_EN = 9,   /**< European Number */
    UTF8_BIDI_CLASS_ES = 10,  /**< European Separator */
    UTF8_BIDI_CLASS_ET = 11,  /**< European Number Terminator */
    UTF8_BIDI_CLASS_AN = 12,  /**< Arabic Number */
    UTF8_BIDI_CLASS_CS = 13,  /**< Common Number Separator */
    UTF8_BIDI_CLASS_NSM = 14, /**< Nonspacing Mark */
    UTF8_BIDI_CLASS_BN = 15,  /**< Boundary Neutral */
    UTF8_BIDI_CLASS_B = 16,   /**< Paragraph Separator */
    UTF8_BIDI_CLASS_S = 17,   /**< Segment Separator */
    UTF8_BIDI_CLASS_WS = 18,  /**< Whitespace */
    UTF8_BIDI_CLASS_ON = 19,  /**< Other Neutrals */
    UTF8_BIDI_CLASS_LRI = 20, /**< Left-to-Right Isolate */
    UTF8_BIDI_CLASS_RLI = 21, /**< Right-to-Left Isolate */
    UTF8_BIDI_CLASS_FSI = 22, /**< First Strong Isolate */
    UTF8_BIDI_CLASS_PDI = 23, /**< Pop Directional Isolate */
} utf8_bidi_class_t;

/** Decomposition type. */
typedef enum _utf8_decomp_type
{
    UTF8_DECOMP_TYPE_FONT = 1,      /**< Font */
    UTF8_DECOMP_TYPE_NOBREAK = 2,   /**< Nobreak */
    UTF8_DECOMP_TYPE_INITIAL = 3,   /**< Initial */
    UTF8_DECOMP_TYPE_MEDIAL = 4,    /**< Medial */
    UTF8_DECOMP_TYPE_FINAL = 5,     /**< Final */
    UTF8_DECOMP_TYPE_ISOLATED = 6,  /**< Isolated */
    UTF8_DECOMP_TYPE_CIRCLE = 7,    /**< Circle */
    UTF8_DECOMP_TYPE_SUPER = 8,     /**< Super */
    UTF8_DECOMP_TYPE_SUB = 9,       /**< Sub */
    UTF8_DECOMP_TYPE_VERTICAL = 10, /**< Vertical */
    UTF8_DECOMP_TYPE_WIDE = 11,     /**< Wide */
    UTF8_DECOMP_TYPE_NARROW = 12,   /**< Narrow */
    UTF8_DECOMP_TYPE_SMALL = 13,    /**< Small */
    UTF8_DECOMP_TYPE_SQUARE = 14,   /**< Square */
    UTF8_DECOMP_TYPE_FRACTION = 15, /**< Fraction */
    UTF8_DECOMP_TYPE_COMPAT = 16,   /**< Compat */
} utf8_decomp_type_t;

/** Boundclass property. (TR29) */
typedef enum _utf8_boundclass
{
    UTF8_BOUNDCLASS_START = 0,               /**< Start */
    UTF8_BOUNDCLASS_OTHER = 1,               /**< Other */
    UTF8_BOUNDCLASS_CR = 2,                  /**< Cr */
    UTF8_BOUNDCLASS_LF = 3,                  /**< Lf */
    UTF8_BOUNDCLASS_CONTROL = 4,             /**< Control */
    UTF8_BOUNDCLASS_EXTEND = 5,              /**< Extend */
    UTF8_BOUNDCLASS_L = 6,                   /**< L */
    UTF8_BOUNDCLASS_V = 7,                   /**< V */
    UTF8_BOUNDCLASS_T = 8,                   /**< T */
    UTF8_BOUNDCLASS_LV = 9,                  /**< Lv */
    UTF8_BOUNDCLASS_LVT = 10,                /**< Lvt */
    UTF8_BOUNDCLASS_REGIONAL_INDICATOR = 11, /**< Regional indicator */
    UTF8_BOUNDCLASS_SPACINGMARK = 12,        /**< Spacingmark */
    UTF8_BOUNDCLASS_PREPEND = 13,            /**< Prepend */
    UTF8_BOUNDCLASS_ZWJ = 14,                /**< Zero Width Joiner */

    /* the following are no longer used in Unicode 11, but we keep
       the constants here for backward compatibility */
    UTF8_BOUNDCLASS_E_BASE = 15,         /**< Emoji Base */
    UTF8_BOUNDCLASS_E_MODIFIER = 16,     /**< Emoji Modifier */
    UTF8_BOUNDCLASS_GLUE_AFTER_ZWJ = 17, /**< Glue_After_ZWJ */
    UTF8_BOUNDCLASS_E_BASE_GAZ = 18,     /**< E_BASE + GLUE_AFTER_ZJW */

    /* the Extended_Pictographic property is used in the Unicode 11
       grapheme-boundary rules, so we store it in the boundclass field */
    UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC = 19,
    UTF8_BOUNDCLASS_E_ZWG = 20, /* UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC + ZWJ */
} utf8_boundclass_t;

/** Indic_Conjunct_Break property. (TR44) */
typedef enum _utf8_indic_conjunct_break
{
    UTF8_INDIC_CONJUNCT_BREAK_NONE = 0,
    UTF8_INDIC_CONJUNCT_BREAK_LINKER = 1,
    UTF8_INDIC_CONJUNCT_BREAK_CONSONANT = 2,
    UTF8_INDIC_CONJUNCT_BREAK_EXTEND = 3,
} utf8_indic_conjunct_break_t;

/**
 * Function pointer type passed to utf8_map_custom() and
 * utf8_decompose_custom(), which is used to specify a user-defined
 * mapping of codepoints to be applied in conjunction with other mappings.
 */
typedef int32_t (*utf8_custom_func_t)(int32_t codepoint, void *data);

/**
 * Array containing the byte lengths of a UTF-8 encoded codepoint based
 * on the first byte.
 */
CLOX_EXTERN const int8_t CLOX_STDCALL utf8_utf8class[256];

/**
 * Returns the utf8proc API version as a string MAJOR.MINOR.PATCH
 * (http://semver.org format), possibly with a "-dev" suffix for
 * development versions.
 */
CLOX_EXTERN const char *utf8_version(void);

/**
 * Returns the utf8proc supported Unicode version as a string MAJOR.MINOR.PATCH.
 */
CLOX_EXTERN const char *utf8_unicode_version(void);

/**
 * Returns an informative error string for the given utf8proc error code
 * (e.g. the error codes returned by utf8_map()).
 */
CLOX_EXTERN const char *utf8_errmsg(ssize_t errcode);

/**
 * Reads a single codepoint from the UTF-8 sequence being pointed to by `str`.
 * The maximum number of bytes read is `strlen`, unless `strlen` is
 * negative (in which case up to 4 bytes are read).
 *
 * If a valid codepoint could be read, it is stored in the variable
 * pointed to by `codepoint_ref`, otherwise that variable will be set to -1.
 * In case of success, the number of bytes read is returned; otherwise, a
 * negative error code is returned.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_iterate(const uint8_t *str, ssize_t strlen, int32_t *codepoint_ref);

/**
 * Check if a codepoint is valid (regardless of whether it has been
 * assigned a value by the current Unicode standard).
 *
 * @return 1 if the given `codepoint` is valid and otherwise return 0.
 */
CLOX_EXTERN bool_t CLOX_STDCALL utf8_codepoint_valid(int32_t codepoint);

/**
 * Encodes the codepoint as an UTF-8 string in the byte array pointed
 * to by `dst`. This array must be at least 4 bytes long.
 *
 * In case of success the number of bytes written is returned, and
 * otherwise 0 is returned.
 *
 * This function does not check whether `codepoint` is valid Unicode.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_encode_char(int32_t codepoint, uint8_t *dst);

/**
 * Look up the properties for a given codepoint.
 *
 * @param codepoint The Unicode codepoint.
 *
 * @returns
 * A pointer to a (constant) struct containing information about
 * the codepoint.
 * @par
 * If the codepoint is unassigned or invalid, a pointer to a special struct is
 * returned in which `category` is 0 (@ref UTF8_CATEGORY_CN).
 */
CLOX_EXTERN const utf8_property_t *CLOX_STDCALL utf8_get_property(int32_t codepoint);

/** Decompose a codepoint into an array of codepoints.
 *
 * @param codepoint the codepoint.
 * @param dst the destination buffer.
 * @param bufsize the size of the destination buffer.
 * @param options one or more of the following flags:
 * - @ref UTF8_REJECTNA  - return an error `codepoint` is unassigned
 * - @ref UTF8_IGNORE    - strip "default ignorable" codepoints
 * - @ref UTF8_CASEFOLD  - apply Unicode casefolding
 * - @ref UTF8_COMPAT    - replace certain codepoints with their
 *                             compatibility decomposition
 * - @ref UTF8_CHARBOUND - insert 0xFF bytes before each grapheme cluster
 * - @ref UTF8_LUMP      - lump certain different codepoints together
 * - @ref UTF8_STRIPMARK - remove all character marks
 * - @ref UTF8_STRIPNA   - remove unassigned codepoints
 * @param last_boundclass
 * Pointer to an integer variable containing
 * the previous codepoint's (boundclass + indic_conjunct_break << 1) if the @ref UTF8_CHARBOUND
 * option is used.  If the string is being processed in order, this can be initialized to 0 for
 * the beginning of the string, and is thereafter updated automatically.  Otherwise, this parameter is ignored.
 *
 * @return
 * In case of success, the number of codepoints written is returned; in case
 * of an error, a negative error code is returned (utf8_errmsg()).
 * @par
 * If the number of written codepoints would be bigger than `bufsize`, the
 * required buffer size is returned, while the buffer will be overwritten with
 * undefined data.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_decompose_char(
    int32_t codepoint, int32_t *dst, ssize_t bufsize,
    utf8_option_t options, int *last_boundclass);

/**
 * The same as utf8_decompose_char(), but acts on a whole UTF-8
 * string and orders the decomposed sequences correctly.
 *
 * If the @ref UTF8_NULLTERM flag in `options` is set, processing
 * will be stopped, when a NULL byte is encountered, otherwise `strlen`
 * bytes are processed.  The result (in the form of 32-bit unicode
 * codepoints) is written into the buffer being pointed to by
 * `buffer` (which must contain at least `bufsize` entries).  In case of
 * success, the number of codepoints written is returned; in case of an
 * error, a negative error code is returned (utf8_errmsg()).
 * See utf8_decompose_custom() to supply additional transformations.
 *
 * If the number of written codepoints would be bigger than `bufsize`, the
 * required buffer size is returned, while the buffer will be overwritten with
 * undefined data.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_decompose(
    const uint8_t *str, ssize_t strlen,
    int32_t *buffer, ssize_t bufsize, utf8_option_t options);

/**
 * The same as utf8_decompose(), but also takes a `custom_func` mapping function
 * that is called on each codepoint in `str` before any other transformations
 * (along with a `custom_data` pointer that is passed through to `custom_func`).
 * The `custom_func` argument is ignored if it is `NULL`.  See also utf8_map_custom().
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_decompose_custom(
    const uint8_t *str, ssize_t strlen,
    int32_t *buffer, ssize_t bufsize, utf8_option_t options,
    utf8_custom_func_t custom_func, void *custom_data);

/**
 * Normalizes the sequence of `length` codepoints pointed to by `buffer`
 * in-place (i.e., the result is also stored in `buffer`).
 *
 * @param buffer the (native-endian UTF-32) unicode codepoints to re-encode.
 * @param length the length (in codepoints) of the buffer.
 * @param options a bitwise or (`|`) of one or more of the following flags:
 * - @ref UTF8_NLF2LS  - convert LF, CRLF, CR and NEL into LS
 * - @ref UTF8_NLF2PS  - convert LF, CRLF, CR and NEL into PS
 * - @ref UTF8_NLF2LF  - convert LF, CRLF, CR and NEL into LF
 * - @ref UTF8_STRIPCC - strip or convert all non-affected control characters
 * - @ref UTF8_COMPOSE - try to combine decomposed codepoints into composite
 *                           codepoints
 * - @ref UTF8_STABLE  - prohibit combining characters that would violate
 *                           the unicode versioning stability
 *
 * @return
 * In case of success, the length (in codepoints) of the normalized UTF-32 string is
 * returned; otherwise, a negative error code is returned (utf8_errmsg()).
 *
 * @warning The entries of the array pointed to by `str` have to be in the
 *          range `0x0000` to `0x10FFFF`. Otherwise, the program might crash!
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_normalize_utf32(int32_t *buffer, ssize_t length, utf8_option_t options);

/**
 * Reencodes the sequence of `length` codepoints pointed to by `buffer`
 * UTF-8 data in-place (i.e., the result is also stored in `buffer`).
 * Can optionally normalize the UTF-32 sequence prior to UTF-8 conversion.
 *
 * @param buffer the (native-endian UTF-32) unicode codepoints to re-encode.
 * @param length the length (in codepoints) of the buffer.
 * @param options a bitwise or (`|`) of one or more of the following flags:
 * - @ref UTF8_NLF2LS  - convert LF, CRLF, CR and NEL into LS
 * - @ref UTF8_NLF2PS  - convert LF, CRLF, CR and NEL into PS
 * - @ref UTF8_NLF2LF  - convert LF, CRLF, CR and NEL into LF
 * - @ref UTF8_STRIPCC - strip or convert all non-affected control characters
 * - @ref UTF8_COMPOSE - try to combine decomposed codepoints into composite
 *                           codepoints
 * - @ref UTF8_STABLE  - prohibit combining characters that would violate
 *                           the unicode versioning stability
 * - @ref UTF8_CHARBOUND - insert 0xFF bytes before each grapheme cluster
 *
 * @return
 * In case of success, the length (in bytes) of the resulting nul-terminated
 * UTF-8 string is returned; otherwise, a negative error code is returned
 * (utf8_errmsg()).
 *
 * @warning The amount of free space pointed to by `buffer` must
 *          exceed the amount of the input data by one byte, and the
 *          entries of the array pointed to by `str` have to be in the
 *          range `0x0000` to `0x10FFFF`. Otherwise, the program might crash!
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_reencode(int32_t *buffer, ssize_t length, utf8_option_t options);

/**
 * Given a pair of consecutive codepoints, return whether a grapheme break is
 * permitted between them (as defined by the extended grapheme clusters in UAX#29).
 *
 * @param codepoint1 The first codepoint.
 * @param codepoint2 The second codepoint, occurring consecutively after `codepoint1`.
 * @param state Beginning with Version 29 (Unicode 9.0.0), this algorithm requires
 *              state to break graphemes. This state can be passed in as a pointer
 *              in the `state` argument and should initially be set to 0. If the
 *              state is not passed in (i.e. a null pointer is passed), UAX#29 rules
 *              GB10/12/13 which require this state will not be applied, essentially
 *              matching the rules in Unicode 8.0.0.
 *
 * @warning If the state parameter is used, `utf8_grapheme_break_stateful` must
 *          be called IN ORDER on ALL potential breaks in a string.  However, it
 *          is safe to reset the state to zero after a grapheme break.
 */
CLOX_EXTERN bool_t CLOX_STDCALL utf8_grapheme_break_stateful(
    int32_t codepoint1, int32_t codepoint2, int32_t *state);

/**
 * Same as utf8_grapheme_break_stateful(), except without support for the
 * Unicode 9 additions to the algorithm. Supported for legacy reasons.
 */
CLOX_EXTERN bool_t CLOX_STDCALL utf8_grapheme_break(
    int32_t codepoint1, int32_t codepoint2);

/**
 * Given a codepoint `c`, return the codepoint of the corresponding
 * lower-case character, if any; otherwise (if there is no lower-case
 * variant, or if `c` is not a valid codepoint) return `c`.
 */
CLOX_EXTERN int32_t CLOX_STDCALL utf8_tolower(int32_t c);

/**
 * Given a codepoint `c`, return the codepoint of the corresponding
 * upper-case character, if any; otherwise (if there is no upper-case
 * variant, or if `c` is not a valid codepoint) return `c`.
 */
CLOX_EXTERN int32_t CLOX_STDCALL utf8_toupper(int32_t c);

/**
 * Given a codepoint `c`, return the codepoint of the corresponding
 * title-case character, if any; otherwise (if there is no title-case
 * variant, or if `c` is not a valid codepoint) return `c`.
 */
CLOX_EXTERN int32_t CLOX_STDCALL utf8_totitle(int32_t c);

/**
 * Given a codepoint `c`, return `1` if the codepoint corresponds to a lower-case character
 * and `0` otherwise.
 */
CLOX_EXTERN int CLOX_STDCALL utf8_islower(int32_t c);

/**
 * Given a codepoint `c`, return `1` if the codepoint corresponds to an upper-case character
 * and `0` otherwise.
 */
CLOX_EXTERN int CLOX_STDCALL utf8_isupper(int32_t c);

/**
 * Given a codepoint, return a character width analogous to `wcwidth(codepoint)`,
 * except that a width of 0 is returned for non-printable codepoints
 * instead of -1 as in `wcwidth`.
 *
 * @note
 * If you want to check for particular types of non-printable characters,
 * (analogous to `isprint` or `iscntrl`), use utf8_category(). */
CLOX_EXTERN int CLOX_STDCALL utf8_charwidth(int32_t codepoint);

/**
 * Given a codepoint, return whether it has East Asian width class A (Ambiguous)
 *
 * Codepoints with this property are considered to have charwidth 1 (if they are printable)
 * but some East Asian fonts render them as double width.
 */
CLOX_EXTERN bool_t CLOX_STDCALL utf8_charwidth_ambiguous(int32_t codepoint);

/**
 * Return the Unicode category for the codepoint (one of the
 * @ref utf8_category_t constants.)
 */
CLOX_EXTERN utf8_category_t CLOX_STDCALL utf8_category(int32_t codepoint);

/**
 * Return the two-letter (nul-terminated) Unicode category string for
 * the codepoint (e.g. `"Lu"` or `"Co"`).
 */
CLOX_EXTERN const char *CLOX_STDCALL utf8_category_string(int32_t codepoint);

/**
 * Maps the given UTF-8 string pointed to by `str` to a new UTF-8
 * string, allocated dynamically by `malloc` and returned via `dstptr`.
 *
 * If the @ref UTF8_NULLTERM flag in the `options` field is set,
 * the length is determined by a NULL terminator, otherwise the
 * parameter `strlen` is evaluated to determine the string length, but
 * in any case the result will be NULL terminated (though it might
 * contain NULL characters with the string if `str` contained NULL
 * characters). Other flags in the `options` field are passed to the
 * functions defined above, and regarded as described.  See also
 * utf8_map_custom() to supply a custom codepoint transformation.
 *
 * In case of success the length of the new string is returned,
 * otherwise a negative error code is returned.
 *
 * @note The memory of the new UTF-8 string will have been allocated
 * with `malloc`, and should therefore be deallocated with `free`.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_map(
    const uint8_t *str, ssize_t strlen, uint8_t **dstptr, utf8_option_t options);

/**
 * Like utf8_map(), but also takes a `custom_func` mapping function
 * that is called on each codepoint in `str` before any other transformations
 * (along with a `custom_data` pointer that is passed through to `custom_func`).
 * The `custom_func` argument is ignored if it is `NULL`.
 */
CLOX_EXTERN ssize_t CLOX_STDCALL utf8_map_custom(
    const uint8_t *str, ssize_t strlen, uint8_t **dstptr, utf8_option_t options,
    utf8_custom_func_t custom_func, void *custom_data);

/** @name Unicode normalization
 *
 * Returns a pointer to newly allocated memory of a NFD, NFC, NFKD, NFKC or
 * NFKC_Casefold normalized version of the null-terminated string `str`.  These
 * are shortcuts to calling utf8_map() with @ref UTF8_NULLTERM
 * combined with @ref UTF8_STABLE and flags indicating the normalization.
 */
/** @{ */
/** NFD normalization (@ref UTF8_DECOMPOSE). */
CLOX_EXTERN uint8_t *CLOX_STDCALL utf8_NFD(const uint8_t *str);
/** NFC normalization (@ref UTF8_COMPOSE). */
CLOX_EXTERN uint8_t *CLOX_STDCALL utf8_NFC(const uint8_t *str);
/** NFKD normalization (@ref UTF8_DECOMPOSE and @ref UTF8_COMPAT). */
CLOX_EXTERN uint8_t *CLOX_STDCALL utf8_NFKD(const uint8_t *str);
/** NFKC normalization (@ref UTF8_COMPOSE and @ref UTF8_COMPAT). */
CLOX_EXTERN uint8_t *CLOX_STDCALL utf8_NFKC(const uint8_t *str);
/**
 * NFKC_Casefold normalization (@ref UTF8_COMPOSE and @ref UTF8_COMPAT
 * and @ref UTF8_CASEFOLD and @ref UTF8_IGNORE).
 **/
CLOX_EXTERN uint8_t *CLOX_STDCALL utf8_NFKC_Casefold(const uint8_t *str);
/** @} */

CLOX_C_HEADER_END

#endif /* CLOX_BASE_UTF8_H_ */
