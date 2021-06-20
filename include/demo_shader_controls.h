#ifndef CEREAL_ADVENTURE_DEMO_SHADER_CONTROLS_H
#define CEREAL_ADVENTURE_DEMO_SHADER_CONTROLS_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class DemoShaderControls : public GameObject {
    public:
        enum class Controls {
            AmbientDiffuse,
            AmbientSpecular,
            Ssao,
            BakedAo,
            Diffuse,
            Specular,
            Shadow,
            VisualizeSsao,
            VisualizeBloom,
            Bloom,
            Dither,
            DebugBlackAndWhite,
            Count
        };

    public:
        DemoShaderControls();
        ~DemoShaderControls();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        void initialize(Controls control, ysKey::Code key, bool value);
        void invert(Controls control);
        void set(Controls control, bool value);
        bool get(Controls control);

        void submit(Controls control, float value);

    protected:
        bool m_controls[(int)Controls::Count];
        bool m_controlDefault[(int)Controls::Count];
        ysKey::Code m_keyMapping[(int)Controls::Count];
        SpringConnector m_connectors[(int)Controls::Count];
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_DEMO_SHADER_CONTROLS_H */
