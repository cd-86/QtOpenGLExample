#include "FontFace.h"

#include <QStandardPaths>
#include <QDebug>
#include <QtMath>
#include <QImage>
#include <QFileInfo>

#define CHAR_X_OFFSET 2
#define CHAR_Y_OFFSET 2

FontFace::FontFace()
{
    m_textureData = new uchar[0];
    if(FT_Init_FreeType(&m_ft)) {
        qDebug() << "ERROR::FREETYPE: Could not init FreeType Library";
        return;
    }
    if(FT_New_Face(m_ft, "fonts/NotoSansSC-Regular.ttf", 0, &m_face)) {
        qDebug() << "ERROR::FREETYPE: Failed to load font";
        return;
    }
    FT_Set_Pixel_Sizes(m_face, 0, 48);
}

FontFace::~FontFace()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
    delete[] m_textureData;
}

void FontFace::getVertexData(QVector<QVector4D> &vertex, QVector<uint32_t> &index, std::string text, float x, float y, int alignment, int spacing, int first, float scale)
{
    QString qstr = QString::fromStdString(text);
    getVertexData(vertex, index, qstr, x, y, alignment, spacing, first, scale);
}

void FontFace::getVertexData(QVector<QVector4D> &vertex, QVector<uint32_t> &index, QString text, float x, float y, int alignment, int spacing, int first, float scale)
{
    QVector<float> w;
    w.append(0.0f);
    for (QChar c : text) {
        if (c == '\n'){
            w.append(0.0f);
            continue;
        }
        if (!m_fontMap.contains(c)){
            addChar(c);
        }
        w.last() += (float) m_fontMap[c].advance * scale;
    }

    if (alignment & Qt::AlignHCenter){
        for (int i = 0; i < w.size(); ++i) {
            w[i] = x - w[i] / 2;
        }
    } else if (alignment & Qt::AlignRight) {
        for (int i = 0; i < w.size(); ++i) {
            w[i] = x - w[i];
        }
    } else {
        for (int i = 0; i < w.size(); ++i) {
            w[i] = x;
        }
    }

    if (alignment & Qt::AlignVCenter){
        y += (w.size() - 1) * 48 * scale - 24 * scale;
    } else if (alignment & Qt::AlignBottom) {
        y += w.size() * 48 * scale;
    }
    qDebug() << y;
    x = w[0];
    int i = 0;
    int line = 0;
    float x1, x2, y1, y2;
    for (QChar c : text) {
        if (c == '\n'){
            line += 1;
            x = w[line];
            y -= (48 + spacing) * scale;
            continue;
        }
        TextureInfo _c = m_fontMap[c];
        x1 = x + (float)_c.bearingX * scale;
        y1 = y + (float)_c.bearingY * scale;
        x2 = x1 + (float)_c.width * scale;
        y2 = y1 - (float)_c.height * scale;
        vertex.append(QVector4D(x1, y1, _c.xOffset, CHAR_Y_OFFSET));
        vertex.append(QVector4D(x2, y1, _c.xOffset + _c.width, CHAR_Y_OFFSET));
        vertex.append(QVector4D(x2, y2, _c.xOffset + _c.width, _c.height + CHAR_Y_OFFSET));
        vertex.append(QVector4D(x1, y2, _c.xOffset, _c.height + CHAR_Y_OFFSET));
        int j = first + i * 4;
        index << j << j + 1 << j + 2 << j << j + 2 << j + 3;
        x += _c.advance * scale;
        i++;
    }
}

QImage FontFace::getImage()
{
    return QImage(m_textureData, m_textureWidth, m_textureHeight, m_textureWidth, QImage::Format_Grayscale8);
}

void FontFace::addChar(QChar c)
{
    if(FT_Load_Char(m_face, c.unicode(), FT_LOAD_RENDER))
        qDebug() << "ERROR::FREETYTPE: Failed to load Glyph";
    FT_Bitmap &bitmap = m_face->glyph->bitmap;
    uint newWidth = m_textureWidth + bitmap.width + CHAR_X_OFFSET;
    uint newHeight = qMax<uint>(bitmap.rows + CHAR_Y_OFFSET * 2, m_textureHeight);
    uchar *newTexture = new uchar[newWidth * newHeight];
    for (uint i = 0; i < newWidth * newHeight; ++i){
        newTexture[i] = 0;
    }
    for (uint i = 0; i < m_textureHeight; ++i) {
        for (uint j = 0; j < m_textureWidth; ++j) {
            newTexture[i * newWidth + j] = m_textureData[i * m_textureWidth + j];
        }
    }
    for (uint i = 0; i < bitmap.rows; ++i) {
        for (uint j = 0; j < bitmap.width; ++j) {
            newTexture[(i + CHAR_Y_OFFSET) * newWidth + j + m_textureWidth] = bitmap.buffer[i * bitmap.width + j];
        }
    }
    m_fontMap[c] = {
        bitmap.width,
        bitmap.rows,
        m_face->glyph->bitmap_left,
        m_face->glyph->bitmap_top,
        m_face->glyph->advance.x >> 6,
        m_textureWidth
    };
    delete[] m_textureData;
    m_textureData = newTexture;
    m_textureWidth = newWidth;
    m_textureHeight = newHeight;
}
