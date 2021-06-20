#version 420

layout(binding = 0) uniform sampler2D Input;
layout(binding = 1) uniform sampler2D InputBloom;
layout(binding = 2) uniform sampler2D Dither;

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;

layout (binding = 1) uniform BloomSettings {
	float BloomAmount;
	float DitherAmount;
	float DebugBlackAndWhite;
};


float linearToSrgb(float u) {
	const float MinSrgbPower = 0.0031308;

	if (u < MinSrgbPower) {
		return 12.92 * u;
	}
	else {
		return 1.055 * pow(u, 1 / 2.4) - 0.055;
	}
}

vec3 linearToSrgb(vec3 v) {
	return vec3(
		linearToSrgb(v.r),
		linearToSrgb(v.g),
		linearToSrgb(v.b)
	);
}

vec3 hableTonemapPartial(vec3 s) {
	const float A = 0.15f;
	const float B = 0.50f;
	const float C = 0.10f;
	const float D = 0.20f;
	const float E = 0.02f;
	const float F = 0.30f;

	return ((s * (A * s + C * B) + D * E) / (s * (A * s + B) + D * F)) - E/F;
}

vec3 hableTonemap(vec3 input) {
	const float exposureBias = 2.0f;
	const vec3 mapped = hableTonemapPartial(exposureBias * input);

	const vec3 W = vec3(11.3f);
	const vec3 whiteScale = vec3(1.0f) / hableTonemapPartial(W);
	return mapped * whiteScale;
}

vec3 orderedDither(vec3 c) {
	const int x = int(gl_FragCoord.x);
	const int y = int(gl_FragCoord.y);

	const int size = textureSize(Dither, 0).x;

	const float M = texture(Dither, vec2(mod(x, size) / size, mod(y, size) / size)).r;

	const float dither = mix(0.05, 2.0, DebugBlackAndWhite) * (M - 0.5);
	return c + vec3(dither, dither, dither);
}

vec3 toBlackAndWhite(vec3 c) {
	const float brightness = dot(c.rgb, vec3(0.2126, 0.7152, 0.0722));
	const float bw = round(brightness);
	return vec3(bw, bw, bw);
}

void main(void) {
    const vec3 input = texture(Input, ex_Tex).rgb;
    const vec3 bloom = texture(InputBloom, ex_Tex).rgb;

	const vec3 final = input + bloom * BloomAmount;
	const vec3 tonemapped = hableTonemap(final);
	const vec3 srgb = linearToSrgb(tonemapped);
	const vec3 dithered = mix(srgb, orderedDither(srgb), DitherAmount);
	const vec3 filtered = mix(dithered, toBlackAndWhite(dithered), DebugBlackAndWhite);
    
    out_Color = vec4(filtered, 1.0);	
}
