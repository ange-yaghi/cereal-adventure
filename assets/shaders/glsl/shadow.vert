#version 420

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_Tex;
layout(location=2) in vec4 in_Normal;

layout (binding = 0) uniform ScreenVariables {
	mat4 CameraView;
	mat4 Projection;
};

layout (binding = 1) uniform ObjectVariables {
	mat4 Transform;
    vec4 Scale;
};

void main(void) {
	vec4 inputPos = vec4(in_Position.xyz * Scale.xyz, 1.0);
	inputPos = inputPos * Transform;
	inputPos = inputPos * CameraView;
	inputPos = inputPos * Projection;

	gl_Position = vec4(inputPos.xyzw);
}
