#version 330 core

out vec4 FragColor;

in vec3 VPos;
in vec3 VNormal;

uniform vec3 lightDirection;
uniform vec3 lightDiffuse;

void main()
{
	vec3 baseColor = vec3(249.0 / 255, 199.0 / 255.0, 132.0 / 255.0);
	
	vec3 result = vec3(0.0);

	float diff = max(dot(VNormal, -lightDirection), 0.0);
	
	result = baseColor * lightDiffuse * diff;

	FragColor = vec4(result, 1.0);
}