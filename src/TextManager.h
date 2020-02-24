#pragma once
#include "TextShader.h"
#include "color.h"
#include <map>
#include <vector>
#include <string.h>

#include <GL\glew.h>

#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>

#ifdef WIN32
#define FONT_DIRECTORY "../../assets/fonts/"
#else
#define FONT_DIRECTORY "../assets/fonts/"
#endif

#include "Text.h"

class TextManager
{
public:
	static TextManager& getInstance() {
		static TextManager instance;
		return instance;
	}

private:
	std::map<std::string, FontEntry> fontMap;
	std::vector<Text> texts;
	TextShader shader;
	int currentIndex = 0;
	int neededIndex = 0;

	TextManager();
	TextManager(const TextManager& im) {};
	TextManager& operator= (const TextManager& mg) {};

public:
	void Begin() {
		currentIndex = 0;
		neededIndex = 0;
	}

	void End();

	FontEntry addFont(const char* Fontname);;

	void renderText(const char* fontName, const char* text, float x, float y, float scale, Color col);

};

