#pragma once
#include <GL\glew.h>
#include <map>
#include "color.h"
#include "TextShader.h"

struct Character {
	unsigned int texture;
	unsigned int sizeX;
	unsigned int sizeY;
	int bearingLeft;
	int bearingTop;
	int advanceX;
	int advanceY;
};

struct FontEntry {
	int fontHeight;
	std::map<char, Character> characters;
};

class Text
{
public:
	Text();
	~Text() { delete shader; }
    void Render(const char* text, float x, float y, float scale, const Color& col);
	void setFont(FontEntry* entry) { this->entry = entry; }

private:
    FontEntry* entry = NULL;

    TextShader* shader = NULL;
    unsigned int VAO;
    unsigned int VBO;
};

