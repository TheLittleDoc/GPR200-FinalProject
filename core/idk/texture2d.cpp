#include "texture2d.h"
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace idk
{
	Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode, bool transparent)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		
		stbi_set_flip_vertically_on_load(true);

		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

		if (data)
		{
			if (transparent)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		m_width = width;
		m_height = height;
		m_id = texture;

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glGenTextures(1, &m_id);
		glBindTexture(slot, m_id);
	}

	unsigned int Texture2D::GetID()
	{
		return m_id;
	}
}