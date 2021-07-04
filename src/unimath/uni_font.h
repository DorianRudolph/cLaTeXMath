#ifndef UNI_FONT_INCLUDED
#define UNI_FONT_INCLUDED

#include <map>

#include "common.h"
#include "utils/utils.h"
#include "otf/otf.h"
#include "unimath/math_type.h"
#include "unimath/uni_char.h"
#include "unimath/uni_symbol.h"

namespace tex {

/** Represents an open-type font with font-file and font-spec */
struct OtfFont final {
  const i32 _id;
  const std::string _fontFile;
  const sptr<const Otf> _otf;

  OtfFont(i32 id, std::string fontFile, const std::string& clmFile);

  inline const Otf& otf() const { return *_otf; }
};

/** Enum represents all supported font style */
enum class FontStyle : u16 {
  none = 0,
  rm = 0b1,
  bf = 0b10,
  it = 0b100,
  cal = 0b1000,
  frak = 0b10000,
  bb = 0b100000,
  sf = 0b1000000,
  tt = 0b10000000,
  // composed styles
  bfit = bf | it,
  bfcal = bf | cal,
  bffrak = bf | frak,
  sfbf = bf | sf,
  sfit = it | sf,
  sfbfit = bf | it | sf,
  // invalid
  invalid = 0xffff
};

struct FontFamily final {
private:
  std::map<FontStyle, sptr<const OtfFont>> _styles;

public:
  no_copy_assign(FontFamily);

  FontFamily() = default;

  /** Get the FontStyle from the given style name */
  static FontStyle fontStyleOf(const std::string& name);

  /** Add a font to this family with given style */
  void add(const std::string& styleName, const sptr<const OtfFont>& font);

  /** Get the font corresponding to the given style, return null if not found */
  sptr<const OtfFont> get(FontStyle style) const;
};

/** Parameters to load a font */
struct FontSpec {
  const std::string name;
  const std::string fontFile;
  const std::string clmFile;
};

class FontContext {
private:
  /** style name to version map */
  static std::map<std::string, sptr<const MathVersion>> _mathStyles;
  /** font style to version map */
  static std::map<FontStyle, sptr<const MathVersion>> _mathVersions;

  static int _lastId;
  static std::vector<sptr<const OtfFont>> _fonts;

  static std::map<std::string, sptr<FontFamily>> _mainFonts;
  static std::map<std::string, sptr<const OtfFont>> _mathFonts;

  /**
   * Get the math font style from the given name
   *
   * @param name the style name
   * @return the corresponding FontStyle or FontStyle::none if not found
   */
  static FontStyle mathFontStyleOf(const std::string& name);

  /**
   * Get the main font style from the given name
   *
   * @param name the style name
   * @return the corresponding FontStyle or FontStyle::none if not found
   */
  static FontStyle mainFontStyleOf(const std::string& name);

  /**
   * Get the most similar font style from the given source font style.
   *
   * <p>
   * The similarity is measured by the number of set bits of the result
   * that generated by the bitwise operation '&' between source font style
   * and the composed font styles.
   */
  static FontStyle findClosestStyle(const FontStyle src);

  sptr<FontFamily> _mainFont = nullptr;
  sptr<const OtfFont> _mathFont = nullptr;

public:
  no_copy_assign(FontContext);

  FontContext() = default;

  static constexpr i32 NO_FONT = -1;

  /**
   * Add main font (collection) to context.
   *
   * @param versionName the name of this font (collection)
   * @param params font-spec to load
   */
  static void addMainFont(
    const std::string& versionName,
    const std::vector<FontSpec>& params
  );

  /** Add math font to context. */
  static void addMathFont(const FontSpec& params);

  /** Check if has math font */
  static bool hasMathFont();

  /**
   * Set math style to display formulas.
   *
   * @param styleName one of the TeX, ISO, French, and upright
   */
  static void setMathStyle(const std::string& styleName);

  /** Get font-spec from given id, return nullptr if not found. */
  static sptr<const OtfFont> getFont(i32 id);

  /** Select math font by the given version name */
  void selectMathFont(const std::string& versionName);

  /** Select main font by the given version name */
  void selectMainFont(const std::string& versionName);

  /** Get the math font currently in use */
  inline const OtfFont& mathFont() { return *_mathFont; }

  /** Get the id of the math font currently in use */
  inline i32 mathFontId() const { return _mathFont->_id; }

  /** Get the char-object from given symbol */
  Char getChar(const Symbol& symbol, FontStyle style) const;

  /** Get the char-object from given code and styleName */
  Char getChar(c32 code, const std::string& styleName, bool isMathMode) const;

  /** Get the char-object from the given code and style */
  Char getChar(c32 code, FontStyle style, bool isMathMode) const;
};

}  // namespace tex

#endif
