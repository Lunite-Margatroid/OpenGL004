#version 330 core

in vec3 normal;				// ���Զ������Եķ�����
in vec3 fragPos;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform mat3 normalMat;		// ���߾��� �޸��ǵȱ�������ɵ��α�
uniform vec3 viewPos;		// �۲���λ�� ���ڼ��㷴���

void main()
{
	float ambientStrength = 0.1;
	float lightStrength = 0.7;
	float specularStrength = 0.5;
	// ���㷨��������λ��
	vec3 norm = normal * normalMat;
	norm = normalize(norm);

	// ���߷���
	vec3 lightDir = normalize(vec3(lightPos) - fragPos);

	// ���㻷����
	vec3 ambient = vec3(1.0,1.0,1.0) * ambientStrength;

	// ����ɢ���
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor * lightStrength;

	// ���㷴���
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow( max( dot(reflectDir, viewDir), 0.0), 40);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objColor;
	FragColor = vec4(result, 1.0);
}