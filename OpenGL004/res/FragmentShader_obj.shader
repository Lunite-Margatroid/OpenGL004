#version 330 core

// ����struct
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

// ����
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;				// ���Զ������Եķ�����
in vec3 fragPos;
in vec2 TexCoord;			// ��������

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform mat3 normalMat;		// ���߾��� �޸��ǵȱ�������ɵ��α�
uniform vec3 viewPos;		// �۲���λ�� ���ڼ��㷴���

void main()
{
	// ���㷨��������λ��
	vec3 norm = normal * normalMat;
	norm = normalize(norm);

	// ���߷���
	vec3 lightDir = normalize(light.position - fragPos);

	// ���㻷����
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));	// ����Ļ����ⷴ�����������ɫһ��

	// ����ɢ���
	float diff = max(dot(lightDir, norm), 0.0);	// �������ǿ��ϵ��
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoord));

	// ���㷴���
	vec3 reflectDir = reflect(-lightDir, norm);					// ���������
	vec3 viewDir = normalize(viewPos - fragPos);				// �۲췽��
	float spec = pow( max( dot(reflectDir, viewDir), 0.0), material.shininess);	// ����ǿ��
	vec3 specular = material.specular * spec * light.specular;	// �������ɫ

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}