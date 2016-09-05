/*
 *     Real3D
 *
 *     Author:  Antoine TOISOUL LE CANN
 *
 *     Copyright © 2016 Antoine TOISOUL LE CANN, Imperial College London
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
 * \file material.h
 * \brief Implementation of the Material object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an object material for reflections (Only used for simple Phong and Cook Torrance BRDFs).
 * These properties have to be read from reflectance maps to get a realistic renderings.
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>

//This class represent the material of the object.
//Note that it is used for a phong/cook torrance shaders.
//However to get a realistic rendering these properties have to be read from the reflectance maps.
class Material
{
    public:
        /**
         * Default material constructor.
         * @brief Material
         */
        Material();

        /**
         * Creates a material with an ambient, a diffuse and a specular reflection color and parameters.
         * @brief Material
         * @param ambientColor
         * @param diffuseColor
         * @param specularColor
         * @param ambientCoefficient
         * @param diffuseCoefficient
         * @param specularCoefficient
         * @param shininess
         */
        Material(QColor ambientColor, QColor diffuseColor, QColor specularColor, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float shininess);

        /**
         * Returns the ambient color.
         * @brief getAmbientColor
         * @return
         */
        QColor getAmbientColor();

        /**
         * Returns the diffuse color.
         * @brief getDiffuseColor
         * @return
         */
        QColor getDiffuseColor();

        /**
         * Returns the specular color.
         * @brief getDiffuseColor
         * @return
         */
        QColor getSpecularColor();

        /**
         * Returns the ambient reflection coefficient.
         * @brief getAmbientCoefficient
         * @return
         */
        float getAmbientCoefficient();

        /**
         * Returns the diffuse reflection coefficient.
         * @brief getDiffuseCoefficient
         * @return
         */
        float getDiffuseCoefficient();

        /**
         * Returns the specular reflection coefficient.
         * @brief getSpecularCoefficient
         * @return
         */
        float getSpecularCoefficient();

        /**
         * Returns the shininess of the material.
         * @brief getShininess
         * @return
         */
        float getShininess();

    private:
        QColor m_ambientColor;
        QColor m_diffuseColor; /*!< Diffuse color of the material */
        QColor m_specularColor; /*!< Specular color of the material */
        float m_ambientCoefficient; /*!< Ambient coefficient (scaling factor) of the material */
        float m_diffuseCoefficient; /*!< Diffuse coefficient (scaling factor) of the material */
        float m_specularCoefficient; /*!< Specular coefficient (scaling factor) of the material */
        float m_shininess; /*!< Shininess of the material */

};

#endif // MATERIAL_H
