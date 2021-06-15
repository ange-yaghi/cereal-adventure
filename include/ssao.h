#ifndef CEREAL_ADVENTURE_SSAO_H
#define CEREAL_ADVENTURE_SSAO_H

#include "delta.h"

#include "shaders.h"

namespace c_adv {

    class Ssao : public ysObject {
    public:
        Ssao();
        ~Ssao();

        ysError Initialize(const Shaders::Context &context, ysRenderTarget *depthBuffer);
        ysRenderTarget *GetOutput() const { return m_renderTarget; }

        void OnResize(int width, int height);

        ysError Destroy();

        void SetProjection(const ysMatrix &projection);

    protected:
        SsaoControls m_controls;

        dbasic::ShaderStage *m_shaderStage;
        ysShader *m_fragmentShader;
        ysShaderProgram *m_shaderProgram;
        ysRenderTarget *m_renderTarget;

        ysDevice *m_device;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_SSAO_H */
