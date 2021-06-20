#version 420

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D aoTex;
layout(binding = 2) uniform sampler2D ssao;
layout(binding = 3) uniform sampler2D shadowMaps[8];

layout (location = 0) out vec4 out_Color;
layout (location = 1) out vec4 out_BrightColor;

in vec4 ex_Pos;
in vec4 ex_ShadowMapPos[8];
in vec2 ex_Tex;
in vec4 ex_ScreenSpace;
in vec3 ex_Normal;

struct Light {
	vec4 Position;
	vec4 Color;
	vec4 Direction;
	float Attenuation0;
	float Attenuation1;
	float FalloffStart;
	float FalloffScale;
	int FalloffEnabled;
	int Active;
	int ShadowMap;
};

layout (binding = 0) uniform ScreenVariables {
	mat4 CameraView;
	mat4 Projection;

	mat4 ShadowMap0View;
	mat4 ShadowMap0Projection;

	vec4 CameraEye;
	
	vec4 FogColor;
	float FogNear;
	float FogFar;

	float AmbientDiffuseAmount;
	float AmbientSpecularAmount;
	float SsaoAmount;
	float BakedAoAmount;
	float DiffuseAmount;
	float SpecularAmount;
	float ShadowAmount;
	float DebugSsao;
	float DebugBloom;
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

layout (binding = 2) uniform Lighting {
	Light Lights[32];
	vec4 AmbientLighting;
};

float pow5(float v) {
	return (v * v) * (v * v) * v;
}

float f_diffuse(vec3 i, vec3 o, vec3 h, vec3 normal, float power, float roughness) {
	float h_dot_i = dot(h, i);
	float h_dot_i_2 = h_dot_i * h_dot_i;
	float f_d90 = 0.5 + 2 * h_dot_i_2 * roughness;

	float cos_theta_i = dot(i, normal);
	float cos_theta_o = dot(o, normal);

	float f_d = (1 + (f_d90 - 1) * pow5(1 - cos_theta_i)) * (1 + (f_d90 - 1) * pow5(1 - cos_theta_o));
	return clamp(f_d * power * cos_theta_i, 0.0, 1.0);
}

float f_specular(vec3 i, vec3 o, vec3 h, vec3 normal, float F0, float power, float specularPower) {
	vec3 reflected = -reflect(i, normal);
	float intensity = dot(reflected, o);

	if (intensity < 0) return 0;

	// Fresnel approximation
	float F0_scaled = 0.08 * F0;
	float o_dot_h = dot(o, h);
	float s = pow5(1 - o_dot_h);
	float F = F0_scaled + s * (1 - F0_scaled);
	
	return clamp(pow(intensity, specularPower) * F * power, 0.0, 1.0);
}

float f_specular_ambient(vec3 o, vec3 normal, float F0, float power) {
	// Fresnel approximation
	float F0_scaled = 0.08 * F0;
	float o_dot_n = dot(o, normal);
	float s = pow5(1 - o_dot_n);
	float F = F0_scaled + s * (1 - F0_scaled);

	return clamp(F * power, 0.0, 1.0);
}

float srgbToLinear(float u) {
	const float MinSrgbPower = 0.04045;

    if (u < MinSrgbPower) {
        return u / 12.92;
    }
    else {
        return pow((u + 0.055) / 1.055, 2.4);
    }
}

vec3 srgbToLinear(vec3 v) {
	return vec3(
		srgbToLinear(v.r),
		srgbToLinear(v.g),
		srgbToLinear(v.b)
	);
}

vec3 aces_approx(vec3 v) {
    v *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0f, 1.0f);
}

