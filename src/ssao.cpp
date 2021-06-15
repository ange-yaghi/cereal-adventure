#include "../include/ssao.h"

c_adv::Ssao::Ssao() {
    m_shaderProgram = nullptr;
    m_fragmentShader = nullptr;
    m_shaderStage = nullptr;
    m_renderTarget = nullptr;
    m_device = nullptr;
}

c_adv::Ssao::~Ssao() {
    /* void */
}

ysError c_adv::Ssao::Initialize(const Shaders::Context &context, ysRenderTarget *depthBuffer) {
    YDS_ERROR_DECLARE("Initialize");

    m_device = context.Device;

    YDS_NESTED_ERROR_CALL(context.ShaderSet->NewStage("SSAO", &m_shaderStage));

    if (context.Device->GetAPI() == ysDevice::DeviceAPI::DirectX11) {
        /* void */
    }
    else if (context.Device->GetAPI() == ysDevice::DeviceAPI::OpenGL4_0) {
        const std::string fragmentShaderPath = context.ShaderPath + "/glsl/ssao/ssao.frag";
        YDS_NESTED_ERROR_CALL(context.Device->CreatePixelShader(&m_fragmentShader, fragmentShaderPath.c_str(), "PS"));
    }

    ysShader *saqShader = context.Engine->GetSaqVertexShader();
    ysInputLayout *saqInputLayout = context.Engine->GetSaqInputLayout();

    YDS_NESTED_ERROR_CALL(context.Device->CreateShaderProgram(&m_shaderProgram));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_shaderProgram, saqShader));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_shaderProgram, m_fragmentShader));
    YDS_NESTED_ERROR_CALL(context.Device->LinkProgram(m_shaderProgram));

    YDS_NESTED_ERROR_CALL(context.Device->CreateOffScreenRenderTarget(
        &m_renderTarget,
        512, 240,
        ysRenderTarget::Format::R8G8B8A8_UNORM, true, false));

    m_shaderStage->SetInputLayout(saqInputLayout);
    m_shaderStage->SetRenderTarget(m_renderTarget);
    m_shaderStage->SetShaderProgram(m_shaderProgram);
    m_shaderStage->SetType(dbasic::ShaderStage::Type::PostProcessing);
    m_shaderStage->AddInput(depthBuffer, 0);

    YDS_NESTED_ERROR_CALL(m_shaderStage->NewConstantBuffer<SsaoControls>(
        "SsaoControls", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_controls));

    for (int i = 0; i < m_controls.MaxKernelSize; ++i) {
        m_controls.Kernel[i] = ysVector3(
            ysMath::UniformRandom(2.0f) - 1.0f,
            ysMath::UniformRandom(2.0f) - 1.0f,
            ysMath::UniformRandom(2.0f) - 1.0f);
    }

    m_controls.SampleRadius = 0.4f;

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Ssao::OnResize(int width, int height) {
    m_device->ResizeRenderTarget(m_renderTarget, width, height, width, height);
}

ysError c_adv::Ssao::Destroy() {
    YDS_ERROR_DECLARE("Destroy");

    /* TODO */

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::Ssao::SetProjection(const ysMatrix &projection) {
    m_controls.Projection = projection;
}
