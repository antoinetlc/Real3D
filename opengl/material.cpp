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
 * \file material.cpp
 * \brief Implementation of the Material object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an object material for reflections (Only used for simple Phong and Cook Torrance BRDFs).
 * These properties have to be read from reflectance maps to get a realistic renderings.
 */


#include "opengl/material.h"

using namespace std;

/**
 * Default material constructor.
 * @brief Material
 */
Material::Material(): m_ambientColor(QColor()), m_diffuseColor(QColor()), m_specularColor(QColor()), m_ambientCoefficient(0), m_diffuseCoefficient(0), m_specularCoefficient(0), m_shininess(0)
{

}

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
Material::Material(QColor ambient, QColor diffuseColor, QColor specularColor, float ambientCoefficient, float diffuseCoefficient, float specularCoefficient, float shininess):
    m_ambientColor(QColor(ambient)), m_diffuseColor(QColor(diffuseColor)), m_specularColor(QColor(specularColor)),
    m_ambientCoefficient(ambientCoefficient), m_diffuseCoefficient(diffuseCoefficient), m_specularCoefficient(specularCoefficient), m_shininess(shininess)
{

}

/**
 * Returns the ambient color.
 * @brief getAmbientColor
 * @return
 */
QColor Material::getAmbientColor()
{
    return m_ambientColor;
}

/**
 * Returns the diffuse color.
 * @brief getDiffuseColor
 * @return
 */
QColor Material::getDiffuseColor()
{
    return m_diffuseColor;
}


/**
 * Returns the specular color.
 * @brief getDiffuseColor
 * @return
 */
QColor Material::getSpecularColor()
{
    return m_specularColor;
}

/**
 * Returns the ambient reflection coefficient.
 * @brief getAmbientCoefficient
 * @return
 */
float Material::getAmbientCoefficient()
{
    return m_ambientCoefficient;
}

/**
 * Returns the diffuse reflection coefficient.
 * @brief getDiffuseCoefficient
 * @return
 */
float Material::getDiffuseCoefficient()
{
    return m_diffuseCoefficient;
}

/**
 * Returns the specular reflection coefficient.
 * @brief getSpecularCoefficient
 * @return
 */
float Material::getSpecularCoefficient()
{
    return m_specularCoefficient;
}

/**
 * Returns the shininess of the material.
 * @brief getShininess
 * @return
 */
float Material::getShininess()
{
    return m_shininess;
}

