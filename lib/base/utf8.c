/**
 * This file is part of the clox programming language project,
 * under the MIT License. See repo's LICENSE file for license
 * informations.
 *
 * The content of this file is derived from the utf8proc
 * library:
 *
 * https:
 */

#include "clox/base/errno.h"
#include "clox/base/utf8.h"

const char *CLOX_STDCALL utf8_errmsg(ssize_t errcode)
{
    const char *message = NULL;

    switch (errcode)
    {
    case UTF8_ERROR_NOMEM:
        message = "Memory could not be allocated.";
        break;

    case UTF8_ERROR_OVERFLOW:
        message = "The given string is too long to be processed.";
        break;

    case UTF8_ERROR_INVALIDUTF8:
        message = "The given string is not a legal UTF-8 string.";
        break;

    case UTF8_ERROR_NOTASSIGNED:
        message = "The UTF8_REJECTNA flag was set and an unassigned codepoint was found.";
        break;

    case UTF8_ERROR_INVALIDOPTS:
        message = "Invalid options have been used.";
        break;

    default:
        return unreach();
    }

    return message;
}

#include "utf8.inc"

const int8_t utf8_utf8class[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0
};

#define UTF8_HANGUL_SBASE 0xAC00
#define UTF8_HANGUL_LBASE 0x1100
#define UTF8_HANGUL_VBASE 0x1161
#define UTF8_HANGUL_TBASE 0x11A7
#define UTF8_HANGUL_LCOUNT 19
#define UTF8_HANGUL_VCOUNT 21
#define UTF8_HANGUL_TCOUNT 28
#define UTF8_HANGUL_NCOUNT 588
#define UTF8_HANGUL_SCOUNT 11172
/* END is exclusive */
#define UTF8_HANGUL_L_START 0x1100
#define UTF8_HANGUL_L_END 0x115A
#define UTF8_HANGUL_L_FILLER 0x115F
#define UTF8_HANGUL_V_START 0x1160
#define UTF8_HANGUL_V_END 0x11A3
#define UTF8_HANGUL_T_START 0x11A8
#define UTF8_HANGUL_T_END 0x11FA
#define UTF8_HANGUL_S_START 0xAC00
#define UTF8_HANGUL_S_END 0xD7A4

/* Should follow semantic-versioning rules (semver.org) based on API
   compatibility.  (Note that the shared-library version number will
   be different, being based on ABI compatibility.): */
#define STRINGIZEx(x) #x
#define STRINGIZE(x) STRINGIZEx(x)
const char *CLOX_STDCALL utf8_version(void)
{
    return STRINGIZE(UTF8_VERSION_MAJOR) "." STRINGIZE(UTF8_VERSION_MINOR) "." STRINGIZE(UTF8_VERSION_PATCH) "";
}

const char *CLOX_STDCALL utf8_unicode_version(void)
{
    return "15.1.0";
}

#define utf_cont(ch) (((ch) & 0xc0) == 0x80)

ssize_t CLOX_STDCALL utf8_iterate(
    const uint8_t *str, ssize_t strlen, int32_t *dst)
{
    int32_t uc;
    const uint8_t *end;

    *dst = -1;
    if (!strlen)
        return 0;
    end = str + ((strlen < 0) ? 4 : strlen);
    uc = *str++;
    if (uc < 0x80)
    {
        *dst = uc;
        return 1;
    }
    if ((uint32_t)(uc - 0xc2) > (0xf4 - 0xc2))
        return UTF8_ERROR_INVALIDUTF8;
    if (uc < 0xe0)
    { 
        
        if (str >= end || !utf_cont(*str))
            return UTF8_ERROR_INVALIDUTF8;
        *dst = ((uc & 0x1f) << 6) | (*str & 0x3f);
        return 2;
    }
    if (uc < 0xf0)
    { 
        if ((str + 1 >= end) || !utf_cont(*str) || !utf_cont(str[1]))
            return UTF8_ERROR_INVALIDUTF8;
        
        if (uc == 0xed && *str > 0x9f)
            return UTF8_ERROR_INVALIDUTF8;
        uc = ((uc & 0xf) << 12) | ((*str & 0x3f) << 6) | (str[1] & 0x3f);
        if (uc < 0x800)
            return UTF8_ERROR_INVALIDUTF8;
        *dst = uc;
        return 3;
    }
    
    
    if ((str + 2 >= end) || !utf_cont(*str) || !utf_cont(str[1]) || !utf_cont(str[2]))
        return UTF8_ERROR_INVALIDUTF8;
    
    if (uc == 0xf0)
    {
        if (*str < 0x90)
            return UTF8_ERROR_INVALIDUTF8;
    }
    else if (uc == 0xf4)
    {
        if (*str > 0x8f)
            return UTF8_ERROR_INVALIDUTF8;
    }
    *dst = ((uc & 7) << 18) | ((*str & 0x3f) << 12) | ((str[1] & 0x3f) << 6) | (str[2] & 0x3f);
    return 4;
}

