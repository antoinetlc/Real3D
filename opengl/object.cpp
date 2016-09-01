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
 * \file object.cpp
 * \brief Implementation of a Object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an Object. Stores the mesh and textures.
 */

#include "opengl/object.h"

using namespace std;


/**
 * Default Object constructor.
 * @brief Object
 */
Object::Object(): m_mesh(Mesh()), m_material(Material()), m_modelMatrix(QMatrix4x4()),
    m_diffuseTexture(Texture()), m_specularTexture(),
    m_normalMap(Texture()), m_roughnessMap(Texture())
{

}

/**
 * Loads an object from its name.
 * @brief Object::Object
 * @param objectName
 */
Object::Object(string objectName): m_mesh(Mesh()), m_material(Material()), m_modelMatrix(QMatrix4x4()),
m_diffuseTexture(Texture()), m_specularTexture(Texture()),
m_normalMap(Texture()), m_roughnessMap(Texture())
{
    m_mesh = Mesh(objectName);
    m_material = Material(QColor(128,128,0), QColor(128,128,0), QColor(255,255,255), (float) 0.1, (float) 1.0, (float)1.0, (float)500.0);

    m_modelMatrix = QMatrix4x4();
    m_modelMatrix.setToIdentity();
}


/**
  * Destructor.
  */
Object::~Object()
{

}

/**
 * Loads the textures of the object.
 * Returns true if the textures were correctly loaded.
 * @brief loadTextures
 * @return
 */
bool Object::loadTextures()
{
    //Load the textures
    bool diffuseTextureLoaded = false;
    bool specularTextureLoaded = false;
    bool normalMapLoaded = false;
    bool roughnessMapLoaded = false;

    normalMapLoaded = m_normalMap.load_8UC3();
    diffuseTextureLoaded = m_diffuseTexture.load_32FC3();
    specularTextureLoaded = m_specularTexture.load_32FC3();
    roughnessMapLoaded = m_roughnessMap.load_32FC3();

    bool correctlyLoaded = (diffuseTextureLoaded && specularTextureLoaded && normalMapLoaded && roughnessMapLoaded);

    return correctlyLoaded;
}

/**
 * Resets the model matrix of the object to the identity.
 * @brief resetModelMatrix
 */
void Object::resetModelMatrix()
{
     m_modelMatrix.setToIdentity();
}

/**
 * Scales the object in every dimension.
 * @brief Object::scale
 * @param scaling
 */
void Object::scale(float scaling)
{
    m_modelMatrix.scale(scaling, scaling);
}

/**
 * Sets the aspect ratio of the object automatically if a texture is loaded.
 * @brief setAspectRatio
 */
void Object::setAspectRatio()
{
    //All the texture are supposed to have the same aspect ratio = width/height
    float aspectRatio = 1.0;

    //Choose the aspect ratio depending on which texture is loaded
    if(m_diffuseTexture.isLoaded())
    {
        aspectRatio = m_diffuseTexture.getAspectRatio();
    }
    else if(m_specularTexture.isLoaded())
    {
        aspectRatio = m_diffuseTexture.getAspectRatio();
    }
    else if(m_normalMap.isLoaded())
    {
        aspectRatio = m_diffuseTexture.getAspectRatio();
    }
    else if(m_roughnessMap.isLoaded())
    {
        aspectRatio = m_diffuseTexture.getAspectRatio();
    }

    m_modelMatrix.scale(1.0, 1.0/aspectRatio);
}

/**
 * Sets the aspect ratio of the object to a given size.
 * @brief setAspectRatio
 * @param aspectRatio
 */
void Object::setAspectRatio(float aspectRatio)
{
    m_modelMatrix.scale(1.0, 1.0/aspectRatio);
}

/**
 * The the position and the rotation of the object.
 * @brief setPositionAndRotation
 * @param x
 * @param y
 * @param z
 * @param angleX
 * @param angleY
 * @param angleZ
 */
void Object::setPositionAndRotation(float x, float y, float z, int angleX, int angleY, int angleZ)
{
    //Set the translation
    m_modelMatrix.setToIdentity();
    this->setAspectRatio();
    m_modelMatrix.translate(x, y, z);

    //Set the rotation
    angleX %= 360;
    angleY %= 360;
    angleZ %= 360;
    this->rotateX(angleX);
    this->rotateY(angleY);
    this->rotateZ(angleZ);
}


/**
 * Rotates the object around the X axis.
 * @brief rotateX
 * @param angleX
 */
