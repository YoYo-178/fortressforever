#include "cbase.h"

extern "C" {
    long _dtol3(const double x) {
        return (long)_mm_cvttsd_si32(_mm_set_sd(x));
    }

    double _ltod3(const long x) {
        return (double)_mm_cvttsd_si32(_mm_set_sd(x));
    }

    long _ftol3(const float x) {
        return (long)_mm_cvttsd_si32(_mm_set_sd(x));
    }
}