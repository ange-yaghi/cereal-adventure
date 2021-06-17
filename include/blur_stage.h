#ifndef CEREAL_ADVENTURE_BLUR_STAGE_H
#define CEREAL_ADVENTURE_BLUR_STAGE_H

#include "shaders.h"

#include "delta.h"

namespace c_adv {

    class BlurStage : public dbasic::ShaderStage {
    public:
        BlurStage();
        ~BlurStage();

        ysError Create(const Shaders::Context &context);

        virtual void OnPass(int pass);
        virtual void OnEnd();

        ysRenderTarget *GetInput() const { return m_input; }
        void SetInput(ysRenderTarget *input) { m_input = input; }

        ysRenderTarget *GetOutput() const { return m_output; }
        void SetOutput(ysRenderTarget *output) { m_output = output; }

    protected:
        void SetHorizontal(bool horizontal);

        union {
            struct {
                ysRenderTarget *m_buffer0;
                ysRenderTarget *m_buffer1;
            };

            ysRenderTarget *m_buffers[2];
        };

        ysRenderTarget *m_input;
        ysRenderTarget *m_output;

        BlurControls m_controls;

        ysShader *m_fragmentShader;
        ysShaderProgram *m_shaderProgram;

        ysRenderTarget *m_currentWriteBuffer;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_BLUR_STAGE_H */
