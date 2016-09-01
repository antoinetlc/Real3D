/*
 *     Real3D
 *
 *     Author:  Antoine TOISOUL LE CANN
 *
 *     Copyright © 2016 Antoine TOISOUL LE CANN
 *              All rights reserved
 *
 *
 * Real3D is free software: you can redistribute it and/or modify
 *
 * it under the terms of the GNU Lesser General Public License as published by
 *
 * the Free Software Foundation, either version 3 of the License, or
 *
 * (at your option) any later version.
 *
 * Real3D is distributed in the hope that it will be useful,
 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file mathfunctions.h
 * \brief Implementation of mathematical functions.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a clamp function.
 */

#ifndef MATHFUNCTIONS
#define MATHFUNCTIONS

#define M_PI 3.14159265358979323846

#include <iostream>
#include <cmath>
#include <vector>

#include <QMatrix3x3>
#include <QVector3D>

/*---- OpenCV ----*/
#include <opencv2/core/core.hpp>

#include <omp.h>

/**
 * Clamp a number so it is in the range [inf ; sup].
 * If number<inf, then the function returns inf.
 * If number>sup, then the function returns sup.
 * If number is in the range [inf ; sup] then the function returns the number.
 * @param INPUT : value is a float which will be clamped.
 * @param INPUT : inf is the lower bound of the range.
 * @param INPUT : sup is the upper bound of the range.
 * @return The clamped value in the range [inf ; sup]. If sup < inf then the function returns the number.
 */
float clamp(float value, float inf, float sup);

#endif // MATHFUNCTIONS

