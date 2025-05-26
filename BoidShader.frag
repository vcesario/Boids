#version 330 core

out vec4 FragColor;

in vec3 VPos;
in vec3 VNormal;

uniform vec3 lightDirection;

void main()
{
	vec3 baseColor = vec3(249.0 / 255, 199.0 / 255.0, 132.0 / 255.0);
	
	vec3 result = baseColor * dot(VNormal, -lightDirection);

	FragColor = vec4(result, 1.0);
}