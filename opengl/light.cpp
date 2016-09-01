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
 * \file light.cpp
 * \brief Implementation of the Light object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a point light source.
 */

#include "opengl/light.h"

using namespace std;

/**
 * Default constructor.
 * @brief Light
 */
Light::Light():
    m_lightPosition(QVector4D()), m_modelMatrix(QMatrix4x4()), m_lightColor(QVector3D()), m_lightIntensity()
{

}

/**
 * Creates a point light source at a given position with a given color and intensity.
 * @brief Light
 * @param lightPosition
 * @param lightColor
 * @param lightIntensity
 */
Light::Light(QVector4D lightPosition, QVector3D lightColor,float lightIntensity):
    m_lightPosition(QVector4D(lightPosition)), m_modelMatrix(QMatrix4x4()), m_lightColor(QVector3D(lightColor)),
    m_lightIntensity(lightIntensity)
{
    m_modelMatrix.setToIdentity();
}

/**
 * Sets the position of the light.
 * @brief setPosition
 * @param x
 * @param y
 * @param z
 */
void Light::setPosition(float x, float y , float z)
{
    m_lightPosition = QVector4D(x, y ,z , 1.0);
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(x,y,z);
}

/**
 * Translates the light source position along the x axis.
 * @brief translateX
 * @param translationX
 */
void Light::translateX(float translationX)
{
    m_modelMatrix.translate(translationX, 0.0, 0.0);

    float currentPosition = m_lightPosition.x();
    m_lightPosition.setX(currentPosition+translationX);
}

/**
 * Translates the light source position along the y axis.
 * @brief translateY
 * @param translationY
 */
void Light::translateY(float translationY)
{

    m_modelMatrix.translate(0.0, translationY, 0.0);
    float currentPosition = m_lightPosition.y();
    m_lightPosition.setY(currentPosition+translationY);
}

/**
 * Translates the light source position along the z axis.
 * @brief translateZ
 * @param translationZ
 */
void Light::translateZ(float translationZ)
{ 
    m_modelMatrix.translate(0.0, 0.0, translationZ);
    float currentPosition = m_lightPosition.z();
    m_lightPosition.setZ(currentPosition+translationZ);
}

/**
 * Returns the light position.
 * @brief getLightPosition
 * @return
 */
QVector4D Light::getLightPosition() const
{
    return m_lightPosition;
}

/**
 * Returns the model matrix of the light source.
 * @brief getModelMatrix
 * @return
 */
QMatrix4x4 Light::getModelMatrix() const
{
    return m_modelMatrix;
}

