#version 400
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
 * \file texture.fsh
 * \brief Fragment shader for the texture mapping.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Fragment shader for the texture mapping. Maps the input texture on the geometry.
 */
 
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D textureRendered;

in vec2 varyingTextureCoordinate;

out vec4 fragColor;

void main(void)
{
	fragColor = texture2D(textureRendered, varyingTextureCoordinate.st);
}



