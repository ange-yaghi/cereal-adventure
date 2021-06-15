#version 420

const int MAX_KERNEL_SIZE = 128;

layout(binding = 0) uniform sampler2D DepthMap;
layout(binding = 1) uniform SSAO_Input {
    mat4 Projection;
    vec3 Kernel[MAX_KERNEL_SIZE];
    float SampleRadius;
};

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;

float ndc_to_vs_z(float ndc_z) {
    return Projection[2][3] / (ndc_z - Projection[2][2]);
}

void main(void) {
	const float depth = texture(DepthMap, ex_Tex).x;
    const float ndc_depth = 2 * depth - 1;
    const float vs_depth = ndc_to_vs_z(ndc_depth);

    vec3 position;
    position.x = (ex_Pos.x * vs_depth) / Projection[0][0];
    position.y = (ex_Pos.y * vs_depth) / Projection[1][1];
    position.z = vs_depth;

    float ambientOcclusion = 0.0;
    vec3 average = vec3(0, 0, 0);
    for (int i = 0; i < MAX_KERNEL_SIZE; ++i) {
        const vec3 samplePosition = position + Kernel[i] * 0.2;

        vec4 offset = vec4(samplePosition, 1.0);
        offset = offset * Projection;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + vec2(0.5);

        offset.x = clamp(offset.x, 0, 1);
        offset.y = clamp(offset.y, 0, 1);

        const float sampleDepth = ndc_to_vs_z(2 * texture(DepthMap, offset.xy).x - 1);
        if (abs(samplePosition.z - sampleDepth) < SampleRadius) {
            ambientOcclusion += step(sampleDepth, samplePosition.z);
        }
    }

    ambientOcclusion = 1.0 - ambientOcclusion / 128.0;

    out_Color = vec4(ambientOcclusion);
}
