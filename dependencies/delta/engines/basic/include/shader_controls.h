#ifndef DELTA_BASIC_SHADER_CONTROLS_H
#define DELTA_BASIC_SHADER_CONTROLS_H

#include "delta_core.h"

namespace dbasic {

    struct Vertex {
        ysVector4 Pos = { 0.0f, 0.0f, 0.0f, 0.0f };
        ysVector2 TexCoord = { 0.0f, 0.0f };
        ysVector4 Normal = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    struct ShaderObjectVariables {
        ysMatrix Transform = ysMath::LoadIdentity();

        ysVector4 MulCol = { 1.0f, 1.0f, 1.0f, 1.0f };
        float TexOffset[2] = { 0.0f, 0.0f };
        float TexScale[2] = { 1.0f, 1.0f };
        float Scale[3] = { 1.0f, 1.0f, 1.0f };
        int ColorReplace = 0;
        int Lit = 1;

        int Pad[2] = { 0, 0 };
    };

    struct ShaderScreenVariables {
        ysMatrix CameraView = ysMath::LoadIdentity();
        ysMatrix Projection = ysMath::LoadIdentity();

        float Eye[3] = { 0.0f, 0.0f, 0.0f };
    };

    struct ShaderSkinningControls {
        ysMatrix BoneTransforms[256];
    };

    struct Light {
        ysVector4 Position = { 0.0f, 0.0f, 0.0f, 1.0f };
        ysVector4 Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        ysVector4 Direction = { 0.0f, 0.0f, -1.0f, 1.0f };
        float Attenuation0 = 0.0f;
        float Attenuation1 = 0.0f;
        int FalloffEnabled = 1; 
        int Active = 0;
    };

    struct LightingControls {
        static constexpr int MaxLights = 32;

        Light Lights[MaxLights];
        ysVector4 AmbientLighting;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SHADER_CONTROLS_H */
