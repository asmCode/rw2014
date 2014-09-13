#pragma once

#include "BlinkCurve.h"
#include "../QuadIn.h"
#include "../QuadOut.h"

typedef BlinkCurve<float, QuadOut<float>, QuadIn<float> > QuadBlinkCurve;
