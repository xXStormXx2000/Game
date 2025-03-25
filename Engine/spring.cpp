#include "spring.h"

float spring(float vel, float pos, float tpos, float sptif, float demp) {
    return -(tpos - pos) * sptif + vel * demp;
}
