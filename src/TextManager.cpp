#include "TextManager.h"
#include "Text.h"


TextManager::TextManager() { texts.resize(20); }

void TextManager::End() {
	currentIndex = 0;
	if (neededIndex > texts.size()) {
		texts.resize(neededIndex);
	}
}

inline FontEntry TextManager::addFont(const char* Fontname) {
	std::map<std::string, FontEntry>::iterator it;
	if ((it = fontMap.find(Fontname)) != fontMap.end()) {
		return (*it).second;
	}

	std::string location = FONT_DIRECTORY;
	location.append(Fontname);

	FT_Library lib;
	if (FT_Init_FreeType(&lib))
		throw new std::exception("Could not init FreeType library.");

	FT_Face face;
	if (FT_New_Face(lib, location.c_str(), 0, &face))
		throw new std::exception("Could not load font.");

	FT_Set_Pixel_Sizes(face, 0, 128);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FontEntry entry;
	// entry.characters.resize(127);

	for (char c = 0; c < 127; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph: " << c << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			face->glyph->bitmap_left,
			face->glyph->bitmap_top,
			(int)face->glyph->advance.x,
			(int)face->glyph->advance.y
		};

		entry.characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(lib);

	entry.fontHeight = face->height;
	fontMap.insert(std::pair<std::string, FontEntry>(Fontname, entry));

	return entry;
}

void TextManager::renderText(const char* fontName, const char* text, float x, float y, float scale, Color col)
{
	neededIndex++;
	if (currentIndex > texts.size()) {
		return;
	}
	FontEntry entry = addFont(fontName);
	texts[currentIndex].setFont(&entry);
	texts[currentIndex++].Render(text, x, y, scale, col);

}