bool_t CLOX_STDCALL utf8_codepoint_valid(int32_t uc)
{
    return (((uint32_t)uc) - 0xd800 > 0x07ff) && ((uint32_t)uc < 0x110000);
}

ssize_t CLOX_STDCALL utf8_encode_char(int32_t uc, uint8_t *dst)
{
    if (uc < 0x00)
    {
        return 0;
    }
    else if (uc < 0x80)
    {
        dst[0] = (uint8_t)uc;
        return 1;
    }
    else if (uc < 0x800)
    {
        dst[0] = (uint8_t)(0xC0 + (uc >> 6));
        dst[1] = (uint8_t)(0x80 + (uc & 0x3F));
        return 2;
        
        
    }
    else if (uc < 0x10000)
    {
        dst[0] = (uint8_t)(0xE0 + (uc >> 12));
        dst[1] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
        dst[2] = (uint8_t)(0x80 + (uc & 0x3F));
        return 3;
    }
    else if (uc < 0x110000)
    {
        dst[0] = (uint8_t)(0xF0 + (uc >> 18));
        dst[1] = (uint8_t)(0x80 + ((uc >> 12) & 0x3F));
        dst[2] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
        dst[3] = (uint8_t)(0x80 + (uc & 0x3F));
        return 4;
    }
    else
        return 0;
}

/* internal version used for inserting 0xff bytes between graphemes */
CLOX_STATIC ssize_t CLOX_STDCALL charbound_encode_char(int32_t uc, uint8_t *dst)
{
    if (uc < 0x00)
    {
        if (uc == -1)
        { /* internal value used for grapheme breaks */
            dst[0] = (uint8_t)0xFF;
            return 1;
        }
        return 0;
    }
    else if (uc < 0x80)
    {
        dst[0] = (uint8_t)uc;
        return 1;
    }
    else if (uc < 0x800)
    {
        dst[0] = (uint8_t)(0xC0 + (uc >> 6));
        dst[1] = (uint8_t)(0x80 + (uc & 0x3F));
        return 2;
    }
    else if (uc < 0x10000)
    {
        dst[0] = (uint8_t)(0xE0 + (uc >> 12));
        dst[1] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
        dst[2] = (uint8_t)(0x80 + (uc & 0x3F));
        return 3;
    }
    else if (uc < 0x110000)
    {
        dst[0] = (uint8_t)(0xF0 + (uc >> 18));
        dst[1] = (uint8_t)(0x80 + ((uc >> 12) & 0x3F));
        dst[2] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
        dst[3] = (uint8_t)(0x80 + (uc & 0x3F));
        return 4;
    }
    else
        return 0;
}

/* internal "unsafe" version that does not check whether uc is in range */
CLOX_STATIC const utf8_property_t *CLOX_STDCALL unsafe_get_property(int32_t uc)
{
    /* ASSERT: uc >= 0 && uc < 0x110000 */
    return utf8_properties + (utf8_stage2table[utf8_stage1table[uc >> 8] + (uc & 0xFF)]);
}

const utf8_property_t *CLOX_STDCALL utf8_get_property(int32_t uc)
{
    return uc < 0 || uc >= 0x110000 ? utf8_properties : unsafe_get_property(uc);
}

