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
 * \file cookTorrance.fsh
 * \brief Fragment shader for the Cook Torrance BRDF.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Fragment shader for the Cook Torrance BRDF. Also implements environment mapping.
 */
 
#define M_PI 3.1415926535897932384626433832795

uniform float exposure;

uniform bool environmentMapping;
uniform int timeMs;

uniform mat4 mvMatrix;
uniform mat4 vMatrix;//viewing matrix

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_map;
uniform sampler2D roughness_map;

uniform sampler2D environmentMap;
uniform sampler2D environmentMapRough;
uniform sampler2D environmentMapDiffuse;

uniform mat3 normalMatrix; //mv matrix without translation
uniform vec3 lightPosition;

in vec4 varyingVertex_camSpace;
in vec3 varyingNormal_camSpace;
in vec4 varyingLightPosition_camSpace;
in vec2 varyingTextureCoordinate;

out vec4 fragColor;

vec3 cartesianToSpherical(vec3 vector)
{
	float r = length(vector);
	float theta = acos(vector.y/r);
	float phi = mod(atan(vector.x, vector.z), 2.0*M_PI);
	
	return vec3(r, theta, phi);
}

/*----------------------Cook Torrance microfacet model----------------------------*/

/**
 *Geometric factor for the Cook Torrance BRDF
 */
float geometricFactorCookTorrance(vec3 normal, vec3 halfVector, vec3 viewingDirection, vec3 lightDirection)
{
	float NdotH = max(dot(normal, halfVector), 0.0);
	float NdotV = max(dot(normal, viewingDirection), 0.0);
	float VdotH = max(dot(viewingDirection, halfVector), 0.0);
	float NdotL = max(dot(normal,lightDirection), 0.0);
	
	float shadowingFactor = 2.0*NdotH*NdotV/VdotH;
	float maskingFactor =  2.0*NdotH*NdotL/VdotH;
	
	float G = min(1.0, min(maskingFactor, shadowingFactor));
	
	return G;
	
}

/**
 *Beckmann distribution of microfacets slopes
 */
float beckmannSlopeDistribution(vec3 normal, vec3 halfVector, float roughness)
{
	float cos_beta = dot(normal, halfVector); 
	float beta = acos(dot(normal, halfVector));
	float NdotH = dot(normal, halfVector);
	
	float constant = 1.0/(4.0*roughness*roughness*pow(cos_beta, 4.0));

	//return constant*exp(-pow( tan(beta)/roughness,2.0));
	return constant*exp(- (1.0-NdotH*NdotH)/(NdotH*NdotH*roughness*roughness));
}

/**
 *Implementation of the Cook Torrance BRDF
 */
vec4 cookTorrance(float fresnel, float roughness, vec3 normal, vec3 viewingDirection, vec3 lightDirection, vec3 halfVector, vec4 specularColor)
{
		vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
		
		//incoming and outgoing light angles
		float cosTheta_i = max(dot(normal, lightDirection), 0.0);
		float cosTheta_o = max(dot(normal, viewingDirection), 0.0);
		
		float G = geometricFactorCookTorrance(normal, halfVector, viewingDirection, lightDirection);
		float D = beckmannSlopeDistribution(normal, halfVector, roughness);
		
		//Normally result is divided by normal dot lightDirection (cos theta i). But this terms cancels with the normal dot lightDirection of the rendering equation
		result = fresnel*D*G/(M_PI*cosTheta_o)*specularColor;
		
		return result;
}

