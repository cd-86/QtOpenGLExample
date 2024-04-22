#ifndef FONTFACE_H
#define FONTFACE_H

#include <QMap>
#include <QVector4D>

#include "ft2build.h"
#include FT_FREETYPE_H
class FontFace
{
public:
    FontFace();
    ~FontFace();

    void getVertexData(QVector<QVector4D> &vertex, QVector<uint32_t> &index, std::string text, float x, float y, int alignment, int spacing, int first, float scale=1.0f);
    void getVertexData(QVector<QVector4D> &vertex, QVector<uint32_t> &index, QString text, float x, float y, int alignment, int spacing, int first, float scale=1.0f);
    QImage getImage();
    uint textureWidth() {return m_textureWidth;}
    uint textureHeight() {return m_textureHeight;}

private:
    struct TextureInfo
    {
        uint width;
        uint height;
        int bearingX;
        int bearingY;
        int advance;
        uint xOffset;

    };
    void addChar(QChar c);

private:
    bool inited {false};
    uchar *m_textureData {Q_NULLPTR};
    uint m_textureWidth {0};
    uint m_textureHeight {0};
    QMap<QChar, TextureInfo> m_fontMap;
    FT_Library m_ft;
    FT_Face m_face;
};
#endif // FONTFACE_H
