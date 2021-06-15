#include "../include/shaders.h"

#include "../include/ssao.h"

#include <sstream>

c_adv::Shaders::Shaders() {
    m_lightCount = 0;

    m_cameraPosition = ysMath::LoadVector(0.0f, 0.0f, 10.0f);
    m_cameraTarget = ysMath::Constants::Zero3;
    m_cameraMode = CameraMode::Flat;
    m_cameraAngle = 0.0f;

    m_cameraFov = ysMath::Constants::PI / 3.0f;

    m_cameraUp = ysMath::Constants::ZAxis;
    m_screenHeight = 1.0f;
    m_screenWidth = 1.0f;

    m_nearClip = 2.0f;
    m_farClip = 100.0f;
    m_shadowDepth = 200.0f;

    m_mainStage = nullptr;

    m_shadowVertexShader = nullptr;
    m_vertexShader = nullptr;
    m_fragmentShader = nullptr;
    m_shadowInputLayout = nullptr;
    m_inputLayout = nullptr;
    m_mainShaderProgram = nullptr;
    m_shadowMapShaderProgram = nullptr;

    m_device = nullptr;

    m_ssao = nullptr;

    m_aoTexture = 0;
    m_mainStageDiffuseTexture = 0;

    m_shadowLightPosition = ysMath::LoadVector(-5.0f, 0.0f, 10.0f);
    m_shadowLightTarget = ysMath::Constants::Zero;
    m_shadowLightUp = ysMath::Constants::YAxis;

    m_shadowMapObjectVariables = nullptr;
    m_shadowMapScreenVariables = nullptr;
}

c_adv::Shaders::~Shaders() {
    /* void */
}

