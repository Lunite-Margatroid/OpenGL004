#version 330 core

// 材质struct
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

// 光照  平行光（定向光）
struct Light
{
	vec3 direction;			// 光照方向

	vec3 ambient;			// 环境光
	vec3 diffuse;			// 漫反射
	vec3 specular;			// 镜面反射
};

in vec3 normal;				// 来自顶点属性的法向量
in vec3 fragPos;
in vec2 TexCoord;			// 纹理坐标

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform mat3 normalMat;		// 法线矩阵 修复非等比缩放造成的形变
uniform vec3 viewPos;		// 观察者位置 用于计算反射光

void main()
{
	// 计算法向量并单位化
	vec3 norm = normalMat * normal;
	norm = normalize(norm);

	// 光线方向
	vec3 lightDir = normalize(-light.direction);

	// 计算环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));	// 物体的环境光反射和漫反射颜色一样

	// 计算散射光
	float diff = max(dot(lightDir, norm), 0.0);	// 漫反射光强度系数
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	// 计算反射光
	vec3 reflectDir = reflect(-lightDir, norm);					// 反射光向量
	vec3 viewDir = normalize(viewPos - fragPos);				// 观察方向
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);	// 反射强度
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular;	// 反射光颜色

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}