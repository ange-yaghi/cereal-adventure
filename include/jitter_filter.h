#ifndef CEREAL_ADVENTURE_JITTER_FILTER_H
#define CEREAL_ADVENTURE_JITTER_FILTER_H

#include "delta.h"

namespace c_adv {
 
    class JitterFilter {
    public:
        JitterFilter();
        ~JitterFilter();

        ysTransform Transform;

        void Update();

    protected:
        ysTransform *m_inputTransform;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_JITTER_FILTER_H */
