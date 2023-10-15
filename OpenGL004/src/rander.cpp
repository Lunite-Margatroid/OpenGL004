#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>


#include "render.h"


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " <<
			function << " file: " << file << "  line: " << line << std::endl;
		return false;
	}
	return true;
}

/// <summary>
/// ��������
/// </summary>
/// <param name="texture">�������id</param>
/// <param name="path">����ͼƬԴ�ļ�</param>
/// <param name="index">��������</param>
/// <param name= "texColorMode">������ɫģʽ</param>
/// <param name= "resColorMode">ԴͼƬ��ɫģʽ</param>
/// <param name="bGenerateMipmap">�Ƿ��Զ����ɶ༶��Զ����</param>
/// <returns></returns>
bool LoadTexture(unsigned int* texture, const std::string& path, unsigned char index, int texColorMode, int resColorMode, bool bGenerateMipmap)
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
	GLCall(glBindTexture(GL_TEXTURE_2D, *texture)); // ������


	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, texColorMode, width, height, 0, resColorMode, GL_UNSIGNED_BYTE, img_data));
	// param1 ����Target
	// param2 �༶��Զ���� 0Ϊ��������
	// param3 �����ʽ
	// param4&5 ��Ⱥ͸߶�x&y
	// param6 ��ʷ���� ��Ϊ0
	// param7 Դ���ݸ�ʽ
	// param8 Դ��������
	// param9 Դ����

	if (bGenerateMipmap)
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));	// �Զ����ɶ༶��Զ��������
	stbi_image_free(img_data);						// �ͷ�ͼ����Դ
	return true;
}


