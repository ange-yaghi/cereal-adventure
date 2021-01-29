#version 420

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_Tex;
layout(location=2) in vec4 in_Normal;

out vec4 ex_Pos;
out vec4 ex_ShadowMap0;
out vec2 ex_Tex;
out vec3 ex_Normal;

layout (binding = 0) uniform ScreenVariables {
	mat4 CameraView;
	mat4 Projection;

	mat4 ShadowMap0View;
	mat4 ShadowMap0Projection;

	vec4 CameraEye;
	
	vec4 FogColor;
	float FogNear;
	float FogFar;
};

layout (binding = 1) uniform ObjectVariables {
	mat4 Transform;
	vec2 TexOffset;
	vec2 TexScale;
	vec4 Scale;

	vec4 BaseColor;
	vec4 Emission;
	float SpecularMix;
	float DiffuseMix;
	float Metallic;
	float DiffuseRoughness;
	float SpecularPower;
	float IncidentSpecular;
	float FogEffect;
	
	int ColorReplace;
	int AoMap;
	int Lit;
};

void main(void) {
	vec4 inputPos = vec4(in_Position.xyz * Scale.xyz, 1.0);

	inputPos = inputPos * Transform;
	ex_Pos = inputPos;

	ex_ShadowMap0 = inputPos;
	ex_ShadowMap0 = ex_ShadowMap0 * ShadowMap0View;
	ex_ShadowMap0 = ex_ShadowMap0 * ShadowMap0Projection;

	inputPos = inputPos * CameraView;
	inputPos = inputPos * Projection;

	vec4 finalNormal = vec4(in_Normal.xyz, 0.0);
	ex_Normal = vec3(finalNormal * Transform);

	gl_Position = vec4(inputPos.xyzw);

	ex_Tex = in_Tex;
}
