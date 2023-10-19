#version 330 core

in vec3 normal;				// 来自顶点属性的法向量
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform mat3 normalMat;		// 法线矩阵 修复非等比缩放造成的形变
uniform vec3 viewPos;		// 观察者位置 用于计算反射光

void main()
{
	float ambientStrength = 0.1;
	float lightStrength = 0.7;
	float specularStrength = 0.5;
	// 计算法向量并单位化
	vec3 norm = normal * normalMat;
	norm = normalize(norm);

	// 光线方向
	vec3 lightDir = normalize(vec3(lightPos) - fragPos);

	// 计算环境光
	vec3 ambient = vec3(1.0,1.0,1.0) * ambientStrength;

	// 计算散射光
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor * lightStrength;

	// 计算反射光
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow( max( dot(reflectDir, viewDir), 0.0), 40);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objColor;
	FragColor = vec4(result, 1.0);
}