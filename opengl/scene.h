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
 * \file scene.h
 * \brief Implementation of a Scene.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a Scene with Objects, point light sources and an environment map.
 */

#ifndef SCENE_H
#define SCENE_H

#include "opengl/object.h"
#include "opengl/light.h"

#include <QVector>
#include <QVector4D>

class Scene
{
    public:
        /**
         * Default scene constructor.
         * Creates a scene with a square object and a single point light source.
         * @brief Scene
         */
        Scene();

        /**
         * Loads a scene with several objects and point light sources given by the input vectors.
         * @brief Scene
         * @param listOfObjectNames
         * @param listOfPointLights
         */
        Scene(QVector<std::string>& listOfObjectNames, const QVector<Light> &listOfPointLights);

        /**
          * Destructor
          */
        ~Scene();

        /**
         * Build the scene by loading the geometry and setting the light sources.
         * @brief buildScene
         */
        void buildScene();

        /**
         * Remove all the objects.
         * @brief removeObjects
         */
        void removeObjects();

        /*---Geometric transformations--*/
        /**
         * translate light source lightNumber along the X axis by the amount translationX.
         * @brief translateLightSourceX
         * @param lightNumber
         * @param translationX
         */
        void translateLightSourceX(const int lightNumber, const float translationX);

        /**
         * translate light source lightNumber along the Y axis by the amount translationY.
         * @brief translateLightSourceY
         * @param lightNumber
         * @param translationY
         */
        void translateLightSourceY(const int lightNumber, const float translationY);

        /**
         * translate light source lightNumber along the Z axis by the amount translationZ.
         * @brief translateLightSourceZ
         * @param lightNumber
         * @param translationZ
         */
        void translateLightSourceZ(const int lightNumber, const float translationZ);

        /**
         * Sets light source lightNumber to a given position.
         * @brief setLightSourcePosition
         * @param lightNumber
         * @param x
         * @param y
         * @param z
         */
        void setLightSourcePosition(const int lightNumber, const float x, const float y, const float z);

        /**
         * Rotate the object objectNumber around the X axis by the angle rotationX.
         * @brief rotateObjectX
         * @param objectNumber
         * @param rotationX
         */
        void rotateObjectX(const int objectNumber, const int rotationX);

        /**
         * Rotate the object objectNumber around the Y axis by the angle rotationY.
         * @brief rotateObjectY
         * @param objectNumber
         * @param rotationY
         */
        void rotateObjectY(const int objectNumber, const int rotationY);

        /**
         * Rotate the object objectNumber around the Z axis by the angle rotationZ.
         * @brief rotateObjectZ
         * @param objectNumber
         * @param rotationZ
         */
        void rotateObjectZ(const int objectNumber, const int rotationZ);

        /**
         * Reset the object model matrix to identity.
         * @brief resetTransformationsObjects
         */
        void resetTransformationsObjects();

        /**
         * Reset the objects and the lights to their original position
         * @brief resetScene
         */
        void resetScene();

        /**
         * Load the textures of the objects.
         * @brief loadTexturesObjects
         */
        void loadTexturesObjects();

        /**
         * Set the aspect ratio of the objects.
         * @brief setAspectRatiosObjects
         */
        void setAspectRatiosObjects();

        /**
         * Loads the diffuse map of object objectNumber.
         * returns true if the texture was correctly loaded.
         * @brief loadDiffuseMap
         * @param filePath
         * @param objectNumber
         * @return
         */
        bool loadDiffuseMap(const std::string filePath, const int objectNumber);

        /**
         * Loads the specular map of object objectNumber.
         * returns true if the texture was correctly loaded.
         * @brief loadSpecularMap
         * @param filePath
         * @param objectNumber
         * @return
         */
        bool loadSpecularMap(const std::string filePath, const int objectNumber);

        /**
         * Loads the normal map of object objectNumber.
         * returns true if the texture was correctly loaded.
         * @brief loadNormalMap
         * @param filePath
         * @param objectNumber
         * @return
         */
        bool loadNormalMap(const std::string filePath, const int objectNumber);

        /**
         * Loads the roughness map of object objectNumber.
         * returns true if the texture was correctly loaded.
         * @brief loadRoughnessMap
         * @param filePath
         * @param objectNumber
         * @return
         */
        bool loadRoughnessMap(const std::string filePath, const int objectNumber);

        /**
         * Loads the environment map (EM), the EM with diffuse convolution and the EM for rough specular reflection.
         * @brief loadEnvironmentMap
         * @param EMPath
         * @param EMDiffusePath
         * @param EMRoughPath
         * @return
         */
        bool loadEnvironmentMap(const std::string EMPath, const std::string EMDiffusePath, const std::string EMRoughPath);

        /**
         * Returns an array of objects.
         * @brief getObjects
         * @return
         */
        QVector<Object> getObjects();

        /**
         * Returns an array of point light sources.
         * @brief getPointLightSources
         * @return
         */
        QVector<Light> getPointLightSources();

        /**
         * Returns the environment map ID.
         * @brief getEnvironmentMapId
         * @return
         */
        GLuint getEnvironmentMapId();

        /**
         * Returns the ID of the environment map with rough specular convolution.
         * @brief getEnvironmentMapRoughId
         * @return
         */
        GLuint getEnvironmentMapRoughId();

        /**
         * Returns the ID of the environment map with diffuse convolution.
         * @brief getEnvironmentMapDiffuseId
         * @return
         */
        GLuint getEnvironmentMapDiffuseId();

    private:
        QVector<Object> m_objects; /*!< Array of objects. */
        QVector<Light> m_pointLights; /*!< Array of point light sources. */
        Texture m_environmentMap; /*!< Texture of the environment map (latitude longitude map). */
        Texture m_environmentMapRough;  /*!< Texture of the environment map with rough specular convolution (latitude longitude map). */
        Texture m_environmentMapDiffuse; /*!< Texture of the environment map with diffuse convolution (latitude longitude map). */
};

#endif // SCENE_H