/* return whether there is a grapheme break between boundclasses lbc and tbc
   (according to the definition of extended grapheme clusters)

  Rule numbering refers to TR29 Version 29 (Unicode 9.0.0):
  http:

  CAVEATS:
   Please note that evaluation of GB10 (grapheme breaks between emoji zwj sequences)
   and GB 12/13 (regional indicator code points) require knowledge of previous characters
   and are thus not handled by this function. This may result in an incorrect break before
   an E_Modifier class codepoint and an incorrectly missing break between two
   REGIONAL_INDICATOR class code points if such support does not exist in the caller.

   See the special support in grapheme_break_extended, for required bookkeeping by the caller.
*/
CLOX_STATIC bool_t CLOX_STDCALL grapheme_break_simple(int lbc, int tbc)
{
    return (lbc == UTF8_BOUNDCLASS_START) ? TRUE : 
               (lbc == UTF8_BOUNDCLASS_CR &&       
                tbc == UTF8_BOUNDCLASS_LF)
               ? FALSE
               : 
               (lbc >= UTF8_BOUNDCLASS_CR && lbc <= UTF8_BOUNDCLASS_CONTROL) ? TRUE
                                                                             : 
               (tbc >= UTF8_BOUNDCLASS_CR && tbc <= UTF8_BOUNDCLASS_CONTROL) ? TRUE
                                                                             : 
               (lbc == UTF8_BOUNDCLASS_L &&                                    
                (tbc == UTF8_BOUNDCLASS_L ||                                   
                 tbc == UTF8_BOUNDCLASS_V ||                                   
                 tbc == UTF8_BOUNDCLASS_LV ||                                  
                 tbc == UTF8_BOUNDCLASS_LVT))
               ? FALSE
               :                              
               ((lbc == UTF8_BOUNDCLASS_LV || 
                 lbc == UTF8_BOUNDCLASS_V) && 
                (tbc == UTF8_BOUNDCLASS_V ||  
                 tbc == UTF8_BOUNDCLASS_T))
               ? FALSE
               :                               
               ((lbc == UTF8_BOUNDCLASS_LVT || 
                 lbc == UTF8_BOUNDCLASS_T) &&  
                tbc == UTF8_BOUNDCLASS_T)
               ? FALSE
               :                                      
               (tbc == UTF8_BOUNDCLASS_EXTEND ||      
                tbc == UTF8_BOUNDCLASS_ZWJ ||         
                tbc == UTF8_BOUNDCLASS_SPACINGMARK || 
                lbc == UTF8_BOUNDCLASS_PREPEND)
               ? FALSE
               :                                
               (lbc == UTF8_BOUNDCLASS_E_ZWG && 
                tbc == UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC)
               ? FALSE
               :                                             
               (lbc == UTF8_BOUNDCLASS_REGIONAL_INDICATOR && 
                tbc == UTF8_BOUNDCLASS_REGIONAL_INDICATOR)
               ? FALSE
               :     
               TRUE; 
}

CLOX_STATIC bool_t CLOX_STDCALL grapheme_break_extended(int lbc, int tbc, int licb, int ticb, int32_t *state)
{
    if (state)
    {
        int state_bc, state_icb; /* boundclass and indic_conjunct_break state */
        if (*state == 0)
        { /* state initialization */
            state_bc = lbc;
            state_icb = licb == UTF8_INDIC_CONJUNCT_BREAK_CONSONANT ? licb : UTF8_INDIC_CONJUNCT_BREAK_NONE;
        }
        else
        {                             /* lbc and licb are already encoded in *state */
            state_bc = *state & 0xff; 
            state_icb = *state >> 8;  
        }

        bool_t break_permitted = grapheme_break_simple(state_bc, tbc) &&
                                 !(state_icb == UTF8_INDIC_CONJUNCT_BREAK_LINKER && ticb == UTF8_INDIC_CONJUNCT_BREAK_CONSONANT); 

        
        
        
        if (ticb == UTF8_INDIC_CONJUNCT_BREAK_CONSONANT || state_icb == UTF8_INDIC_CONJUNCT_BREAK_CONSONANT || state_icb == UTF8_INDIC_CONJUNCT_BREAK_EXTEND)
            state_icb = ticb;
        else if (state_icb == UTF8_INDIC_CONJUNCT_BREAK_LINKER)
            state_icb = ticb == UTF8_INDIC_CONJUNCT_BREAK_EXTEND ? UTF8_INDIC_CONJUNCT_BREAK_LINKER : ticb;

        
        
        
        
        
        if (state_bc == tbc && tbc == UTF8_BOUNDCLASS_REGIONAL_INDICATOR)
            state_bc = UTF8_BOUNDCLASS_OTHER;
        
        else if (state_bc == UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC)
        {
            if (tbc == UTF8_BOUNDCLASS_EXTEND) 
                state_bc = UTF8_BOUNDCLASS_EXTENDED_PICTOGRAPHIC;
            else if (tbc == UTF8_BOUNDCLASS_ZWJ)
                state_bc = UTF8_BOUNDCLASS_E_ZWG; 
            else
                state_bc = tbc;
        }
        else
            state_bc = tbc;

        *state = state_bc + (state_icb << 8);
        return break_permitted;
    }
    else
        return grapheme_break_simple(lbc, tbc);
}

