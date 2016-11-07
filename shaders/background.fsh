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
 * \file background.fsh
 * \brief Fragment shader to draw the background in the environment map rendering.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Fragment shader to draw the background in the environment map rendering.
 */
 
#define M_PI 3.1415926535897932384626433832795

uniform int timeMs;

uniform mat4 vMatrix;

uniform sampler2D backgroundEnvMap;

//Stam's BRDF parameters
in vec4 varyingVertex_camSpace;
in vec2 varyingTextureCoordinate;

out vec4 fragColor;

vec3 cartesianToSpherical(vec3 vector)
{
	float r = length(vector);
	float theta = acos(vector.y/r);
	float phi = mod(atan(vector.x, vector.z), 2.0*M_PI);
	
	return vec3(r, theta, phi);
}

void main(void)
{
	vec3 viewingDirection_camSpace = normalize(-varyingVertex_camSpace.xyz);
	
	vec3 viewingDirection_worldSpace = normalize((inverse(transpose(vMatrix))*vec4(viewingDirection_camSpace, 0.0)).xyz); //Cam space to world space
	vec3 viewingDirectionSpherical_worldSpace = cartesianToSpherical(viewingDirection_worldSpace);
	
	float timeScale = 0.5;
	float rotationEnvMap = float(timeMs)*0.018*M_PI/180.0*timeScale; //0.018 degrees/ms = 360 degrees in 20 s
	
	//-rotation so that it is synchronised with the reflection of the object
	float u = mod((viewingDirectionSpherical_worldSpace.z)  + rotationEnvMap +M_PI, 2.0*M_PI); // phi. Center of environment map is 0 hence +M_PI
	float v = viewingDirectionSpherical_worldSpace.y; // theta
	
	u/=2.0*M_PI;
	v/=M_PI;
			
	fragColor = texture2D(backgroundEnvMap,vec2(u,v));

	fragColor.x = pow(fragColor.x, 1.0/2.2);
	fragColor.y = pow(fragColor.y, 1.0/2.2);
	fragColor.z = pow(fragColor.z, 1.0/2.2);
}


