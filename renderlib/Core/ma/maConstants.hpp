/*****************************************************************************
**  maConstants.hpp
**
**      This file contains the constants for the Ma package.
**
**
**
\****************************************************************************/

#ifdef MA_CONSTANTS_HPP
#error maConstants.hpp multiply included
#endif
#define MA_CONSTANTS_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//	I tried to use <limits> here but ran into stupid warnings
//	generated because Microsoft can't follow the C++ standard...sigh.
//
#include <float.h>
#include <limits>

//============================================================================
//============================================================================
namespace maConstants {
//------------------------------------------------------------------------
//	Constants
//------------------------------------------------------------------------
const double c_dPI = 3.14159265358979323846;
const double c_dPI_Times_2 = c_dPI * 2.0;
const double c_dPI_Times_4 = c_dPI * 4.0;
const double c_dPI_Div_2 = c_dPI / 2.0;
const double c_dPI_Inverse = 1.0 / c_dPI;
const double c_dRadToAngle = 180.0 / c_dPI;
const double c_dAngleToRad = c_dPI / 180.0;

const float c_fPI = 3.141592653589f;
const float c_fPI_Times_2 = c_fPI * 2.0f;
const float c_fPI_Times_4 = c_fPI * 4.0f;
const float c_fPI_Div_2 = c_fPI / 2.0f;
const float c_fPI_Inverse = 1.0f / c_fPI;
const float c_fRadToAngle = 180.0f / c_fPI;
const float c_fAngleToRad = c_fPI / 180.0f;

const double c_dEpsilon = 1.0e-6;
const float c_fEpsilon = 1.0e-6f;

const float c_fRoundOff = 1.0e-3f; // float round-off tolerance

const float c_fLargest = FLT_MAX;
const double c_dLargest = DBL_MAX;
}
