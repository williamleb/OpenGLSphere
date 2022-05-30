#version 400 core

uniform mat4 view;
uniform vec3 viewPos;
uniform mat4 projection;

in vec4 vPosition;
in vec3 vNormal;

out vec3 fNormal;
out vec3 fEyeVector;
out vec3 fPosition;

void
main()
{
	 fNormal = vNormal;
	 fPosition = vPosition.xyz;
	 vec3 ajustedViewPos = viewPos - fPosition;
	 fEyeVector = vec3(ajustedViewPos.xy,-ajustedViewPos.z);

     gl_Position = projection * view * vec4(vPosition.xy, -vPosition.z, 1);
}

