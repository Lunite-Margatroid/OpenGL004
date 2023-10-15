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

// ȫ�ֱ���
float currentTime = 0.0f;
float deltaTime = 0.0f;
float lastTime = 0.0f;

glm::mat4 eMat(1.0f);

Camera camera;

int main()
{
	// ��ʼ��glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��ʼ������
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL004", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fail to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// ����������
	glfwMakeContextCurrent(window);

	// ����opengl���� Ҫ������������֮��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize glad" << std::endl;
		return false;
	}


	// ������ֱͬ��
	glfwSwapInterval(1);

	// �����ӿ�
	GLCall(glViewport(0, 0, WIDTH, HEIGHT));

	// ��׽cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// �����ص�����
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);
	void scroll_callback(GLFWwindow * window, double offsetx, double offsery);
	void mouse_callback(GLFWwindow * window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	void ProcessInput(GLFWwindow * window);
	// ע��ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	
	GLCall(glEnable(GL_DEPTH_TEST));					// ��ȼ��
	GLCall(glClearColor(0.11f, 0.385f, 0.367f, 1.f));	//������ɫ

	// �任
	glm::mat4 modelTrans(1.f);
	glm::mat4 viewTrans(1.f);
	glm::mat4 projectionTrans(1.f);

	viewTrans = glm::translate(viewTrans, glm::vec3(0.0f, 0.0f, -3.0f));
	projectionTrans = glm::perspective(PI / 4.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	// data
	std::string pathVertexShader(".\\res\\VertexShader.shader");
	std::string pathFragmentShader(".\\res\\FragmentShader.shader");

	glm::vec3 lightPos = glm::vec3(6.0f, 5.0f, 3.0f);
	glm::vec3 objPos = glm::vec3(0.0f, 0.0f, 0.0f);

	float vertice[] =
	{
		// ����						��ɫ
		-0.5f, -0.5f, -0.5f,	0.8f,0.8f,0.8f,
		-0.5f, -0.5f, 0.5f,		0.8f,0.8f,0.8f,
		-0.5f, 0.5f, -0.5f,		0.8f,0.8f,0.8f,
		-0.5f, 0.5f, 0.5f,		0.8f,0.8f,0.8f,

		0.5f, -0.5f, -0.5f,		0.8f,0.8f,0.8f,
		0.5f, -0.5f, 0.5f,		0.8f,0.8f,0.8f,
		0.5f , 0.5f, -0.5f,		0.8f,0.8f,0.8f,
		0.5f, 0.5f, 0.5f,		0.8f,0.8f,0.8f,
	};

	float objVertice[] =
	{
		// ����						��ɫ
		-1.0f, -1.0f, -1.0f,	float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		-1.0f, -1.0f, 1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		-1.0f, 1.0f, -1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		-1.0f, 1.0f, 1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,

		1.0f, -1.0f, -1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		1.0f, -1.0f, 1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		1.0f , 1.0f, -1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
		1.0f, 1.0f, 1.0f,		float(0x39) / 255,float(0xc5) / 255,float(0xbb) / 255,
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


	{
		Shader shader(pathVertexShader, pathFragmentShader);
		Shader objShader(".\\res\\VertexShader_obj.shader", ".\\res\\FragmentShader_obj.shader");

		VertexArray va(36);
		va.AddBuffer(8, 6, vertice);
		va.AddElementBuffer(36, indice);
		va.PushAttrib(3);
		va.PushAttrib(3);
		va.ApplyLayout();
		

		VertexArray objVa(36);
		objVa.AddBuffer(8, 6, objVertice);
		objVa.AddElementBuffer(36, indice);
		objVa.PushAttrib(3);
		objVa.PushAttrib(3);
		objVa.ApplyLayout();


		void UpdateTimer();
		lastTime = currentTime = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			// ����ʱ�����
			UpdateTimer();

			viewTrans = camera.GetViewTrans();
			projectionTrans = camera.GetProjectionTrans();

			shader.Unbind();
			shader.Bind();

			modelTrans = glm::translate(eMat, lightPos);
			shader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
			shader.SetUniformMatrix4f("viewTrans", false, glm::value_ptr(viewTrans));
			shader.SetUniformMatrix4f("projectionTrans", false, glm::value_ptr(projectionTrans));

			va.DrawElement();

			objShader.Unbind();
			objShader.Bind();
			modelTrans = glm::translate(eMat, objPos);
			objShader.SetUniformMatrix4f("modelTrans", false, glm::value_ptr(modelTrans));
			objShader.SetUniformMatrix4f("viewTrans", false, glm::value_ptr(viewTrans));
			objShader.SetUniformMatrix4f("projectionTrans", false, glm::value_ptr(projectionTrans));
			objVa.DrawElement();
			

			// ������̿�������
			ProcessInput(window);
			// ��������
			glfwSwapBuffers(window);
			// �����ɫ����ȼ�⻺��
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));	
			// ��鴥���¼�
			glfwPollEvents();
		}
	
	
	}


	glfwTerminate();
	return 0;
}

// �ص����� �ı䴰�ڴ�С��λ��ʱ ���������ӿ�
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

// ������̽���
void ProcessInput(GLFWwindow* window)
{
	// ���esc���£���رմ���
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