bool_t CLOX_STDCALL utf8_grapheme_break_stateful(
    int32_t c1, int32_t c2, int32_t *state)
{

    const utf8_property_t *p1 = utf8_get_property(c1);
    const utf8_property_t *p2 = utf8_get_property(c2);
    return grapheme_break_extended(p1->boundclass,
                                   p2->boundclass,
                                   p1->indic_conjunct_break,
                                   p2->indic_conjunct_break,
                                   state);
}

bool_t CLOX_STDCALL utf8_grapheme_break(
    int32_t c1, int32_t c2)
{
    return utf8_grapheme_break_stateful(c1, c2, NULL);
}

CLOX_STATIC int32_t CLOX_STDCALL seqindex_decode_entry(const uint16_t **entry)
{
    int32_t entry_cp = **entry;
    if ((entry_cp & 0xF800) == 0xD800)
    {
        *entry = *entry + 1;
        entry_cp = ((entry_cp & 0x03FF) << 10) | (**entry & 0x03FF);
        entry_cp += 0x10000;
    }
    return entry_cp;
}

CLOX_STATIC int32_t CLOX_STDCALL seqindex_decode_index(const uint32_t seqindex)
{
    const uint16_t *entry = &utf8_sequences[seqindex];
    return seqindex_decode_entry(&entry);
}

CLOX_STATIC ssize_t CLOX_STDCALL seqindex_write_char_decomposed(uint16_t seqindex, int32_t *dst, ssize_t bufsize, utf8_option_t options, int *last_boundclass)
{
    ssize_t written = 0;
    const uint16_t *entry = &utf8_sequences[seqindex & 0x3FFF];
    int len = seqindex >> 14;
    if (len >= 3)
    {
        len = *entry;
        entry++;
    }
    for (; len >= 0; entry++, len--)
    {
        int32_t entry_cp = seqindex_decode_entry(&entry);

        written += utf8_decompose_char(entry_cp, dst + written,
                                       (bufsize > written) ? (bufsize - written) : 0, options,
                                       last_boundclass);
        if (written < 0)
            return UTF8_ERROR_OVERFLOW;
    }
    return written;
}

int32_t CLOX_STDCALL utf8_tolower(int32_t c)
{
    int32_t cl = utf8_get_property(c)->lowercase_seqindex;
    return cl != UINT16_MAX ? seqindex_decode_index((uint32_t)cl) : c;
}

int32_t CLOX_STDCALL utf8_toupper(int32_t c)
{
    int32_t cu = utf8_get_property(c)->uppercase_seqindex;
    return cu != UINT16_MAX ? seqindex_decode_index((uint32_t)cu) : c;
}

int32_t CLOX_STDCALL utf8_totitle(int32_t c)
{
    int32_t cu = utf8_get_property(c)->titlecase_seqindex;
    return cu != UINT16_MAX ? seqindex_decode_index((uint32_t)cu) : c;
}

int CLOX_STDCALL utf8_islower(int32_t c)
{
    const utf8_property_t *p = utf8_get_property(c);
    return p->lowercase_seqindex != p->uppercase_seqindex && p->lowercase_seqindex == UINT16_MAX;
}

int CLOX_STDCALL utf8_isupper(int32_t c)
{
    const utf8_property_t *p = utf8_get_property(c);
    return p->lowercase_seqindex != p->uppercase_seqindex && p->uppercase_seqindex == UINT16_MAX && p->category != UTF8_CATEGORY_LT;
}

/* return a character width analogous to wcwidth (except portable and
   hopefully less buggy than most system wcwidth functions). */
int CLOX_STDCALL utf8_charwidth(int32_t c)
{
    return utf8_get_property(c)->charwidth;
}

bool_t CLOX_STDCALL utf8_charwidth_ambiguous(int32_t c)
{
    return utf8_get_property(c)->ambiguous_width;
}

utf8_category_t CLOX_STDCALL utf8_category(int32_t c)
{
    return (utf8_category_t)utf8_get_property(c)->category;
}

