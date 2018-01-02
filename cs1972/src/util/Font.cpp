#include "Font.h"

#include <QFile>
#include <QTextStream>
#include <vector>
#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"

struct AlignedQuad : public stbtt_aligned_quad {};
struct PackedChar : public stbtt_packedchar {};
struct FontInfo : public stbtt_fontinfo {};

/**
 * @brief readFontFile
 * @param path -- path to .ttf file
 *
 * @return raw data in the font file
 */
std::vector<uint8_t> readFontFile(const char* path)
{
    // Create and open file
    QString fontFilePath = QString(path);
    QFile fontFile(fontFilePath);

    if(!fontFile.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open font file" << std::endl;
        std::exit(-1);
    }

    // Get size
    long size = fontFile.size();

    // Read font data
    fontFile.seek(0);
    std::vector<uint8_t> fontData = std::vector<uint8_t>(size);
    fontFile.read(reinterpret_cast<char*>(&fontData[0]), size);

    // Finish
    fontFile.close();
    return fontData;
}

/**
 * @brief Font::Font
 * @param path -- path to the .ttf file that defines the font
 * @param fontResolution -- resolution of the font (how many pixels tall a character is in the font texture). Higher
 *      resolution increases sharpness of characters, but does not affect size upon rendering text
 * @param fontTextureDimensions -- dimensions of the font texture, might need to be larger if fontResolution is larger
 * @param charOffset -- unicode value of the first character this font stores
 * @param numChars -- how many characters this font stores
 */
Font::Font(const char *path, float fontResolution, glm::vec2 fontTextureDimensions, int charOffset, int numChars) :
    m_fontRes(fontResolution), m_fontTexDims(fontTextureDimensions), m_charOffset(charOffset), m_numChars(numChars)
{
    glGenTextures(1, &m_fontTex);
    _loadFont(path);
}

Font::~Font()
{
    glDeleteTextures(1, &m_fontTex);
}

/**
 * @brief Font::_loadFont
 * @param path
 *
 * Packs font bitmap data into m_fontTex, and stores information about character positioning (within the texture)
 * in m_charData.
 *
 */
void Font::_loadFont(const char *path)
{
    // Character positioning and font metadata
    m_charData = std::make_unique<PackedChar[]>(m_numChars);

    // Bitmap in which to store font
    std::unique_ptr<uint8_t[]> fontBitmap = std::make_unique<uint8_t[]>(m_fontTexDims.x * m_fontTexDims.y);

    // Read font data
    std::vector<uint8_t> fontData = readFontFile(path);

    // Initialize pack context
    stbtt_pack_context packContext;
    if(!stbtt_PackBegin(&packContext, fontBitmap.get(), m_fontTexDims.x, m_fontTexDims.y, 0, 1, nullptr)) {
        std::cerr << "Failed to load font" << std::endl;
        std::exit(-1);
    }

    // Pack font into fontBitmap, and put character position metadata in m_charData
    stbtt_PackSetOversampling(&packContext, 2, 2);
    stbtt_PackFontRange(&packContext, fontData.data(), 0, m_fontRes, m_charOffset, m_numChars, m_charData.get());
    stbtt_PackEnd(&packContext);

    // Create font texture from fontBitmap
    glBindTexture(GL_TEXTURE_2D, m_fontTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_fontTexDims.x, m_fontTexDims.y, 0, GL_RED, GL_UNSIGNED_BYTE, fontBitmap.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief Font::renderText
 *
 * @param shader -- shader to use for rendering
 * @param text -- text to render
 * @param pointSize -- scale at which to render the text
 * @param startPos -- start position of text, in world space
 *
 * Renders text using the given shader, with the specified point size and start position
 */
void Font::renderText(GLuint shader, QString text, float pointSize, glm::vec3 startPos)
{
    _renderText(shader, text.toStdString(), pointSize, startPos);
}

void Font::_renderText(GLuint shader, std::string text, float pointSize, glm::vec3 startPos)
{
    // Initialize quad
    float charX = 0, charY = 0;
    AlignedQuad quad;

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fontTex);
    glUniform1i(glGetUniformLocation(shader, "useTexture"), 1);
    glUniform1i(glGetUniformLocation(shader, "tex"), GL_TEXTURE0);

    // Set model matrix
    glm::mat4 model = glm::translate(glm::mat4(), startPos);
    model = glm::scale(model, glm::vec3(pointSize / m_fontRes));
    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, glm::value_ptr(model));

    for(uint i = 0; i < text.length(); i++) {
        char c = text[i];

        // Get quad data
        stbtt_GetPackedQuad(m_charData.get(), m_fontTexDims.x, m_fontTexDims.y, c - m_charOffset, &charX, &charY, &quad, 0);

        // Draw quad
        _drawQuad(quad.x0, -quad.y1, quad.x1, -quad.y0, quad.s0, quad.t1, quad.s1, quad.t0);
    }

    // Don't use texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(glGetUniformLocation(shader, "useTexture"), 0);

    // Reset model matrix
    model = glm::mat4();
    glUniformMatrix4fv(glGetUniformLocation(shader, "m"), 1, GL_FALSE, glm::value_ptr(model));
}

/**
 * @brief Font::_drawQuad
 *
 * Helper method to draw quad with the given start position, end position, and uv coordinates
 */
void Font::_drawQuad(float x0, float y0, float x1, float y1, float s0, float t0, float s1, float t1)
{
    // Quad data
    float quadData[] = {
        x0, y1, 0, 0, 0, 1, s0, t1,
        x0, y0, 0, 0, 0, 1, s0, t0,
        x1, y1, 0, 0, 0, 1, s1, t1,
        x1, y0, 0, 0, 0, 1, s1, t0
    };

    // Create and bind
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Send data
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32,  quadData, GL_STATIC_DRAW);

    // Enable attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Unbind and delete
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

/**
 * @brief Font::getFontMetrics
 * @param pointSize -- scale at which the text is rendered
 * @param text -- text to render
 *
 * @return FontMetrics object, containing information about the ascent, descent, and width of the text given its pointSize
 */
FontMetrics Font::getFontMetrics(float pointSize, QString text)
{
    return _getFontMetrics(pointSize, text.toStdString());
}

FontMetrics Font::_getFontMetrics(float pointSize, std::string text)
{
    // Initialize quad
    float charX = 0, charY = 0;
    AlignedQuad quad;

    // Initial values
    float ascent = -INFINITY;
    float descent = INFINITY;
    float startOffset = INFINITY;

    for(uint i = 0; i < text.length(); i++) {
        char c = text[i];

        // Get quad data
        stbtt_GetPackedQuad(m_charData.get(), m_fontTexDims.x, m_fontTexDims.y, c - m_charOffset, &charX, &charY, &quad, 0);

        // Update font metrics
        ascent = glm::max(-quad.y0, ascent);
        descent = glm::min(-quad.y1, descent);
        startOffset = glm::min(quad.x0, startOffset); // Will be set to the start position of the first quad
    }

    // Width and scale
    float width = quad.x1 + startOffset;
    float scale = pointSize / m_fontRes;

    return FontMetrics(ascent * scale, descent * scale, width * scale);
}
