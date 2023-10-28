#version 330 core
// 多光源
// 材质struct
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

// 光照
// 定向光
struct DirLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
};

// 点光源
struct PointLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;

	float kConstant;		// 常数项
	float kLinear;			// 一次项
	float kQuadratic;		// 2次项
};


// 聚光
struct SpotLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 direction;
	
	// 渐变边界
	float innerbdr;
	float outerbdr;

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

uniform DirLight dirLight;						// 定向光源
#define N_POINTLIGHT 4
uniform PointLight pointLight[N_POINTLIGHT];	// 点光源
uniform SpotLight spotLight;					// 聚光
uniform mat3 normalMat;							// 法线矩阵 修复非等比缩放造成的形变
uniform vec3 viewPos;							// 观察者位置 用于计算反射光

vec3 ProcDirectionLight(DirLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir);
vec3 ProcPointLight(PointLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir);
vec3 ProcSpotLight(SpotLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir);
void main()
{
	// 计算法向量并单位化
	vec3 norm = normalMat * normal;
	norm = normalize(norm);
	// 计算片段材质
	vec3 texDiff = vec3(texture(material.diffuse, TexCoord));
	vec3 texSpec = vec3(texture(material.specular, TexCoord));
	// 从片段指向观察者
	vec3 viewDir = normalize(fragPos-viewPos);

	vec3 result = vec3(0.0, 0.0, 0.0);

	// 定向光
	result += ProcDirectionLight(dirLight, texDiff, texSpec, norm, viewDir);

	// 点光源
	int i = 0;
	for (; i < N_POINTLIGHT; i++)
	{
		result += ProcPointLight(pointLight[i], texDiff, texSpec, norm, viewDir);
	}

	// 聚光
	result += ProcSpotLight(spotLight,texDiff,texSpec, norm, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 ProcDirectionLight(DirLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir)
{
	vec3 output;
	// 光线方向
	vec3 lightDir = normalize(-light.direction);

	// 计算环境光
	vec3 ambient = light.ambient * texDiff;	// 物体的环境光反射和漫反射颜色一样

	// 计算散射光
	float diff = max(dot(lightDir, normalVec), 0.0);	// 漫反射光强度系数
	vec3 diffuse = diff * light.diffuse * texDiff;

	// 计算反射光
	vec3 reflectDir = reflect(-lightDir, normalVec);					// 反射光向量
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);	// 反射强度
	vec3 specular = texSpec * spec * light.specular;	// 反射光颜色

	output = ambient + diffuse + specular;
	return output;
}

vec3 ProcPointLight(PointLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir)
{
	// 计算环境光
	vec3 ambient = light.ambient * texDiff;

	// 计算漫反射光
	vec3 lightDir = fragPos - light.position;
	float lightDst = length(lightDir);
	lightDir = normalize(lightDir);
	float diff = max(dot(normalVec , lightDir),0.0f);
	vec3 diffuse = light.diffuse * texDiff * diff;

	// 计算镜面反射
	vec3 reflectDir = reflect(-lightDir, normalVec);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * texSpec * spec;

	// 计算衰减
	float attenuation = 1.0 / 
		(light.kConstant + light.kLinear * lightDst + light.kQuadratic * lightDst * lightDst);

	vec3 output = (ambient + diffuse + specular) * attenuation;
	return output;
}

vec3 ProcSpotLight(SpotLight light, vec3 texDiff, vec3 texSpec, vec3 normalVec, vec3 viewDir)
{
	vec3 output;
	// 光线方向
	vec3 lightDir = light.position - fragPos;
	float lightDst = length(lightDir);
	lightDir = normalize(lightDir);

	// 计算片段是否在光照范围内
	float theta = dot(lightDir, -normalize(light.direction));

	// 计算环境光
	vec3 ambient = light.ambient * texDiff;

	// 片段在光照范围外 仅计算环境光
	output = ambient;

	if (theta >= light.outerbdr)
	{// 片段在光照范围内
		// 计算散射光
		float diff = max(dot(lightDir, normalVec), 0.0);					// 漫反射光强度系数
		vec3 diffuse = diff * light.diffuse * texDiff;

		// 计算反射光
		vec3 reflectDir = reflect(-lightDir, normalVec);					// 反射光向量
		float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);	// 反射强度
		vec3 specular = texSpec * spec * light.specular;	// 反射光颜色

		// 片段在过渡带
		if (theta < light.innerbdr)
		{
			float attenuation = (theta - light.outerbdr) / (light.innerbdr - light.outerbdr);
			diffuse *= attenuation;
			specular *= attenuation;
		}
		output += diffuse + specular;
	}

	float attenuation = 1.0 /
		(light.kConstant + light.kLinear * lightDst + light.kQuadratic * lightDst * lightDst);
	output *= attenuation;

	return output;
}