#pragma once


#ifndef PI
#define PI 3.14159f
#endif


enum Camera_Movement
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// 初始值
const float YAW = -PI/2.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SCROLL_SENSITIVITY = 0.02f;
const float ROTATE_SENEITIVITY = 0.005f;
const float FOV = PI / 4.0f;

class Camera
{
private:
	glm::vec3 m_position;		// 摄像机的位置在世界坐标的向量
	glm::vec3 m_front;			// 摄像机的朝向（z轴的负方向）在世界坐标中的方向
	glm::vec3 m_up;				// 摄像机坐标的y轴正方向在世界坐标的方向
	glm::vec3 m_right;			// 摄像机坐标x轴正方向在世界坐标的方向
	glm::vec3 m_worldUp;		// 不要老想着坐标 这是个抽象向量

	float m_yaw;				// 转轴为y轴 左右
	float m_pitch;				// 转轴为x轴 上下
		
	float m_fov;				// 转轴为z轴 Roll

	float m_moveSpeed;
	float m_scrollSensitivity;
	float m_rotateSensitivity;

	void UpdateViewTrans();
	void UpdateProjectionTrans();
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH,
		float fov = FOV
		);

	void SetMoveSpeed(float moveSpeed);
	void SetScrollSensitivity(float sensitivity);
	void SetRotateSensitivity(float sensitivity);
	void SetFov(float fov);

	glm::mat4 GetViewTrans();
	glm::mat4 GetProjectionTrans();

	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;

	void ProcessKeyInput(Camera_Movement diration, float deltaTime);
	void ProcessMouse(float offsetx, float offsety);
	void ProcessScroll(float offsety);

};

