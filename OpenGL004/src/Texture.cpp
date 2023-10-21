#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "render.h"

#include "Texture.h"

bool Texture::LoadTexture(unsigned int* texture, const std::string& path, unsigned char index, int texColorMode, int resColorMode, bool bGenerateMipmap)
{
	int width, height, nChannals;
	unsigned char* img_data;
	img_data = stbi_load(path.c_str(), &width, &height, &nChannals, 0);

	if (img_data == 0)
	{
		std::cout << "Fail to load img!" << std::endl;
		return false;
	}
	GLCall(glGenTextures(1, texture));
	GLCall(glActiveTexture(GL_TEXTURE0 + index));
	GLCall(glBindTexture(GL_TEXTURE_2D, *texture)); // 绑定纹理


	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, texColorMode, width, height, 0, resColorMode, GL_UNSIGNED_BYTE, img_data));
	// param1 纹理Target
	// param2 多级渐远级别 0为基本级别
	// param3 纹理格式
	// param4&5 宽度和高度x&y
	// param6 历史遗留 恒为0
	// param7 源数据格式
	// param8 源数据类型
	// param9 源数据

	if (bGenerateMipmap)
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));	// 自动生成多级渐远级别纹理
	stbi_image_free(img_data);						// 释放图像资源
	return true;
	return false;
}

Texture::Texture(const std::string& imgPath, unsigned char index, int texColorMode, int imgColorMode, bool bGenerateMipmap)
{
	this->LoadTexture(&m_uTextureID, imgPath, index, texColorMode, imgColorMode, true);
	m_ubTextureIndex = index;
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));		// 设置环绕方式

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));	// 设置插值方式
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_uTextureID));
}

void Texture::Bind()
{
	GLCall(glActiveTexture(GL_TEXTURE0 + m_ubTextureIndex));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_uTextureID));
}

unsigned char Texture::GetIndex() const
{
	return m_ubTextureIndex;
}
