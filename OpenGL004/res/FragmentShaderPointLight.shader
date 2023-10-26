#version 330 core

// 材质struct
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

// 光照
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 衰减
	float kConstant;		// 常数项
	float kLinear;			// 一次项
	float kQuadratic;		// 2次项
};

in vec3 normal;				// 来自顶点属性的法向量
in vec3 fragPos;			// 片段坐标
in vec2 TexCoord;			// 纹理坐标

out vec4 FragColor;			// output片段颜色

uniform Material material;	// 材质
uniform Light light;		// 光源

uniform mat3 normalMat;		// 法线矩阵 修复非等比缩放造成的形变
uniform vec3 viewPos;		// 观察者位置 用于计算反射光

void main()
{
	// 计算法向量并单位化
	vec3 norm = normalMat * normal;
	norm = normalize(norm);

	// 光线方向&光源距离
	vec3 lightDir = light.position - fragPos;
	float lightDst = length(lightDir);
	lightDir = normalize(lightDir);


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

	// 计算&应用衰减
	float attenuation = 1.0 / (light.kConstant + 
		light.kLinear * lightDst + light.kQuadratic * lightDst * lightDst);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}