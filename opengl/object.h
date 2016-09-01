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
 * \file object.h
 * \brief Implementation of a Object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an Object. Stores the mesh and textures.
 */


#ifndef OBJECT_H
#define OBJECT_H

#include "opengl/mesh.h"
#include "opengl/material.h"
#include "opengl/texture.h"

#include <QApplication>
#include <QVector3D>
#include <QMatrix4x4>
#include <QDir>

#include <string>
#include <sstream>

class Object
{
    public:
        /**
         * Default Object constructor.
         * @brief Object
         */
        Object();

        /**
         * Loads an object from its name.
         * @brief Object::Object
         * @param objectName
         */     
        Object(std::string objectName);

        /**
          * Destructor.
          */
        ~Object();

        /**
         * Loads the textures of the object.
         * Returns true if the textures were correctly loaded.
         * @brief loadTextures
         * @return
         */
        bool loadTextures();

        /**
         * Resets the model matrix of the object to the identity.
         * @brief resetModelMatrix
         */
        void resetModelMatrix();

        /**
         * Scales the object in every dimension.
         * @brief Object::scale
         * @param scaling
         */
        void scale(float scaling);

        /**
         * Sets the aspect ratio of the object automatically if a texture is loaded.
         * @brief setAspectRatio
         */
        void setAspectRatio();

        /**
         * Sets the aspect ratio of the object to a given size.
         * @brief setAspectRatio
         * @param aspectRatio
         */
        void setAspectRatio(float aspectRatio);

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
        void setPositionAndRotation(float x, float y, float z, int angleX, int angleY, int angleZ);

        /**
         * Rotates the object around the X axis.
         * @brief rotateX
         * @param angleX
         */
        void rotateX(int angleX);

        /**
         * Rotates the object around the Y axis.
         * @brief rotateY
         * @param angleY
         */
        void rotateY(int angleY);

        /**
         * Rotates the object arounds the Z axis.
         * @brief rotateZ
         * @param angleZ
         */
        void rotateZ(int angleZ);

        /**
         * Loads the diffuse texture given a path of an image file.
         * Returns true if the texture was correctly loaded.
         * @brief loadDiffuseTexture
         * @param filePath
         */
        bool loadDiffuseTexture(const std::string filePath);

        /**
         * Loads the specular texture given a path of an image file.
         * Returns true if the texture was correctly loaded.
         * @brief loadDiffuseTexture
         * @param filePath
         */
        bool loadSpecularTexture(const std::string filePath);

        /**
         * Loads a normal map given a path of an image file.
         * Returns true if the texture was correctly loaded.
         * @brief loadNormalMap
         * @param filePath
         * @return
         */
        bool loadNormalMap(const std::string filePath);

        /**
         * Loads a roughness given a path of an image file.
         * Returns true if the texture was correctly loaded.
         * @brief loadRoughnessMap
         * @param filePath
         * @return
         */
        bool loadRoughnessMap(const std::string filePath);

        /**
         * Returns the object mesh.
         * @brief getMesh
         * @return
         */
        Mesh getMesh() const;

        /**
         * Returns the object material.
         * @brief getMaterial
         * @return
         */
        Material getMaterial() const;

        /**
         * Returns the object model matrix.
         * @brief getModelMatrix
         * @return
         */
        QMatrix4x4 getModelMatrix() const;

        /**
         * Returns the object texture coordinates.
         * @brief getTextureCoordinates
         * @return
         */
        QVector<QVector2D> getTextureCoordinates() const;

        /**
         * Returns the object diffuse texture.
         * @brief getDiffuseTexture
         * @return
         */
        Texture getDiffuseTexture() const;

        /**
         * Returns the object specular texture.
         * @brief getSpecularTexture
         * @return
         */
        Texture getSpecularTexture() const;

        /**
         * Returns the object normal map texture.
         * @brief getNormalMap
         * @return
         */
        Texture getNormalMap() const;

        /**
         * Returns the object roughness map texture.
         * @brief getNormalMap
         * @return
         */
        Texture getRoughnessMap() const;

    private:
        Mesh m_mesh;/*!< Object mesh. */
        Material m_material; /*!< Object material */
        QMatrix4x4 m_modelMatrix; /*!< Object model matrix */

        Texture m_diffuseTexture; /*!< Object diffuse texture */
        Texture m_specularTexture; /*!< Object specular texture */
        Texture m_normalMap; /*!< Object normal map */
        Texture m_roughnessMap; /*!< Object roughness map */
};

#endif // OBJECT_H