const char *CLOX_STDCALL utf8_category_string(int32_t c)
{
    static const char s[][3] = {"Cn", "Lu", "Ll", "Lt", "Lm", "Lo", "Mn", "Mc", "Me", "Nd", "Nl", "No", "Pc", "Pd", "Ps", "Pe", "Pi", "Pf", "Po", "Sm", "Sc", "Sk", "So", "Zs", "Zl", "Zp", "Cc", "Cf", "Cs", "Co"};
    return s[utf8_category(c)];
}

#define utf8_decompose_lump(replacement_uc)                    \
    return utf8_decompose_char((replacement_uc), dst, bufsize, \
                               options & ~(unsigned int)UTF8_LUMP, last_boundclass)

ssize_t CLOX_STDCALL utf8_decompose_char(int32_t uc, int32_t *dst, ssize_t bufsize, utf8_option_t options, int *last_boundclass)
{
    const utf8_property_t *property;
    uint16_t category;
    int32_t hangul_sindex;
    if (uc < 0 || uc >= 0x110000)
        return UTF8_ERROR_NOTASSIGNED;
    property = unsafe_get_property(uc);
    category = property->category;
    hangul_sindex = uc - UTF8_HANGUL_SBASE;
    if (options & (UTF8_COMPOSE | UTF8_DECOMPOSE))
    {
        if (hangul_sindex >= 0 && hangul_sindex < UTF8_HANGUL_SCOUNT)
        {
            int32_t hangul_tindex;
            if (bufsize >= 1)
            {
                dst[0] = UTF8_HANGUL_LBASE +
                         hangul_sindex / UTF8_HANGUL_NCOUNT;
                if (bufsize >= 2)
                    dst[1] = UTF8_HANGUL_VBASE +
                             (hangul_sindex % UTF8_HANGUL_NCOUNT) / UTF8_HANGUL_TCOUNT;
            }
            hangul_tindex = hangul_sindex % UTF8_HANGUL_TCOUNT;
            if (!hangul_tindex)
                return 2;
            if (bufsize >= 3)
                dst[2] = UTF8_HANGUL_TBASE + hangul_tindex;
            return 3;
        }
    }
    if (options & UTF8_REJECTNA)
    {
        if (!category)
            return UTF8_ERROR_NOTASSIGNED;
    }
    if (options & UTF8_IGNORE)
    {
        if (property->ignorable)
            return 0;
    }
    if (options & UTF8_STRIPNA)
    {
        if (!category)
            return 0;
    }
    if (options & UTF8_LUMP)
    {
        if (category == UTF8_CATEGORY_ZS)
            utf8_decompose_lump(0x0020);
        if (uc == 0x2018 || uc == 0x2019 || uc == 0x02BC || uc == 0x02C8)
            utf8_decompose_lump(0x0027);
        if (category == UTF8_CATEGORY_PD || uc == 0x2212)
            utf8_decompose_lump(0x002D);
        if (uc == 0x2044 || uc == 0x2215)
            utf8_decompose_lump(0x002F);
        if (uc == 0x2236)
            utf8_decompose_lump(0x003A);
        if (uc == 0x2039 || uc == 0x2329 || uc == 0x3008)
            utf8_decompose_lump(0x003C);
        if (uc == 0x203A || uc == 0x232A || uc == 0x3009)
            utf8_decompose_lump(0x003E);
        if (uc == 0x2216)
            utf8_decompose_lump(0x005C);
        if (uc == 0x02C4 || uc == 0x02C6 || uc == 0x2038 || uc == 0x2303)
            utf8_decompose_lump(0x005E);
        if (category == UTF8_CATEGORY_PC || uc == 0x02CD)
            utf8_decompose_lump(0x005F);
        if (uc == 0x02CB)
            utf8_decompose_lump(0x0060);
        if (uc == 0x2223)
            utf8_decompose_lump(0x007C);
        if (uc == 0x223C)
            utf8_decompose_lump(0x007E);
        if ((options & UTF8_NLF2LS) && (options & UTF8_NLF2PS))
        {
            if (category == UTF8_CATEGORY_ZL ||
                category == UTF8_CATEGORY_ZP)
                utf8_decompose_lump(0x000A);
        }
    }
    if (options & UTF8_STRIPMARK)
    {
        if (category == UTF8_CATEGORY_MN ||
            category == UTF8_CATEGORY_MC ||
            category == UTF8_CATEGORY_ME)
            return 0;
    }
    if (options & UTF8_CASEFOLD)
    {
        if (property->casefold_seqindex != UINT16_MAX)
        {
            return seqindex_write_char_decomposed(property->casefold_seqindex, dst, bufsize, options, last_boundclass);
        }
    }
    if (options & (UTF8_COMPOSE | UTF8_DECOMPOSE))
    {
        if (property->decomp_seqindex != UINT16_MAX &&
            (!property->decomp_type || (options & UTF8_COMPAT)))
        {
            return seqindex_write_char_decomposed(property->decomp_seqindex, dst, bufsize, options, last_boundclass);
        }
    }
    if (options & UTF8_CHARBOUND)
    {
        bool_t boundary;
        boundary = grapheme_break_extended(0, property->boundclass, 0, property->indic_conjunct_break,
                                           last_boundclass);
        if (boundary)
        {
            if (bufsize >= 1)
                dst[0] = -1; /* sentinel value for grapheme break */
            if (bufsize >= 2)
                dst[1] = uc;
            return 2;
        }
    }
    if (bufsize >= 1)
        *dst = uc;
    return 1;
}

