#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace cMath {

	float degToRad(float deg) {
		return (M_PI / 180) * deg;
	}
}