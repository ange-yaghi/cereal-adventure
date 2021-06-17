#version 420

layout(binding = 0) uniform sampler2D Input;

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;

layout(binding = 1) uniform BlurInput {
    int Horizontal;
};

void main(void) {
    const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    vec2 texelOffset = 1.0 / textureSize(Input, 0);
    vec3 result = texture(Input, ex_Tex).rgb * weight[0];

    if (Horizontal == 1) {
        for (int i = 1; i < 5; ++i) {
            result += texture(Input, ex_Tex + vec2(texelOffset.x * i, 0.0)).rgb * weight[i];
            result += texture(Input, ex_Tex - vec2(texelOffset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else {
        for (int i = 1; i < 5; ++i) {
            result += texture(Input, ex_Tex + vec2(0.0, texelOffset.y * i)).rgb * weight[i];
            result += texture(Input, ex_Tex - vec2(0.0, texelOffset.y * i)).rgb * weight[i];
        }
    }

    out_Color = vec4(result, 1.0);	
}