ssize_t CLOX_STDCALL utf8_decompose(
    const uint8_t *str, ssize_t strlen,
    int32_t *buffer, ssize_t bufsize, utf8_option_t options)
{
    return utf8_decompose_custom(str, strlen, buffer, bufsize, options, NULL, NULL);
}

ssize_t CLOX_STDCALL utf8_decompose_custom(
    const uint8_t *str, ssize_t strlen,
    int32_t *buffer, ssize_t bufsize, utf8_option_t options,
    utf8_custom_func_t custom_func, void *custom_data)
{
    /* strlen will be ignored, if UTF8_NULLTERM is set in options */
    ssize_t wpos = 0;
    if ((options & UTF8_COMPOSE) && (options & UTF8_DECOMPOSE))
        return UTF8_ERROR_INVALIDOPTS;
    if ((options & UTF8_STRIPMARK) &&
        !(options & UTF8_COMPOSE) && !(options & UTF8_DECOMPOSE))
        return UTF8_ERROR_INVALIDOPTS;
    {
        int32_t uc;
        ssize_t rpos = 0;
        ssize_t decomp_result;
        int boundclass = UTF8_BOUNDCLASS_START;
        while (1)
        {
            if (options & UTF8_NULLTERM)
            {
                rpos += utf8_iterate(str + rpos, -1, &uc);
                /* checking of return value is not necessary,
                   as 'uc' is < 0 in case of error */
                if (uc < 0)
                    return UTF8_ERROR_INVALIDUTF8;
                if (rpos < 0)
                    return UTF8_ERROR_OVERFLOW;
                if (uc == 0)
                    break;
            }
            else
            {
                if (rpos >= strlen)
                    break;
                rpos += utf8_iterate(str + rpos, strlen - rpos, &uc);
                if (uc < 0)
                    return UTF8_ERROR_INVALIDUTF8;
            }
            if (custom_func != NULL)
            {
                uc = custom_func(uc, custom_data); /* user-specified custom mapping */
            }
            decomp_result = utf8_decompose_char(
                uc, buffer + wpos, (bufsize > wpos) ? (bufsize - wpos) : 0, options,
                &boundclass);
            if (decomp_result < 0)
                return decomp_result;
            wpos += decomp_result;
            /* prohibiting integer overflows due to too long strings: */
            if (wpos < 0 ||
                wpos > (ssize_t)(SSIZE_MAX / sizeof(int32_t) / 2))
                return UTF8_ERROR_OVERFLOW;
        }
    }
    if ((options & (UTF8_COMPOSE | UTF8_DECOMPOSE)) && bufsize >= wpos)
    {
        ssize_t pos = 0;
        while (pos < wpos - 1)
        {
            int32_t uc1, uc2;
            const utf8_property_t *property1, *property2;
            uc1 = buffer[pos];
            uc2 = buffer[pos + 1];
            property1 = unsafe_get_property(uc1);
            property2 = unsafe_get_property(uc2);
            if (property1->combining_class > property2->combining_class &&
                property2->combining_class > 0)
            {
                buffer[pos] = uc2;
                buffer[pos + 1] = uc1;
                if (pos > 0)
                    pos--;
                else
                    pos++;
            }
            else
            {
                pos++;
            }
        }
    }
    return wpos;
}

