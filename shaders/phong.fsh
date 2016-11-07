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
 * \file phong.fsh
 * \brief Fragment shader for the Phong BRDF.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Fragment shader for the Phong BRDF.
 */

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform float shininess;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

in vec3 varyingNormal_camSpace;
in vec3 varyingLightDirection_camSpace;
in vec3 varyingViewingDirection_camSpace;
in vec2 varyingTextureCoordinates;

out vec4 fragColor;

//Fragment shader compute the final color
void main(void)
{
    vec3 normal = normalize(varyingNormal_camSpace);
    vec3 lightDirection = normalize(varyingLightDirection_camSpace);
    vec3 viewingDirection = normalize(varyingViewingDirection_camSpace);
 
    vec4 diffuseIllumination = diffuseCoefficient*diffuseColor;
    vec4 specularIllumination = specularCoefficient*specularColor;

    //Phong shading
    //ambient+diffuse+specular
    vec3 reflectionVector = -lightDirection -2.0*dot(normal, -lightDirection)*normal;
    fragColor =  max(0.0, dot(normal, lightDirection))*diffuseIllumination
            + pow(max(0.0, dot(reflectionVector, viewingDirection)), shininess)*specularIllumination;
}
