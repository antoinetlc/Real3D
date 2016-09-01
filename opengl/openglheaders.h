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
 * \file openglheaders.h
 * \brief OpenGL includes.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * OpenGL includes.
 */

#ifndef OPENGLHEADERS_H
#define OPENGLHEADERS_H

#ifdef _WIN32
#include <GL/glew.h>
#endif

#ifdef __gnu_linux__
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#endif // OPENGLHEADERS_H
