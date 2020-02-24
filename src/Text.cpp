#include "Text.h"
#include <iostream>
#include "CameraManager.h"
#include "TextManager.h"

Text::Text()
{
	shader = new TextShader();
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

// Calculates text-rectangles for the character textures to be rendered onto
void Text::Render(const char* text, float x, float y, float scale, const Color& col)
{
	if (!entry) return;

	glDisable(GL_DEPTH_TEST);
	shader->activate(*CameraManager::getInstance().activeCamera);
	shader->textColor(col);

	float originX = x;
	float scr_width = (float) CameraManager::getInstance().activeCamera->WindowWidth;
	float scr_height = (float) CameraManager::getInstance().activeCamera->WindowHeight;

	scale *= scr_width / 720.0f;

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// foreach character in text get properties from Character-struct and apply texture to defined quad
	for (const char* c = text; *c; c++) {
		if (*c == '\n') {
			y -= (float) ((entry->fontHeight >> 6) * scale / scr_height) * 4;
			x = originX;
			continue;
		}

		Character ch = entry->characters[*c];

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
		
		x += (ch.advanceX >> 6) * scale / (float)scr_width / 2.0f;
		y += (ch.advanceY >> 6) * scale / (float)scr_height / 2.0f;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->deactivate();
	glEnable(GL_DEPTH_TEST);
	entry = NULL;
}