ysError c_adv::Shaders::Initialize(const Context &context) {
    YDS_ERROR_DECLARE("Initialize");

    YDS_NESTED_ERROR_CALL(context.Device->CreateOffScreenRenderTarget(&m_depthBuffer, 2048, 2048, ysRenderTarget::Format::R32_FLOAT, false, true));

    m_ssao = new Ssao();

    m_shadowMapStages = new dbasic::ShaderStage *[MaxShadowMaps];
    for (int i = 0; i < MaxShadowMaps; ++i) {
        std::stringstream ss; ss << "ShadowMap::" << i;
        YDS_NESTED_ERROR_CALL(context.ShaderSet->NewStage(ss.str(), &m_shadowMapStages[i]));
    }
    
    YDS_NESTED_ERROR_CALL(context.ShaderSet->NewStage("ShaderStage::Depth", &m_depthPass));
    YDS_NESTED_ERROR_CALL(m_ssao->Initialize(context, m_depthBuffer));
    YDS_NESTED_ERROR_CALL(context.ShaderSet->NewStage("ShaderStage::Main", &m_mainStage));
    YDS_NESTED_ERROR_CALL(context.ShaderSet->NewStage("ShaderStage::UI", &m_uiStage));

    if (context.Device->GetAPI() == ysDevice::DeviceAPI::DirectX11) {
        /* void */
    }
    else if (context.Device->GetAPI() == ysDevice::DeviceAPI::OpenGL4_0) {
        const std::string shadowVertexShaderPath = context.ShaderPath + "/glsl/depth_only.vert";
        const std::string vertexShaderPath = context.ShaderPath + "/glsl/main.vert";
        const std::string fragmentShaderPath = context.ShaderPath + "/glsl/main.frag";

        YDS_NESTED_ERROR_CALL(context.Device->CreateVertexShader(&m_shadowVertexShader, shadowVertexShaderPath.c_str(), "VS"));
        YDS_NESTED_ERROR_CALL(context.Device->CreateVertexShader(&m_vertexShader, vertexShaderPath.c_str(), "VS"));
        YDS_NESTED_ERROR_CALL(context.Device->CreatePixelShader(&m_fragmentShader, fragmentShaderPath.c_str(), "PS"));
    }

    // Shadow maps
    m_shadowMapScreenVariables = new AllShadowMapScreenVariables;
    m_shadowMapObjectVariables = new ShadowMapObjectVariables[MaxShadowMaps];
    m_shadowMaps = new ysRenderTarget *[MaxShadowMaps];

    YDS_NESTED_ERROR_CALL(context.Device->CreateInputLayout(&m_shadowInputLayout, m_shadowVertexShader, context.GeometryFormat));

    YDS_NESTED_ERROR_CALL(context.Device->CreateShaderProgram(&m_shadowMapShaderProgram));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_shadowMapShaderProgram, m_shadowVertexShader));
    YDS_NESTED_ERROR_CALL(context.Device->LinkProgram(m_shadowMapShaderProgram));

    // Main depth pass
    YDS_NESTED_ERROR_CALL(m_depthPass->NewConstantBuffer<ShadowMapScreenVariables>(
        "Buffer::ScreenData", 0, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_depthPassScreenVariables));
    YDS_NESTED_ERROR_CALL(m_depthPass->NewConstantBuffer<ShadowMapObjectVariables>(
        "Buffer::ObjectData", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::ObjectData, &m_depthPassObjectVariables));

    m_depthPass->SetInputLayout(m_shadowInputLayout);
    m_depthPass->SetRenderTarget(m_depthBuffer);
    m_depthPass->SetShaderProgram(m_shadowMapShaderProgram);
    m_depthPass->SetType(dbasic::ShaderStage::Type::FullPass);
    m_depthPass->SetCullMode(ysDevice::CullMode::Back);

    // Shadow map passes
    for (int i = 0; i < MaxShadowMaps; ++i) {
        YDS_NESTED_ERROR_CALL(context.Device->CreateOffScreenRenderTarget(&m_shadowMaps[i], 4096, 4096, ysRenderTarget::Format::R32_FLOAT, false, true));

        YDS_NESTED_ERROR_CALL(m_shadowMapStages[i]->NewConstantBuffer<ShadowMapScreenVariables>(
            "Buffer::ScreenData", 0, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_shadowMapScreenVariables->ScreenVariables[i]));
        YDS_NESTED_ERROR_CALL(m_shadowMapStages[i]->NewConstantBuffer<ShadowMapObjectVariables>(
            "Buffer::ObjectData", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::ObjectData, &m_shadowMapObjectVariables[i]));

        m_shadowMapStages[i]->SetInputLayout(m_shadowInputLayout);
        m_shadowMapStages[i]->SetRenderTarget(m_shadowMaps[i]);
        m_shadowMapStages[i]->SetShaderProgram(m_shadowMapShaderProgram);
        m_shadowMapStages[i]->SetType(dbasic::ShaderStage::Type::FullPass);
        m_shadowMapStages[i]->SetCullMode(ysDevice::CullMode::Back);
    }

    // Main Stage
    YDS_NESTED_ERROR_CALL(context.Device->CreateInputLayout(&m_inputLayout, m_vertexShader, context.GeometryFormat));

    YDS_NESTED_ERROR_CALL(context.Device->CreateShaderProgram(&m_mainShaderProgram));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_mainShaderProgram, m_vertexShader));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_mainShaderProgram, m_fragmentShader));
    YDS_NESTED_ERROR_CALL(context.Device->LinkProgram(m_mainShaderProgram));

    m_mainStage->SetInputLayout(m_inputLayout);
    m_mainStage->SetRenderTarget(context.RenderTarget);
    m_mainStage->SetShaderProgram(m_mainShaderProgram);
    m_mainStage->SetType(dbasic::ShaderStage::Type::FullPass);

    YDS_NESTED_ERROR_CALL(m_mainStage->NewConstantBuffer<ShaderScreenVariables>(
        "Buffer::ScreenData", 0, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_shaderScreenVariables));
    YDS_NESTED_ERROR_CALL(m_mainStage->NewConstantBuffer<ShaderObjectVariables>(
        "Buffer::ObjectData", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::ObjectData, &m_shaderObjectVariables));
    YDS_NESTED_ERROR_CALL(m_mainStage->NewConstantBuffer<LightingControls>(
        "Buffer::LightingData", 2, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_lightingControls));
    YDS_NESTED_ERROR_CALL(m_mainStage->NewConstantBuffer<AllShadowMapScreenVariables>(
        "Buffer::ShadowMapData", 3, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, m_shadowMapScreenVariables));

    m_mainStage->AddTextureInput(0, &m_mainStageDiffuseTexture);
    m_mainStage->AddTextureInput(1, &m_aoTexture);
    m_mainStage->AddInput(m_ssao->GetOutput(), 2);

    for (int i = 0; i < MaxShadowMaps; ++i) {
        m_mainStage->AddInput(m_shadowMaps[i], 3 + i);
    }

    m_uiStage->SetClearColor(ysMath::Constants::Zero);
    m_uiStage->SetInputLayout(m_inputLayout);
    m_uiStage->SetRenderTarget(context.UiRenderTarget);
    m_uiStage->SetShaderProgram(m_mainShaderProgram);
    m_uiStage->SetType(dbasic::ShaderStage::Type::FullPass);
    m_uiStage->SetFlagBit(10);

    YDS_NESTED_ERROR_CALL(m_uiStage->NewConstantBuffer<ShaderScreenVariables>(
        "Buffer::ScreenData", 0, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_uiShaderScreenVariables));
    YDS_NESTED_ERROR_CALL(m_uiStage->NewConstantBuffer<ShaderObjectVariables>(
        "Buffer::ObjectData", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::ObjectData, &m_shaderObjectVariables));
    YDS_NESTED_ERROR_CALL(m_uiStage->NewConstantBuffer<LightingControls>(
        "Buffer::LightingData", 2, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_lightingControls));
    YDS_NESTED_ERROR_CALL(m_uiStage->NewConstantBuffer<AllShadowMapScreenVariables>(
        "Buffer::ShadowMapData", 3, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, m_shadowMapScreenVariables));

    m_uiStage->AddTextureInput(0, &m_mainStageDiffuseTexture);
    m_uiStage->AddTextureInput(1, &m_aoTexture);

    m_device = context.Device;

    ConfigureFlags(0, 1);

    return YDS_ERROR_RETURN(ysError::None);
}

