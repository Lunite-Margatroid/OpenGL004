#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "render.h"
#include "Camera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// 全局变量
float currentTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

glm::mat4 eMat(1.0f);

Camera camera;

int main()
{
	// 初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 初始化窗口
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL004", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fail to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 生成上下文
	glfwMakeContextCurrent(window);

	// 加载opengl函数 要在生成上下文之后
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize glad" << std::endl;
		return false;
	}


	// 开启垂直同步
	glfwSwapInterval(1);

	// 设置视口
	GLCall(glViewport(0, 0, WIDTH, HEIGHT));

	// 捕捉cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 声明回调函数
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	void scroll_callback(GLFWwindow * window, double offsetx, double offsery);
	void mouse_callback(GLFWwindow * window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void ProcessInput(GLFWwindow * window);
	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	
	

	// 变换
	glm::mat4 modelTrans;
	glm::mat4 viewTrans;
	glm::mat4 projectionTrans;

	glm::mat3 normalMat;

	viewTrans = glm::translate(viewTrans, glm::vec3(0.0f, 0.0f, -3.0f));
	projectionTrans = glm::perspective(PI / 4.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	// data
	std::string pathVertexShader(".\\res\\VertexShader.shader");
	std::string pathFragmentShader(".\\res\\FragmentShader.shader");

	glm::vec3 lightPos = glm::vec3(6.0f, 5.0f, 3.0f);
	glm::vec3 objPos = glm::vec3(0.0f, 0.0f, 0.0f);

	float vertice[] =
	{
		// 坐标						颜色
		-0.5f, -0.5f, -0.5f,	0.8f,0.8f,0.8f,
		-0.5f, -0.5f, 0.5f,		0.8f,0.8f,0.8f,
		-0.5f, 0.5f, -0.5f,		0.8f,0.8f,0.8f,
		-0.5f, 0.5f, 0.5f,		0.8f,0.8f,0.8f,

		0.5f, -0.5f, -0.5f,		0.8f,0.8f,0.8f,
		0.5f, -0.5f, 0.5f,		0.8f,0.8f,0.8f,
		0.5f , 0.5f, -0.5f,		0.8f,0.8f,0.8f,
		0.5f, 0.5f, 0.5f,		0.8f,0.8f,0.8f,
	};
	float mikuColor[] = { float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255 };
	glm::vec3 objColor(mikuColor[0], mikuColor[1], mikuColor[2]);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	float objVertice[] =
	{
		// 坐标						法向量					纹理坐标
		-1.0f,	-1.0f,	-1.0f,		-1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		-1.0f,	-1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		-1.0f,	1.0f,	-1.0f,		-1.0f,	0.0f,	0.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,	1.0f,	1.0f,

		1.0f,	-1.0f,	-1.0f,		1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,	1.0f,		1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,	-1.0f,		1.0f,	0.0f,	0.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,	1.0f,	1.0f,

		-1.0f,	-1.0f,	-1.0f,		0.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		-1.0f,	-1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,	-1.0f,		0.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,	1.0f,	1.0f,

		-1.0f,	1.0f,	-1.0f,		0.0f,	1.0f,	0.0f,	0.0f,	0.0f,
		-1.0f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,	-1.0f,		0.0f,	1.0f,	0.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,	1.0f,	1.0f,

		-1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	-1.0f,	0.0f,	0.0f,
		-1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	-1.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	-1.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	-1.0f,	1.0f,	1.0f,

		-1.0f,	-1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	0.0f,	0.0f,
		-1.0f,	1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	1.0f,	0.0f,
		1.0f,	1.0f,	1.0f,		0.0f,	0.0f,	1.0f,	1.0f,	1.0f,
	};

	unsigned int indice[36]
	{
		0, 1, 2,
		1, 2, 3,
	};
	for (int i = 6; i < 36; i++)
	{
		indice[i] = indice[i - 6] + 4;
	}

	unsigned int indiceLight[]
	{
		0, 1, 2,
		1, 2, 3,

		4, 5, 6,
		5, 6, 7,

		0, 1, 4,
		1, 4, 5,

		2, 3, 6,
		3, 6, 7,

		0, 2, 4,
		2, 4, 6,

		1, 3, 5,
		3, 5, 7
	};

	glm::vec3 Pos[10];
	for (int i = 0; i < 10; i++)
	{
		Pos[i] = glm::vec3(float(rand()%200-100)/10.f, 
			float(rand() % 200 - 100) / 10.f, 
			float(rand() % 200 - 100) / 10.f);
	}



	{
		// 加载纹理
		std::string imgPath(".\\res\\img\\container2.png");
		Texture texture1(imgPath, 0, GL_RGB, GL_RGBA);

		std::string imgPath2(".\\res\\img\\container2_specular.png");
		Texture texture2(imgPath2, 1, GL_RGB, GL_RGBA);


		Shader shader(pathVertexShader, pathFragmentShader);
		Shader objShader(".\\res\\VertexShaderPointLight.shader", ".\\res\\SpotLightFragment.shader");

		// 光源方块
		VertexArray va(36);
		va.AddBuffer(8, 6, vertice);
		va.AddElementBuffer(36, indiceLight);
		va.PushAttrib(3);
		va.PushAttrib(3);
		va.ApplyLayout();
		
		// 受光方块
		VertexArray objVa(36);
		objVa.AddBuffer(24, 8, objVertice);
		objVa.AddElementBuffer(36, indice);
		objVa.PushAttrib(3);
		objVa.PushAttrib(3);
		objVa.PushAttrib(2);
		objVa.ApplyLayout();

		void UpdateTimer();
		lastTime = currentTime = glfwGetTime();


		GLCall(glEnable(GL_DEPTH_TEST));					// 深度检测
		GLCall(glClearColor(mikuColor[0] * 0.3f, mikuColor[1] * 0.3f, mikuColor[2] * 0.3f, 1.f));	//清屏颜色
		while (!glfwWindowShouldClose(window))
		{
			glm::vec3 cameraPos;
			glm::vec3 cameraDir;

			// 更新时间变量
			UpdateTimer();

			// 绘制光源方块
			viewTrans = camera.GetViewTrans();
			projectionTrans = camera.GetProjectionTrans();

			shader.Unbind();
			shader.Bind();

			modelTrans = glm::rotate(eMat, deltaTime, glm::vec3(-6.0f, 5.0f, 3.0f));
			lightPos = modelTrans * glm::vec4(lightPos, 1.0f);

			modelTrans = glm::translate(modelTrans, lightPos);
			shader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
			shader.SetUniformMatrix4f("viewTrans", false, glm::value_ptr(viewTrans));
			shader.SetUniformMatrix4f("projectionTrans", false, glm::value_ptr(projectionTrans));

			va.DrawElement();

			// 绘制受光方块
			objShader.Unbind();
			objShader.Bind();
			modelTrans = glm::translate(eMat, objPos);
			normalMat = glm::mat3(transpose(inverse(modelTrans)));
			cameraPos = camera.GetPosition();
			cameraDir = camera.GetDirection();

			objShader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
			objShader.SetUniformMatrix4f("viewTrans", false, glm::value_ptr(viewTrans));
			objShader.SetUniformMatrix4f("projectionTrans", false, glm::value_ptr(projectionTrans));
			objShader.SetUniformMatrix3f("normalMat", false, glm::value_ptr(normalMat));
			objShader.SetUniform3f("light.direction",cameraDir.x, cameraDir.y, cameraDir.z);
			objShader.SetUniform3f("light.position", cameraPos.x, cameraPos.y, cameraPos.z);
			objShader.SetUniform1f("light.outerbdr", cos(PI / 13));
			objShader.SetUniform1f("light.innerbdr", cos(PI / 20));

			objShader.SetUniformTexture("material.diffuse", texture1);
			objShader.SetUniformTexture("material.specular", texture2);
			objShader.SetUniform1f("material.shininess", 40.f);

			objShader.SetUniform3f("light.ambient", 0.4f, 0.4f, 0.4f);
			objShader.SetUniform3f("light.diffuse", lightColor.x, lightColor.y, lightColor.z);
			objShader.SetUniform3f("light.specular", lightColor.x, lightColor.y, lightColor.z);
			

			objShader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

			objVa.DrawElement();

			for (int i = 0; i < 10; i++)
			{
				modelTrans = glm::rotate(eMat,
					PI / 3.0f,
					Pos[i]);
				modelTrans = glm::translate(modelTrans, Pos[i] );
				
				objShader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
				normalMat = glm::mat3(transpose(inverse(modelTrans)));
				objShader.SetUniformMatrix3f("normalMat", false, glm::value_ptr(normalMat));
				objVa.DrawElement();
			}

			// 处理键盘控制输入
			ProcessInput(window);
			// 交换缓冲
			glfwSwapBuffers(window);
			// 清除颜色和深度检测缓冲
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));	
			// 检查触发事件
			glfwPollEvents();
		}
	
	
	}


	glfwTerminate();
	return 0;
}

// 回调函数 改变窗口大小和位置时 重新设置视口
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	GLCall(glViewport(0, 0, WIDTH, HEIGHT));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = WIDTH / 2;
	static float lastY = HEIGHT / 2;

	if (firstMouse)
	{
		firstMouse = false;
		lastX = xpos;
		lastY = ypos;
	}

	float offsetX = xpos - lastX;
	float offsetY = lastY - ypos;

	camera.ProcessMouse(offsetX, offsetY);

	lastX = xpos;
	lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double offsetx, double offsety)
{
	camera.ProcessScroll(offsety);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

}

// 处理键盘交互
void ProcessInput(GLFWwindow* window)
{
	// 检测esc按下，则关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(FRONT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(BACK, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(UP, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.ProcessKeyInput(DOWN, deltaTime);
	}
}

void UpdateTimer()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}