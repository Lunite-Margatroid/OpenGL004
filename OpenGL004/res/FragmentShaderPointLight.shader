#version 330 core

// ����struct
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

// ����
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// ˥��
	float kConstant;		// ������
	float kLinear;			// һ����
	float kQuadratic;		// 2����
};

in vec3 normal;				// ���Զ������Եķ�����
in vec3 fragPos;			// Ƭ������
in vec2 TexCoord;			// ��������

out vec4 FragColor;			// outputƬ����ɫ

uniform Material material;	// ����
uniform Light light;		// ��Դ

uniform mat3 normalMat;		// ���߾��� �޸��ǵȱ�������ɵ��α�
uniform vec3 viewPos;		// �۲���λ�� ���ڼ��㷴���

void main()
{
	// ���㷨��������λ��
	vec3 norm = normalMat * normal;
	norm = normalize(norm);

	// ���߷���&��Դ����
	vec3 lightDir = light.position - fragPos;
	float lightDst = length(lightDir);
	lightDir = normalize(lightDir);


	// ���㻷����
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));	// ����Ļ����ⷴ�����������ɫһ��

	// ����ɢ���
	float diff = max(dot(lightDir, norm), 0.0);	// �������ǿ��ϵ��
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	// ���㷴���
	vec3 reflectDir = reflect(-lightDir, norm);					// ���������
	vec3 viewDir = normalize(viewPos - fragPos);				// �۲췽��
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);	// ����ǿ��
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular;	// �������ɫ

	// ����&Ӧ��˥��
	float attenuation = 1.0 / (light.kConstant + 
		light.kLinear * lightDst + light.kQuadratic * lightDst * lightDst);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}