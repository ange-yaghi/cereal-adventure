#include "../include/blur_stage.h"

c_adv::BlurStage::BlurStage() {
    m_buffer0 = nullptr;
    m_buffer1 = nullptr;

    m_input = nullptr;
    m_output = nullptr;

    m_currentWriteBuffer = nullptr;

    m_fragmentShader = nullptr;
    m_shaderProgram = nullptr;

    m_currentWriteBuffer = 0;
}

c_adv::BlurStage::~BlurStage() {
    /* void */
}

ysError c_adv::BlurStage::Create(const Shaders::Context &context) {
    YDS_ERROR_DECLARE("Create");

    YDS_NESTED_ERROR_CALL(context.Device->CreateOffScreenRenderTarget(
        &m_buffer0,
        512,
        240,
        ysRenderTarget::Format::R32G32B32_FLOAT,
        true,
        false));

    YDS_NESTED_ERROR_CALL(context.Device->CreateOffScreenRenderTarget(
        &m_buffer1,
        512,
        240,
        ysRenderTarget::Format::R32G32B32_FLOAT,
        true,
        false));

    if (context.Device->GetAPI() == ysDevice::DeviceAPI::DirectX11) {
        /* void */
    }
    else if (context.Device->GetAPI() == ysDevice::DeviceAPI::OpenGL4_0) {
        const std::string fragmentShaderPath = context.ShaderPath + "/glsl/blur/blur.frag";
        YDS_NESTED_ERROR_CALL(context.Device->CreatePixelShader(&m_fragmentShader, fragmentShaderPath.c_str(), "PS"));
    }

    ysShader *saqShader = context.Engine->GetSaqVertexShader();
    ysInputLayout *saqInputLayout = context.Engine->GetSaqInputLayout();

    YDS_NESTED_ERROR_CALL(context.Device->CreateShaderProgram(&m_shaderProgram));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_shaderProgram, saqShader));
    YDS_NESTED_ERROR_CALL(context.Device->AttachShader(m_shaderProgram, m_fragmentShader));
    YDS_NESTED_ERROR_CALL(context.Device->LinkProgram(m_shaderProgram));

    YDS_NESTED_ERROR_CALL(NewConstantBuffer<BlurControls>(
        "BlurControls", 1, dbasic::ShaderStage::ConstantBufferBinding::BufferType::SceneData, &m_controls));

    // Stage setup
    SetClearColor(ysMath::Constants::Zero);
    SetInputLayout(context.Engine->GetSaqInputLayout());
    AddInput(m_input, 0);
    SetRenderTarget(m_buffer0);
    SetShaderProgram(m_shaderProgram);
    SetType(dbasic::ShaderStage::Type::PostProcessing);
    SetPasses(10);

    return YDS_ERROR_RETURN(ysError::None);
}

void c_adv::BlurStage::OnPass(int pass) {
    SetHorizontal(pass % 2 == 0);

    if (pass == 0) {
        ShaderStage::SetInput(m_input, 0);
        SetRenderTarget(m_buffer0);

        m_currentWriteBuffer = m_buffer0;
    }
    else if (pass == GetPasses() - 1) {
        ShaderStage::SetInput(m_currentWriteBuffer, 0);
        SetRenderTarget(m_output);
    }
    else {
        if (pass % 2 == 0) {
            ShaderStage::SetInput(m_buffer1, 0);
            SetRenderTarget(m_buffer0);
        }
        else {
            ShaderStage::SetInput(m_buffer0, 0);
            SetRenderTarget(m_buffer1);
        }
    }
}

void c_adv::BlurStage::OnEnd() {
    /* void */
}

void c_adv::BlurStage::SetHorizontal(bool horizontal) {
    m_controls.Horizontal = horizontal
        ? 1
        : 0;
}