ysError c_adv::Shaders::Destroy() {
    YDS_ERROR_DECLARE("Destroy");

    YDS_NESTED_ERROR_CALL(m_ssao->Destroy());

    YDS_NESTED_ERROR_CALL(m_device->DestroyShader(m_vertexShader));
    YDS_NESTED_ERROR_CALL(m_device->DestroyShader(m_fragmentShader));

    YDS_NESTED_ERROR_CALL(m_device->DestroyShaderProgram(m_mainShaderProgram));

    YDS_NESTED_ERROR_CALL(m_device->DestroyInputLayout(m_inputLayout));

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Shaders::OnResize(int width, int height) {
    m_ssao->OnResize(width, height);
}

ysError c_adv::Shaders::UseMaterial(dbasic::Material *material) {
    YDS_ERROR_DECLARE("UseMaterial");

    ResetBrdfParameters();

    if (material == nullptr) {
        SetBaseColor(ysMath::LoadVector(1.0f, 1.0f, 1.0f, 1.0f));
        SetColorReplace(true);
        SetAoMap(false);
    }
    else {
        SetBaseColor(material->GetDiffuseColor());
        SetLit(material->IsLit());

        if (material->UsesDiffuseMap()) {
            SetColorReplace(false);
            SetDiffuseTexture(material->GetDiffuseMap());
        }
        else {
            SetColorReplace(true);
        }

        if (material->UsesAoMap()) {
            SetAoMap(true);
            SetAoTexture(material->GetAoMap());
        }
        else {
            SetAoMap(false);
        }
    }

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Shaders::Update() {
    for (int i = 0; i < MaxShadowMaps; ++i) {
        m_shadowMapStages[i]->SetEnabled(i < m_shadowMapCount);
    }
}

void c_adv::Shaders::ResetBrdfParameters() {
    static const ShaderObjectVariables defaults{};
    m_shaderObjectVariables.BaseColor = defaults.BaseColor;
    m_shaderObjectVariables.AoMap = defaults.AoMap;
    m_shaderObjectVariables.DiffuseMix = defaults.DiffuseMix;
    m_shaderObjectVariables.DiffuseRoughness = defaults.DiffuseRoughness;
    m_shaderObjectVariables.Emission = defaults.Emission;
    m_shaderObjectVariables.IncidentSpecular = defaults.IncidentSpecular;
    m_shaderObjectVariables.Lit = defaults.Lit;
    m_shaderObjectVariables.Metallic = defaults.Metallic;
    m_shaderObjectVariables.SpecularMix = defaults.SpecularMix;
    m_shaderObjectVariables.SpecularPower = defaults.SpecularPower;
    m_shaderObjectVariables.FogEffect = 0.0f;
}

void c_adv::Shaders::SetBaseColor(const ysVector &color) {
    m_shaderObjectVariables.BaseColor = ysMath::GetVector4(color);
}

void c_adv::Shaders::ResetBaseColor() {
    m_shaderObjectVariables.BaseColor = ysVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void c_adv::Shaders::SetScale(float x, float y, float z) {
    m_shaderObjectVariables.Scale[0] = x;
    m_shaderObjectVariables.Scale[1] = y;
    m_shaderObjectVariables.Scale[2] = z;

    for (int i = 0; i < MaxShadowMaps; ++i) {
        m_shadowMapObjectVariables[i].Scale[0] = x;
        m_shadowMapObjectVariables[i].Scale[1] = y;
        m_shadowMapObjectVariables[i].Scale[2] = z;
    }

    m_depthPassObjectVariables.Scale[0] = x;
    m_depthPassObjectVariables.Scale[1] = y;
    m_depthPassObjectVariables.Scale[2] = z;
}

void c_adv::Shaders::SetTexOffset(float u, float v) {
    m_shaderObjectVariables.TexOffset[0] = u;
    m_shaderObjectVariables.TexOffset[1] = v;
}

void c_adv::Shaders::SetTexScale(float u, float v) {
    m_shaderObjectVariables.TexScale[0] = u;
    m_shaderObjectVariables.TexScale[1] = v;
}

void c_adv::Shaders::SetAoMap(bool aoMap) {
    m_shaderObjectVariables.AoMap = (aoMap) ? 1 : 0;
}

void c_adv::Shaders::SetColorReplace(bool colorReplace) {
    m_shaderObjectVariables.ColorReplace = (colorReplace) ? 1 : 0;
}

void c_adv::Shaders::SetLit(bool lit) {
    m_shaderObjectVariables.Lit = (lit) ? 1 : 0;
}

void c_adv::Shaders::SetEmission(const ysVector &emission) {
    m_shaderObjectVariables.Emission = ysMath::GetVector4(emission);
}

void c_adv::Shaders::SetSpecularMix(float specularMix) {
    m_shaderObjectVariables.SpecularMix = specularMix;
}

void c_adv::Shaders::SetDiffuseMix(float diffuseMix) {
    m_shaderObjectVariables.DiffuseMix = diffuseMix;
}

void c_adv::Shaders::SetMetallic(float metallic) {
    m_shaderObjectVariables.Metallic = metallic;
}

void c_adv::Shaders::SetDiffuseRoughness(float diffuseRoughness) {
    m_shaderObjectVariables.DiffuseRoughness = diffuseRoughness;
}

void c_adv::Shaders::SetSpecularRoughness(float specularRoughness) {
    m_shaderObjectVariables.SpecularPower = ::pow(2.0f, 12.0f * (1.0f - specularRoughness));
}

void c_adv::Shaders::SetSpecularPower(float power) {
    m_shaderObjectVariables.SpecularPower = power;
}

void c_adv::Shaders::SetIncidentSpecular(float incidentSpecular) {
    m_shaderObjectVariables.IncidentSpecular = incidentSpecular;
}

void c_adv::Shaders::SetFogNear(float fogNear) {
    m_shaderScreenVariables.FogNear = fogNear;
}

void c_adv::Shaders::SetFogFar(float fogFar) {
    m_shaderScreenVariables.FogFar = fogFar;
}

void c_adv::Shaders::SetFogColor(const ysVector &color) {
    m_shaderScreenVariables.FogColor = ysMath::GetVector4(color);
}

void c_adv::Shaders::SetSsaoEnable(bool enable) {
    m_shaderScreenVariables.SsaoEnable = enable ? 1 : 0;
}

bool c_adv::Shaders::GetSsaoEnable() const {
    return m_shaderScreenVariables.SsaoEnable == 1;
}

void c_adv::Shaders::SetObjectTransform(const ysMatrix &mat) {
    m_shaderObjectVariables.Transform = mat;

    for (int i = 0; i < MaxShadowMaps; ++i) {
        m_shadowMapObjectVariables[i].Transform = mat;
    }

    m_depthPassObjectVariables.Transform = mat;
}

void c_adv::Shaders::SetPositionOffset(const ysVector &position) {
    m_shaderObjectVariables.Transform =
        ysMath::MatMult(m_shaderObjectVariables.Transform, ysMath::TranslationTransform(position));
}

void c_adv::Shaders::SetProjection(const ysMatrix &mat) {
    m_shaderScreenVariables.Projection = mat;
    m_depthPassScreenVariables.Projection = mat;
    m_ssao->SetProjection(mat);
}

void c_adv::Shaders::SetCameraView(const ysMatrix &mat) {
    m_shaderScreenVariables.CameraView = mat;
    m_depthPassScreenVariables.CameraView = mat;
}

void c_adv::Shaders::SetEye(const ysVector &vec) {
    m_shaderScreenVariables.Eye = ysMath::GetVector4(vec);
}

ysError c_adv::Shaders::AddLight(const Light &light) {
    YDS_ERROR_DECLARE("AddLight");

    if (m_lightCount >= LightingControls::MaxLights) return YDS_ERROR_RETURN(ysError::None);
    m_lightingControls.Lights[m_lightCount] = light;
    m_lightingControls.Lights[m_lightCount].Active = 1;
    ++m_lightCount;

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Shaders::ResetLights() {
    m_lightCount = 0;
    for (int i = 0; i < LightingControls::MaxLights; ++i) {
        m_lightingControls.Lights[i].Active = 0;
    }

    m_lightingControls.AmbientLighting = ysVector4(0.0f, 0.0f, 0.0f, 0.0f);
    m_shadowMapCount = 0;
}

int c_adv::Shaders::AddPerspectiveShadowMap(
    const ysVector &source,
    const ysVector &target,
    const ysVector &up,
    float fov,
    float aspect,
    float nearClip,
    float farClip)
{
    if (m_shadowMapCount >= MaxShadowMaps) return -1;

    m_shadowMapScreenVariables->ScreenVariables[m_shadowMapCount].Projection =
        ysMath::Transpose(ysMath::FrustrumPerspective(fov, aspect, nearClip, farClip));
    m_shadowMapScreenVariables->ScreenVariables[m_shadowMapCount].CameraView =
        ysMath::Transpose(ysMath::CameraTarget(source, target, up));

    return m_shadowMapCount++;
}

int c_adv::Shaders::AddOrthographicShadowMap(
    const ysVector &source,
    const ysVector &target,
    const ysVector &up,
    float width,
    float height,
    float nearClip,
    float farClip)
{
    m_shadowMapScreenVariables->ScreenVariables[m_shadowMapCount].Projection =
        ysMath::Transpose(ysMath::OrthographicProjection(width, height, nearClip, farClip));
    m_shadowMapScreenVariables->ScreenVariables[m_shadowMapCount].CameraView =
        ysMath::Transpose(ysMath::CameraTarget(source, target, up));

    return m_shadowMapCount++;
}

ysError c_adv::Shaders::SetAmbientLight(const ysVector4 &ambient) {
    YDS_ERROR_DECLARE("SetAmbientLight");

    m_lightingControls.AmbientLighting = ambient;

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Shaders::SetCameraPosition(float x, float y) {
    const ysVector3 p = ysMath::GetVector3(m_cameraPosition);
    m_cameraPosition = ysMath::LoadVector(x, y, p.z, 1.0f);
}

void c_adv::Shaders::SetCameraPosition(const ysVector &pos) {
    m_cameraPosition = pos;
}

ysVector c_adv::Shaders::GetCameraPosition() const {
    return m_cameraPosition;
}

void c_adv::Shaders::GetCameraPosition(float *x, float *y) const {
    *x = ysMath::GetX(m_cameraPosition);
    *y = ysMath::GetY(m_cameraPosition);
}

void c_adv::Shaders::SetCameraUp(const ysVector &up) {
    m_cameraUp = up;
}

ysVector c_adv::Shaders::GetCameraUp() const {
    return m_cameraUp;
}

void c_adv::Shaders::SetCameraTarget(const ysVector &target) {
    m_cameraTarget = target;
}

void c_adv::Shaders::SetCameraMode(CameraMode mode) {
    m_cameraMode = mode;
}

void c_adv::Shaders::SetCameraAngle(float angle) {
    m_cameraAngle = angle;
}

float c_adv::Shaders::GetCameraFov() const {
    return m_cameraFov;
}

void c_adv::Shaders::SetCameraFov(float fov) {
    m_cameraFov = fov;
}

float c_adv::Shaders::GetCameraAspect() const {
    return m_screenWidth / m_screenHeight;
}

void c_adv::Shaders::SetCameraAltitude(float altitude) {
    const ysVector3 p = ysMath::GetVector3(m_cameraPosition);
    m_cameraPosition = ysMath::LoadVector(p.x, p.y, altitude, 1.0f);
}

float c_adv::Shaders::GetCameraAltitude() const {
    return ysMath::GetZ(m_cameraPosition);
}

void c_adv::Shaders::SetScreenDimensions(float width, float height) {
    m_screenWidth = width;
    m_screenHeight = height;
}

void c_adv::Shaders::CalculateCamera() {
    const float aspect = GetCameraAspect();
    const float sinRot = sin(m_cameraAngle * ysMath::Constants::PI / 180.0f);
    const float cosRot = cos(m_cameraAngle * ysMath::Constants::PI / 180.0f);

    SetProjection(ysMath::Transpose(ysMath::FrustrumPerspective(m_cameraFov, aspect, m_nearClip, m_farClip)));

    const ysVector cameraEye = m_cameraPosition;
    ysVector cameraTarget;
    ysVector up;

    if (m_cameraMode == CameraMode::Flat) {
        cameraTarget = ysMath::Mask(cameraEye, ysMath::Constants::MaskOffZ);
        up = ysMath::LoadVector(-sinRot, cosRot);
    }
    else {
        cameraTarget = m_cameraTarget;
        const ysVector cameraDir = ysMath::Sub(cameraTarget, cameraEye);
        const ysVector right = ysMath::Cross(cameraDir, m_cameraUp);
        up = ysMath::Normalize(ysMath::Cross(right, cameraDir));
    }

    SetCameraView(ysMath::Transpose(ysMath::CameraTarget(cameraEye, cameraTarget, up)));
    SetEye(cameraEye);
}

void c_adv::Shaders::ConfigureFlags(int regularFlagIndex, int riggedFlagIndex) {
    m_mainStage->SetFlagBit(regularFlagIndex);
}

dbasic::StageEnableFlags c_adv::Shaders::GetRegularFlags() const {
    return m_mainStage->GetFlags();
}

void c_adv::Shaders::ConfigureImage(
    float scaleX, float scaleY,
    float texOffsetU, float texOffsetV,
    float texScaleU, float texScaleV)
{
    SetScale(scaleX, scaleY);
    SetTexOffset(texOffsetU, texOffsetV);
    SetTexScale(texScaleU, texScaleV);
    SetColorReplace(false);
    SetLit(true);
}

void c_adv::Shaders::ConfigureBox(float width, float height) {
    SetScale(width / 2.0f, height / 2.0f);
    SetTexOffset(0.0f, 0.0f);
    SetTexScale(1.0f, 1.0f);
    SetColorReplace(true);
}

void c_adv::Shaders::ConfigureAxis(
    const ysVector &position, const ysVector &direction, float length)
{
    ysMatrix trans = ysMath::TranslationTransform(position);
    ysMatrix offset = ysMath::TranslationTransform(ysMath::LoadVector(0, length / 2.0f));

    ysVector orth = ysMath::Cross(direction, ysMath::Constants::ZAxis);
    ysMatrix dir = ysMath::LoadMatrix(orth, direction, ysMath::Constants::ZAxis, ysMath::Constants::IdentityRow4);
    dir = ysMath::Transpose(dir);

    ysMatrix transform = ysMath::MatMult(trans, dir);
    transform = ysMath::MatMult(transform, offset);
    SetObjectTransform(transform);
}

void c_adv::Shaders::ConfigureModel(float scale, dbasic::ModelAsset *model) {
    SetScale(scale, scale, scale);
    SetTexOffset(0.0f, 0.0f);
    SetTexScale(1.0f, 1.0f);
    UseMaterial(model->GetMaterial());
}

void c_adv::Shaders::SetDiffuseTexture(ysTexture *texture) {
    m_mainStage->BindTexture(texture, m_mainStageDiffuseTexture);
    m_uiStage->BindTexture(texture, m_mainStageDiffuseTexture);
}

void c_adv::Shaders::SetAoTexture(ysTexture *texture) {
    m_mainStage->BindTexture(texture, m_aoTexture);
}
