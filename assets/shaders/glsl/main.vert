#version 420

struct ShadowMapInput {
	mat4 CameraView;
	mat4 Projection;
};

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_Tex;
layout(location=2) in vec4 in_Normal;

out vec4 ex_Pos;
out vec4 ex_ShadowMapPos[8];
out vec2 ex_Tex;
out vec4 ex_ScreenSpace;
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

	int SsaoEnable;
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

layout (binding = 3) uniform ShadowMapVariables {
	ShadowMapInput ShadowMapInputs[8];
};

void main(void) {
	vec4 inputPos = vec4(in_Position.xyz * Scale.xyz, 1.0);

	inputPos = inputPos * Transform;
	ex_Pos = inputPos;

	for (int i = 0; i < 8; ++i) {
		ex_ShadowMapPos[i] = inputPos;
		ex_ShadowMapPos[i] = ex_ShadowMapPos[i] * ShadowMapInputs[i].CameraView;
		ex_ShadowMapPos[i] = ex_ShadowMapPos[i] * ShadowMapInputs[i].Projection;
	}

	inputPos = inputPos * CameraView;
	inputPos = inputPos * Projection;

	vec4 finalNormal = vec4(in_Normal.xyz, 0.0);
	ex_Normal = vec3(finalNormal * Transform);

	gl_Position = inputPos;
	ex_ScreenSpace = inputPos;

	ex_Tex = in_Tex;
}
