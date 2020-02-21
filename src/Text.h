#pragma once
#include <GL\glew.h>
#include <map>
#include "color.h"
#include "TextShader.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#ifdef WIN32
#define FONT_DIRECTORY "../../assets/fonts/"
#else
#define FONT_DIRECTORY "../assets/fonts/"
#endif

struct Character {
    unsigned int texture;
    int sizeX;
    int sizeY;
    int bearingLeft;
    int bearingTop;
    unsigned int advanceX;
    unsigned int advanceY;
};

class Text
{
public:
    Text();
    ~Text() { delete shader; }
    void Render(const char* text, float x, float y, float scale, Color col);

    void setFont(const char* font) { this->font = font; Init(this->font); }
    const char* getFont() const { return font; }

protected:
    void Init(const char* font);

private:
    std::map<char, Character> Characters;
    int fontHeight;
    const char* font = "F25_Bank_Printer.ttf";

    TextShader* shader;
    unsigned int VAO;
    unsigned int VBO;
};

