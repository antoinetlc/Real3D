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
 * \file light.h
 * \brief Implementation of the Light object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a point light source.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class Light
{
    public:
        /**
         * Default constructor.
         * @brief Light
         */
        Light();

        /**
         * Creates a point light source at a given position with a given color and intensity.
         * @brief Light
         * @param lightPosition
         * @param lightColor
         * @param lightIntensity
         */
        Light(QVector4D lightPosition, QVector3D lightColor, float lightIntensity);

        /**
         * Sets the position of the light.
         * @brief setPosition
         * @param x
         * @param y
         * @param z
         */
        void setPosition(float x, float y , float z);

        /**
         * Translates the light source position along the x axis.
         * @brief translateX
         * @param translationX
         */
        void translateX(float translationX);

        /**
         * Translates the light source position along the y axis.
         * @brief translateY
         * @param translationY
         */
        void translateY(float translationY);

        /**
         * Translates the light source position along the z axis.
         * @brief translateZ
         * @param translationZ
         */
        void translateZ(float translationZ);

        /**
         * Returns the light position.
         * @brief getLightPosition
         * @return
         */
        QVector4D getLightPosition() const;    

        /**
         * Returns the model matrix of the light source.
         * @brief getModelMatrix
         * @return
         */
        QMatrix4x4 getModelMatrix() const;


    private:
        QVector4D m_lightPosition; /*!< Light position */
        QMatrix4x4 m_modelMatrix; /*!< Light model matrix*/
        QVector3D m_lightColor; /*!< Light color*/
        float m_lightIntensity; /*!< Light intensity*/
};

#endif // LIGHT_H