ssize_t CLOX_STDCALL utf8_normalize_utf32(int32_t *buffer, ssize_t length, utf8_option_t options)
{
    /* UTF8_NULLTERM option will be ignored, 'length' is never ignored */
    if (options & (UTF8_NLF2LS | UTF8_NLF2PS | UTF8_STRIPCC))
    {
        ssize_t rpos;
        ssize_t wpos = 0;
        int32_t uc;
        for (rpos = 0; rpos < length; rpos++)
        {
            uc = buffer[rpos];
            if (uc == 0x000D && rpos < length - 1 && buffer[rpos + 1] == 0x000A)
                rpos++;
            if (uc == 0x000A || uc == 0x000D || uc == 0x0085 ||
                ((options & UTF8_STRIPCC) && (uc == 0x000B || uc == 0x000C)))
            {
                if (options & UTF8_NLF2LS)
                {
                    if (options & UTF8_NLF2PS)
                    {
                        buffer[wpos++] = 0x000A;
                    }
                    else
                    {
                        buffer[wpos++] = 0x2028;
                    }
                }
                else
                {
                    if (options & UTF8_NLF2PS)
                    {
                        buffer[wpos++] = 0x2029;
                    }
                    else
                    {
                        buffer[wpos++] = 0x0020;
                    }
                }
            }
            else if ((options & UTF8_STRIPCC) &&
                     (uc < 0x0020 || (uc >= 0x007F && uc < 0x00A0)))
            {
                if (uc == 0x0009)
                    buffer[wpos++] = 0x0020;
            }
            else
            {
                buffer[wpos++] = uc;
            }
        }
        length = wpos;
    }
    if (options & UTF8_COMPOSE)
    {
        int32_t *starter = NULL;
        int32_t current_char;
        const utf8_property_t *starter_property = NULL, *current_property;
        uint16_t max_combining_class = -1;
        ssize_t rpos;
        ssize_t wpos = 0;
        int32_t composition;
        for (rpos = 0; rpos < length; rpos++)
        {
            current_char = buffer[rpos];
            current_property = unsafe_get_property(current_char);
            if (starter && current_property->combining_class > max_combining_class)
            {
                /* combination perhaps possible */
                int32_t hangul_lindex;
                int32_t hangul_sindex;
                hangul_lindex = *starter - UTF8_HANGUL_LBASE;
                if (hangul_lindex >= 0 && hangul_lindex < UTF8_HANGUL_LCOUNT)
                {
                    int32_t hangul_vindex;
                    hangul_vindex = current_char - UTF8_HANGUL_VBASE;
                    if (hangul_vindex >= 0 && hangul_vindex < UTF8_HANGUL_VCOUNT)
                    {
                        *starter = UTF8_HANGUL_SBASE +
                                   (hangul_lindex * UTF8_HANGUL_VCOUNT + hangul_vindex) *
                                       UTF8_HANGUL_TCOUNT;
                        starter_property = NULL;
                        continue;
                    }
                }
                hangul_sindex = *starter - UTF8_HANGUL_SBASE;
                if (hangul_sindex >= 0 && hangul_sindex < UTF8_HANGUL_SCOUNT &&
                    (hangul_sindex % UTF8_HANGUL_TCOUNT) == 0)
                {
                    int32_t hangul_tindex;
                    hangul_tindex = current_char - UTF8_HANGUL_TBASE;
                    if (hangul_tindex >= 0 && hangul_tindex < UTF8_HANGUL_TCOUNT)
                    {
                        *starter += hangul_tindex;
                        starter_property = NULL;
                        continue;
                    }
                }
                if (!starter_property)
                {
                    starter_property = unsafe_get_property(*starter);
                }
                if (starter_property->comb_index < 0x8000 &&
                    current_property->comb_index != UINT16_MAX &&
                    current_property->comb_index >= 0x8000)
                {
                    int sidx = starter_property->comb_index;
                    int idx = current_property->comb_index & 0x3FFF;
                    if (idx >= utf8_combinations[sidx] && idx <= utf8_combinations[sidx + 1])
                    {
                        idx += sidx + 2 - utf8_combinations[sidx];
                        if (current_property->comb_index & 0x4000)
                        {
                            composition = (utf8_combinations[idx] << 16) | utf8_combinations[idx + 1];
                        }
                        else
                            composition = utf8_combinations[idx];

                        if (composition > 0 && (!(options & UTF8_STABLE) ||
                                                !(unsafe_get_property(composition)->comp_exclusion)))
                        {
                            *starter = composition;
                            starter_property = NULL;
                            continue;
                        }
                    }
                }
            }
            buffer[wpos] = current_char;
            if (current_property->combining_class)
            {
                if (current_property->combining_class > max_combining_class)
                {
                    max_combining_class = current_property->combining_class;
                }
            }
            else
            {
                starter = buffer + wpos;
                starter_property = NULL;
                max_combining_class = -1;
            }
            wpos++;
        }
        length = wpos;
    }
    return length;
}

