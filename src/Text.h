#pragma once
#include <GL\glew.h>
#include <map>
#include "color.h"
#include "TextShader.h"

#include "ft2build.h"
#include FT_FREETYPE_H

struct Character {
    unsigned int texture;
    int sizeX;
    int sizeY;
    int bearingLeft;
    int bearingTop;
    unsigned int advance;
};

class Text
{
public:
    Text(const char* font);
    void Render(const char* text, float x, float y, float scale, Color col);

protected:
    void Init(const char* font);

private:
    std::map<char, Character> Characters;

    TextShader* shader;
    unsigned int VAO;
    unsigned int VBO;
};