void Object::rotateX(int angleX)
{
   angleX %= 360;
   m_modelMatrix.rotate(angleX, QVector3D(1.0,0.0,0.0));
}

/**
 * Rotates the object around the Y axis.
 * @brief rotateY
 * @param angleY
 */
void Object::rotateY(int angleY)
{
    angleY %= 360;
    m_modelMatrix.rotate(angleY, QVector3D(0.0,1.0,0.0));
}

/**
 * Rotates the object arounds the Z axis.
 * @brief rotateZ
 * @param angleZ
 */
void Object::rotateZ(int angleZ)
{
    angleZ %= 360;
    m_modelMatrix.rotate(angleZ, QVector3D(0.0,0.0,1.0));
}

/**
 * Loads the diffuse texture given a path of an image file.
 * Returns true if the texture was correctly loaded.
 * @brief loadDiffuseTexture
 * @param filePath
 */
bool Object::loadDiffuseTexture(const std::string filePath)
{
    m_diffuseTexture = Texture(filePath);

    bool loaded = false;

    //PFM for HDR textures
    if(filePath.substr(filePath.size()-3, 3) == string("pfm"))
    {
        loaded = m_diffuseTexture.load_32FC3();
    }
    else
    {
        loaded = m_diffuseTexture.load_8UC3();
    }

    return loaded;
}

/**
 * Loads the specular texture given a path of an image file.
 * Returns true if the texture was correctly loaded.
 * @brief loadDiffuseTexture
 * @param filePath
 */
bool Object::loadSpecularTexture(const std::string filePath)
{
    m_specularTexture = Texture(filePath);

    bool loaded = false;

    //PFM for HDR textures
    if(filePath.substr(filePath.size()-3, 3) == string("pfm"))
    {
        loaded = m_specularTexture.load_32FC3();
    }
    else
    {
        loaded = m_specularTexture.load_8UC3();
    }

    return loaded;
}

/**
 * Loads a normal map given a path of an image file.
 * Returns true if the texture was correctly loaded.
 * @brief loadNormalMap
 * @param filePath
 * @return
 */
bool Object::loadNormalMap(const std::string filePath)
{
    m_normalMap = Texture(filePath);

    bool loaded = false;

    //PFM for HDR textures
    if(filePath.substr(filePath.size()-3, 3) == string("pfm"))
    {
        loaded = m_normalMap.load_32FC3();
    }
    else
    {
        loaded = m_normalMap.load_8UC3();
    }

    return loaded;
}

/**
 * Loads a roughness given a path of an image file.
 * Returns true if the texture was correctly loaded.
 * @brief loadRoughnessMap
 * @param filePath
 * @return
 */
bool Object::loadRoughnessMap(const std::string filePath)
{
    m_roughnessMap = Texture(filePath);

    bool loaded = false;

    //PFM for HDR textures
    if(filePath.substr(filePath.size()-3, 3) == string("pfm"))
    {
        loaded = m_roughnessMap.load_32FC3();
    }
    else
    {
        loaded = m_roughnessMap.load_8UC3();
    }

    return loaded;
}

/**
 * Returns the object material.
 * @brief getMaterial
 * @return
 */
Material Object::getMaterial() const
{
   return m_material;
}

/**
 * Returns the object mesh.
 * @brief getMesh
 * @return
 */
Mesh Object::getMesh() const
{
   return m_mesh;
}

/**
 * Returns the object model matrix.
 * @brief getModelMatrix
 * @return
 */
QMatrix4x4 Object::getModelMatrix() const
{
    return m_modelMatrix;
}


/**
 * Returns the object texture coordinates.
 * @brief getTextureCoordinates
 * @return
 */
QVector<QVector2D> Object::getTextureCoordinates() const
{
    return m_mesh.getTextureCoordinates();
}


/**
 * Returns the object diffuse texture.
 * @brief getDiffuseTexture
 * @return
 */
Texture Object::getDiffuseTexture() const
{
    return m_diffuseTexture;
}

/**
 * Returns the object specular texture.
 * @brief getSpecularTexture
 * @return
 */
Texture Object::getSpecularTexture() const
{
    return m_specularTexture;
}

/**
 * Returns the object normal map texture.
 * @brief getNormalMap
 * @return
 */
Texture Object::getNormalMap() const
{
    return m_normalMap;
}

/**
 * Returns the object roughness map texture.
 * @brief getNormalMap
 * @return
 */
Texture Object::getRoughnessMap() const
{
    return m_roughnessMap;
}
