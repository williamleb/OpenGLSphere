#version 400 core

uniform vec3 uAmbiantColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uSpecularExponent;
uniform bool phong;

uniform vec3 uLightPosition;
uniform vec4 uLightColor;


in vec3 fNormal;
in vec3 fEyeVector;
in vec3 fPosition;

out vec4 fColor;

float distanceSquared(vec3 left, vec3 right)
{
    vec3 direction = left - right;
    return dot(direction, direction);
}

void main()
{
	vec3 lightPosition = vec3(uLightPosition.xy, uLightPosition.z);

    vec3 lightDirection = normalize(lightPosition - fPosition);
    vec3 nfNormal = normalize(fNormal);

	vec3 nfEyeVector = normalize(fEyeVector);

    float diffuse = max(0, dot(nfNormal, lightDirection));
	float specular = 0;

	if (diffuse > 0 && uSpecularExponent > 0) {
		if (phong)
		{
			vec3 reflectedVector = reflect(-lightDirection, nfNormal);
			specular = pow(max(0.0, dot(nfEyeVector, reflectedVector)),uSpecularExponent);
		}
		else
		{
			vec3 halfwayDir = normalize(lightDirection + nfEyeVector);
			specular = pow(max(dot(nfNormal,halfwayDir),0.0),uSpecularExponent);
		}

	}

	vec4 ambiantContribution = vec4(uAmbiantColor, 1);
	vec4 diffuseContribution = vec4(uDiffuseColor, 1) * diffuse;
	vec4 specularContribution = vec4(uSpecularColor, 1) * specular;

	fColor = ambiantContribution + uLightColor * (diffuseContribution + specularContribution) * (1 / distanceSquared(fPosition, lightPosition));
}