ssize_t CLOX_STDCALL utf8_reencode(int32_t *buffer, ssize_t length, utf8_option_t options)
{
    /* UTF8_NULLTERM option will be ignored, 'length' is never ignored
       ASSERT: 'buffer' has one spare byte of free space at the end! */
    length = utf8_normalize_utf32(buffer, length, options);
    if (length < 0)
        return length;
    {
        ssize_t rpos, wpos = 0;
        int32_t uc;
        if (options & UTF8_CHARBOUND)
        {
            for (rpos = 0; rpos < length; rpos++)
            {
                uc = buffer[rpos];
                wpos += charbound_encode_char(uc, ((uint8_t *)buffer) + wpos);
            }
        }
        else
        {
            for (rpos = 0; rpos < length; rpos++)
            {
                uc = buffer[rpos];
                wpos += utf8_encode_char(uc, ((uint8_t *)buffer) + wpos);
            }
        }
        ((uint8_t *)buffer)[wpos] = 0;
        return wpos;
    }
}

ssize_t CLOX_STDCALL utf8_map(
    const uint8_t *str, ssize_t strlen, uint8_t **dstptr, utf8_option_t options)
{
    return utf8_map_custom(str, strlen, dstptr, options, NULL, NULL);
}

ssize_t CLOX_STDCALL utf8_map_custom(
    const uint8_t *str, ssize_t strlen, uint8_t **dstptr, utf8_option_t options,
    utf8_custom_func_t custom_func, void *custom_data)
{
    int32_t *buffer;
    ssize_t result;
    *dstptr = NULL;
    result = utf8_decompose_custom(str, strlen, NULL, 0, options, custom_func, custom_data);
    if (result < 0)
        return result;
    buffer = (int32_t *)malloc(((size_t)result) * sizeof(int32_t) + 1);
    if (!buffer)
        return UTF8_ERROR_NOMEM;
    result = utf8_decompose_custom(str, strlen, buffer, result, options, custom_func, custom_data);
    if (result < 0)
    {
        free(buffer);
        return result;
    }
    result = utf8_reencode(buffer, result, options);
    if (result < 0)
    {
        free(buffer);
        return result;
    }
    {
        int32_t *newptr;
        newptr = (int32_t *)realloc(buffer, (size_t)result + 1);
        if (newptr)
            buffer = newptr;
    }
    *dstptr = (uint8_t *)buffer;
    return result;
}

uint8_t *CLOX_STDCALL utf8_NFD(const uint8_t *str)
{
    uint8_t *retval;
    utf8_map(str, 0, &retval, UTF8_NULLTERM | UTF8_STABLE | UTF8_DECOMPOSE);
    return retval;
}

uint8_t *CLOX_STDCALL utf8_NFC(const uint8_t *str)
{
    uint8_t *retval;
    utf8_map(str, 0, &retval, UTF8_NULLTERM | UTF8_STABLE | UTF8_COMPOSE);
    return retval;
}

uint8_t *CLOX_STDCALL utf8_NFKD(const uint8_t *str)
{
    uint8_t *retval;
    utf8_map(str, 0, &retval, UTF8_NULLTERM | UTF8_STABLE | UTF8_DECOMPOSE | UTF8_COMPAT);
    return retval;
}

uint8_t *CLOX_STDCALL utf8_NFKC(const uint8_t *str)
{
    uint8_t *retval;
    utf8_map(str, 0, &retval, UTF8_NULLTERM | UTF8_STABLE | UTF8_COMPOSE | UTF8_COMPAT);
    return retval;
}

uint8_t *CLOX_STDCALL utf8_NFKC_Casefold(const uint8_t *str)
{
    uint8_t *retval;
    utf8_map(str, 0, &retval, UTF8_NULLTERM | UTF8_STABLE | UTF8_COMPOSE | UTF8_COMPAT | UTF8_CASEFOLD | UTF8_IGNORE);
    return retval;
}