void main(void) {
	float shadowMapValues[8];
	for (int i = 0; i < 8; ++i) shadowMapValues[i] = 0;
	for (int i = 0; i < 8; ++i) {
		float calculatedDepth = ex_ShadowMapPos[i].z / ex_ShadowMapPos[i].w;
		vec3 shadowMapUV = ex_ShadowMapPos[i].xyz / ex_ShadowMapPos[i].w;
		shadowMapUV += vec3(1.0, 1.0, 1.0);
		shadowMapUV *= 0.5;
		shadowMapUV.z -= 0.001;

		const vec3 samples[9] = {
			vec3(1.0, 1.0, 0.0),
			vec3(0.0, 1.0, 0.0),
			vec3(-1.0, 1.0, 0.0),
			vec3(1.0, -1.0, 0.0),
			vec3(0.0, -1.0, 0.0),
			vec3(-1.0, -1.0, 0.0),
			vec3(1.0, 0.0, 0.0),
			vec3(0.0, 0.0, 0.0),
			vec3(-1.0, 0.0, 0.0)
		};
		
		for (int j = 0; j < 9; ++j) {
			if (shadowMapUV.x > 1.0 || shadowMapUV.x < 0.0) shadowMapValues[i] += 0.0;
			else if (shadowMapUV.y > 1.0 || shadowMapUV.y < 0.0) shadowMapValues[i] += 0.0;
			else {
				const float depth = texture(shadowMaps[i], shadowMapUV.xy + samples[j].xy * 0.001).r;
				if (depth < shadowMapUV.z) shadowMapValues[i] += 0;
				else shadowMapValues[i] += 1;
			}
		}

		shadowMapValues[i] /= 9;
	}

	const float FullSpecular = 1 / 0.08;

	vec3 totalLighting = vec3(1.0, 1.0, 1.0);
	vec3 normal = normalize(ex_Normal.xyz);

	vec4 baseColor;
	float roughness = 0.5;
	float power = 1.0;

	if (ColorReplace == 0) {
		vec4 diffuse = texture(diffuseTex, ex_Tex).rgba;
		baseColor = vec4(srgbToLinear(diffuse.rgb), diffuse.a) * BaseColor;
	}
	else {
		baseColor = BaseColor;
	}

	totalLighting = baseColor.rgb;

	const vec2 ss_uv = 0.5 * (ex_ScreenSpace.xy / ex_ScreenSpace.w + vec2(1, 1));
	const float ssao_f = mix(0.75, 1.0, clamp(4 * texture(ssao, ss_uv).x, 0, 1));

	if (Lit == 1) {
		vec3 o = normalize(CameraEye.xyz - ex_Pos.xyz);
		float cos_theta_o = dot(o, normal);

		vec3 ambientSpecular = 
			f_specular_ambient(o, normal, IncidentSpecular, SpecularMix) * AmbientLighting.rgb * AmbientSpecularAmount;
		vec3 ambientDiffuse = 
			f_diffuse(o, o, o, normal, DiffuseMix, DiffuseRoughness) * AmbientLighting.rgb * baseColor.rgb * AmbientDiffuseAmount;
		vec3 ambientMetallic = 
			f_specular_ambient(o, normal, FullSpecular, 1.0) * AmbientLighting.rgb * baseColor.rgb * AmbientSpecularAmount;

		vec3 totalAmbient = mix(
			ambientSpecular + ambientDiffuse,
			ambientMetallic,
			Metallic);
		
		totalLighting = totalAmbient;
		totalLighting += Emission.rgb;

		for (int li = 0; li < 32; ++li) {
			if (Lights[li].Active == 0) continue;

			vec3 i = Lights[li].Position.xyz - ex_Pos.xyz;
			float dist = length(i);
			float inv_dist = 1.0 / dist;
			i *= inv_dist;

			float cos_theta_i = dot(i, normal);

			if (cos_theta_i < 0) continue;
			if (cos_theta_o < 0) continue;

			vec3 h = normalize(i + o);
			vec3 diffuse =
				f_diffuse(i, o, h, normal, DiffuseMix, DiffuseRoughness) 
					* baseColor.rgb * Lights[li].Color.rgb;
			vec3 specular =
				f_specular(i, o, h, normal, IncidentSpecular, SpecularMix, SpecularPower) 
					* Lights[li].Color.rgb;
			vec3 metallic = vec3(0.0, 0.0, 0.0);

			if (Metallic > 0) {
				metallic =
					f_specular(i, o, h, normal, FullSpecular, 1, SpecularPower) 
						* Lights[li].Color.rgb * baseColor.rgb;
			}

			// Spotlight calculation
			float spotCoherence = -dot(i, Lights[li].Direction.xyz);
			float spotAttenuation = 1.0;
			if (spotCoherence > Lights[li].Attenuation0) spotAttenuation = 1.0;
			else if (spotCoherence < Lights[li].Attenuation1) spotAttenuation = 0.0;
			else {
				float t = Lights[li].Attenuation0 - Lights[li].Attenuation1;
				if (t == 0) spotAttenuation = 1.0;
				else spotAttenuation = (spotCoherence - Lights[li].Attenuation1) / t;
			}

			float falloff = 1.0;
			if (Lights[li].FalloffEnabled == 1) {
				const float invFalloffDist = 1 / max((dist - Lights[li].FalloffStart) / Lights[li].FalloffScale + 1.0, 1.0);
				falloff = (invFalloffDist * invFalloffDist);
			}

			vec3 bsdf = mix(diffuse * DiffuseAmount + specular * SpecularAmount, metallic * SpecularAmount, Metallic);

			if (Lights[li].ShadowMap != -1) {
				falloff *= mix(1.0, shadowMapValues[Lights[li].ShadowMap], ShadowAmount);
			}

			totalLighting += falloff * bsdf * spotAttenuation * spotAttenuation * spotAttenuation;
		}

		totalLighting *= mix(1.0, ssao_f, SsaoAmount);
	}

	const float distanceToCamera = length(CameraEye.xyz - ex_Pos.xyz);
	const float fogAttenuation = 
		(clamp(distanceToCamera, FogNear, FogFar) - FogNear) / (FogFar - FogNear);

	float ao = 1.0;
	if (AoMap == 1) {
		ao = mix(1.0, mix(texture(aoTex, ex_Tex).r, 1.0, 0.5), BakedAoAmount);
	}

	const float brightness = dot(totalLighting.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.8) {
		out_BrightColor = vec4(totalLighting.rgb * (brightness - 1.8) * 0.1, 1.0);
	}
	else {
		out_BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

	out_Color = mix(
		vec4(mix(totalLighting.rgb * ao, FogColor.rgb, fogAttenuation * FogEffect), baseColor.a),
		vec4(ssao_f, ssao_f, ssao_f, 1.0f),
		DebugSsao);

	out_Color = mix(
		out_Color,
		out_BrightColor,
		DebugBloom
	);
}
