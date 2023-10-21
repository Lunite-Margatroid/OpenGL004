#pragma once

class Texture
{
private:
	unsigned int m_uTextureID;
	unsigned char m_ubTextureIndex;

/// <summary>
/// 加载纹理
/// </summary>
/// <param name="texture">纹理对象id</param>
/// <param name="path">纹理图片源文件</param>
/// <param name="index">纹理索引</param>
/// <param name= "texColorMode">纹理颜色模式</param>
/// <param name= "resColorMode">源图片颜色模式</param>
/// <param name="bGenerateMipmap">是否自动生成多级渐远纹理</param>
/// <returns></returns>
	bool LoadTexture(unsigned int* texture, const std::string& path, unsigned char index,
		int texColorMode, int resColorMode, bool bGenerateMipmap);
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="imgPath">纹理图片路径</param>
	/// <param name="index">纹理索引 从0开始，最大15</param>
	/// <param name="texColorMode">纹理的颜色模式，一般为GL_RGB或GL_RGBA。</param>
	/// <param name="imgColorMode">图片资源的颜色模式，同上</param>
	/// <param name="bGenerateMipmap">是否生成多级渐远纹理，默认为true</param>
	Texture(const std::string& imgPath, unsigned char index, int texColorMode, 
		int imgColorMode, bool bGenerateMipmap = true);
	~Texture();

	void Bind();

	unsigned char GetIndex()const;

};

