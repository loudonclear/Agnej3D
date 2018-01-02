#ifndef FONT_H
#define FONT_H

#include "util/CommonIncludes.h"
#include <string>
#include <memory>

/*
 * TODO (optional): You can make this implementation cleaner by replacing gl**
 * methods with your own wrapper classes (Texture, VAO, VBO, openglshape). You may
 * also want to replace the glUniform** methods with calls to your custom graphics
 * object.
 *
 * Other parameters you can change (also totally optional):
 *      - Oversampling -- currently set to 2 in the horizontal direction, 2 in the
 *      vertical direction. Increase oversampling for greater sub-pixel precision
 *      (less aliasing, in particular if the text is in motion)
 *
 *      - Color -- you'll notice that all characters are currently drawn in red.
 *      This is because the font texture has only one color channel, but glsl
 *      interprets this channel as the intensity of the "red" color channel, and
 *      thinks that the other two channels are 0. In order to make the color of the
 *      text arbitrary, you can introduce one or two more uniforms into the default
 *      shader.
 *              Most likely you'll want an "isFont" uniform, which, when it's
 *              set to 1 will make it so that the texture color red value is
 *              interpreted as the overall intensity.
 *
 *
 * tldr: Play around with this class to get some nice looking text! Or don't, that's fine too!
 */

// Forward declarations of structs needed for font rendering
struct AlignedQuad;
struct PackedChar;
struct FontInfo;

// Other forward declarations
class QString;

/**
 * @brief FontMetrics
 *
 * Struct containing info about text size
 */
struct FontMetrics {
    FontMetrics(float a, float d, float w) : ascent(a), descent(d), width(w) {}

    float ascent; // units that text ascends above the baseline
    float descent; // units that text descends below the baseline
    float width; // width of text
};

/**
 * @class Font
 * @author battal
 *
 * Knows how to draw text using a specific font. Also supports querying information about text,
 * drawn at a certain point size using the same font.
 *
 */
class Font
{
public:
    Font(const char *path, float fontResolution = 50, glm::vec2 fontTextureDimensions = glm::vec2(1024, 1024),
         int charOffset = 32, int numChars = 96);
    virtual ~Font();

    void renderText(GLuint shader, QString text, float pointSize, glm::vec3 startPos);
    FontMetrics getFontMetrics(float pointSize, QString text);

private:
    void _loadFont(const char *path);
    void _renderText(GLuint shader, std::string text, float pointSize, glm::vec3 startPos);
    void _drawQuad(float x0, float y0, float x1, float y1, float s0, float t0, float s1, float t1);
    FontMetrics _getFontMetrics(float pointSize, std::string text);

    float m_fontRes;
    GLuint m_fontTex;
    glm::vec2 m_fontTexDims;

    int m_charOffset, m_numChars;
    std::unique_ptr<PackedChar[]> m_charData;
};

#endif // FONT_H
