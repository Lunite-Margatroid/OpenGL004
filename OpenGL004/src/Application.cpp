#ifndef STB_IAMGE_IMPLEMENTATION
#define STB_IAMGE_IMPLEMENTATION
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

// 全局变量
float currentTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

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

	// 开启垂直同步
	glfwSwapInterval(1);

	// 设置视口
	GLCall(glViewport(0, 0, WIDTH, HEIGHT));

	// 声明回调函数
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	void scroll_callback(GLFWwindow * window, double offsetx, double offsery);
	void mouse_callback(GLFWwindow * window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	// 注册回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	
	GLCall(glEnable(GL_DEPTH_TEST));					// 深度检测
	GLCall(glClearColor(0.11, 0.385f, 0.367f, 1.f));	//清屏颜色

	// 变换
	glm::mat4 modelTrans = glm::rotate(modelTrans, -PI / 3.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 viewTrans = glm::translate(viewTrans, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projectionTrans = glm::perspective(PI / 4.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	// data
	std::string pathVertexShader(".\\res\\VertexShader.shader");
	std::string pathFragmentShader(".\\res\\FragmentShader.shader");

	float vertice[] =
	{
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f , 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
	};

	unsigned int indice[]
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

	glm::vec3 color(0.9f, 0.9f, 0.9f);

	{
		Shader shader(pathVertexShader, pathFragmentShader);
		VertexArray va(36);
		va.AddBuffer(8, 3, vertice);
		va.AddElementBuffer(36, indice);
		va.AddVertexAttrib(0, 3, 3, 0);
		while (!glfwWindowShouldClose(window))
		{
			viewTrans = camera.GetViewTrans();
			projectionTrans = camera.GetViewTrans();
			shader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
			shader.SetUniformMatrix4f("viewTrans", false, glm::value_ptr(viewTrans));
			shader.SetUniformMatrix4f("projectionTrans", false, glm::value_ptr(projectionTrans));

			va.DrawElement();

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
	if (action == GLFW_PRESS)
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_W:
			camera.ProcessKeyInput(FRONT, deltaTime);
			break;
		case GLFW_KEY_S:
			camera.ProcessKeyInput(BACK, deltaTime);
			break;
		case GLFW_KEY_A:
			camera.ProcessKeyInput(LEFT, deltaTime);
			break;
		case GLFW_KEY_D:
			camera.ProcessKeyInput(RIGHT, deltaTime);
			break;
		case GLFW_KEY_SPACE:
			camera.ProcessKeyInput(UP, deltaTime);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			camera.ProcessKeyInput(DOWN, deltaTime);
			break;
		default:break;
		};
}