#include "Text.h"
#include <iostream>
#include "CameraManager.h"

Text::Text()
{
	shader = new TextShader();
	Init(font);
}

void Text::Render(const char* text, float x, float y, float scale, Color col)
{
	shader->activate(*CameraManager::getInstance().activeCamera);
	shader->textColor(col);

	float originX = x;
	int scr_width = CameraManager::getInstance().activeCamera->WindowWidth;
	int scr_height = CameraManager::getInstance().activeCamera->WindowHeight;

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	for (const char* c = text; *c; c++) {
		if (*c == '\n') {
			y -= (float) ((fontHeight >> 6) * scale / scr_height) * 4;
			x = originX;
			std::cout << y << std::endl;
			continue;
		}

		Character ch = Characters[*c];

		float xpos = ((x * 2- 1.0f) * scr_width + ch.bearingLeft * scale) / scr_width;
		float ypos = ((y * 2- 1.0f) * scr_height - (ch.sizeY - ch.bearingTop) * scale) / scr_height;

		float width = ch.sizeX * scale / scr_width;
		float height = ch.sizeY * scale / scr_height;

		float vertices[6][4] = {
			{ xpos,			ypos + height,	0.0, 0.0 },
			{ xpos,			ypos,			0.0, 1.0 },
			{ xpos + width, ypos,			1.0, 1.0 },

			{ xpos,			ypos + height,	0.0, 0.0 },
			{ xpos + width, ypos,			1.0, 1.0 },
			{ xpos + width, ypos + height,	1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texture);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		x += (ch.advanceX >> 6) * scale / scr_width / 2;
		y += (ch.advanceY >> 6) * scale / scr_height / 2;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->deactivate();
}

void Text::Init(const char* font)
{
	Characters.clear();

	std::string location = ASSET_DIRECTORY;
	location.append(font);

	FT_Library lib;
	if (FT_Init_FreeType(&lib))
		throw new std::exception("Could not init FreeType library.");

	FT_Face face;
	if (FT_New_Face(lib, location.c_str(), 0, &face))
		throw new std::exception("Could not load font.");

	FT_Set_Pixel_Sizes(face, 0, 128);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
			face->glyph->advance.x,
			face->glyph->advance.y
		};
		Characters.insert(std::pair<char, Character>(c, character));			
	}

	fontHeight = face->height;

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(lib);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
