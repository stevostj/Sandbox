#ifndef GAMEENGINEPLUGINXPLANEWRAPPERTESTS_HELPERMETHODS_H_
#define GAMEENGINEPLUGINXPLANEWRAPPERTESTS_HELPERMETHODS_H_

#include <cmath>

namespace gep_xpw_ut {

    bool ApproximatelyEqual(float a, float b, float epsilon)
    {
        bool rv = std::fabs(a - b) <= ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
        return rv;
    }

    bool ApproximatelyEqual(double a, double b, double epsilon)
    {
        bool rv = std::fabs(a - b) <= ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
        return rv;
    }
}


#endif