void main(void)
{
	//The normal has to be in the camera space
	vec3 normal = normalize(normalMatrix*normalize((2.0*texture2D(normal_map, varyingTextureCoordinate.st).xyz-1.0)));
			
	//Light direction and viewing direction per fragment. 
	float distanceLightSource = length(varyingLightPosition_camSpace.xyz-varyingVertex_camSpace.xyz);
	vec3 lightDirection = normalize(varyingLightPosition_camSpace.xyz-varyingVertex_camSpace.xyz);
	vec3 viewingDirection = normalize(-varyingVertex_camSpace.xyz);
	
    //Half vector = (lightDirection+ViewingDirection)
	vec3 halfVector = normalize(lightDirection+viewingDirection);
	
	//Colors
	vec4 diffuseColor = texture2D(diffuse_texture, varyingTextureCoordinate.st);
	vec4 specularColor = texture2D(specular_texture, varyingTextureCoordinate.st);
	
	//Warning in the texture sigma squared is stored : must take the square root
	float roughness = texture2D(roughness_map, varyingTextureCoordinate.st).x;
	
	//Compute the Fresnel Factor with Schlick approximation
	float n1 = 1.0; //air
	float n2 = 1.5; //metal
	float R = (n1-n2)*(n1-n2)/((n1+n2)*(n1+n2));
	float F = R+(1.0-R)*pow((1.0-dot(halfVector, viewingDirection)),5.0);		
	
	//Timse scaling factor for EM rotation
	float timeScale = 0.5;		

	//Compute Cook Torrance BRDF
	if(!environmentMapping)
	{
		vec4 specularResponse = cookTorrance(F, roughness, normal, viewingDirection, lightDirection, halfVector, specularColor);
			
		float lightSourceDistance = length(varyingLightPosition_camSpace.xyz-varyingVertex_camSpace.xyz)/10.0;
		float lightPower = 10.0;
		float lightAttenuation = lightPower/(lightSourceDistance);
			
		fragColor += clamp(max(dot(normal,lightDirection),0.0)*diffuseColor*lightAttenuation +specularResponse, vec4(0.0,0.0,0.0,0.0), vec4(1.0,1.0,1.0,1.0));
	}
	else
	{
		//Reflection vector calculation
		vec3 reflectionVector_camSpace = normalize(2.0*dot(normal, viewingDirection)*normal-viewingDirection);
		vec3 reflectionVector_worldSpace =  normalize((inverse(vMatrix)*vec4(reflectionVector_camSpace, 0.0)).xyz); //Cam space to world space
		
		vec3 reflectionVectorSpherical_worldSpace =  cartesianToSpherical(reflectionVector_worldSpace);
			
		//add the rotation to the environment map
		float rotationEnvMap = float(timeMs)* 0.018*M_PI/180.0*timeScale; //0.018 degrees/ms = 360 degrees in 20 s
		float uReflection = mod((reflectionVectorSpherical_worldSpace.z) + rotationEnvMap, 2.0*M_PI); // phi. Center of environment map is 0 hence +M_PI
		float vReflection = reflectionVectorSpherical_worldSpace.y; // theta
			
		uReflection /= 2.0*M_PI;
		vReflection /= M_PI;
			
		//The axis theta and v and inverted compared to each other
		vec3 envMapColor = texture2D(environmentMapRough, vec2(uReflection,1.0-vReflection)).xyz;
			
		//Diffuse indexed by normal		
		vec3 normalSpherical_worldSpace =  cartesianToSpherical(vec3(0.0, 0.0, 1.0));
			
		float uDiffuse = mod((normalSpherical_worldSpace.z) + rotationEnvMap +3.0*M_PI/2.0, 2.0*M_PI); // phi. Center of environment map is 0 hence +M_PI
		float vDiffuse = normalSpherical_worldSpace.y; // theta
			
		vec3 envMapDiffuseConvolution = texture2D(environmentMapDiffuse, vec2(uDiffuse,1.0-vDiffuse)).xyz;
			
		fragColor.xyz = envMapDiffuseConvolution*diffuseColor.xyz + envMapColor*specularColor.xyz;	//The diffuse component is precomputed in the diffuse convolution term
		fragColor.xyz *= pow(2.0, exposure);
	}
	
	fragColor.xyz *= pow(2.0,exposure);
	
	fragColor = clamp(fragColor, vec4(0.0,0.0,0.0,0.0), vec4(1.0,1.0,1.0,1.0));
	//Re apply the gamma
	fragColor.x = pow(fragColor.x, 1.0/2.2);
	fragColor.y = pow(fragColor.y, 1.0/2.2);
	fragColor.z = pow(fragColor.z, 1.0/2.2);
	
}


