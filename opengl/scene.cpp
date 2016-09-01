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
 * \file scene.cpp
 * \brief Implementation of a Scene.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a Scene with Objects, point light sources and an environment map.
 */

#include "opengl/scene.h"

using namespace std;

/**
 * Default scene constructor.
 * Creates a scene with a square object and a single point light source.
 * @brief Scene
 */
Scene::Scene(): m_objects(QVector<Object>()), m_pointLights(QVector<Light>()),
    m_environmentMap(Texture("")),
    m_environmentMapRough(Texture("")),
    m_environmentMapDiffuse(Texture(""))
{

    buildScene();
}

/**
 * Loads a scene with several objects and point light sources given by the input vectors.
 * @brief Scene
 * @param listOfObjectNames
 * @param listOfPointLights
 */
Scene::Scene(QVector<string>& listOfObjectNames, const QVector<Light> &listOfPointLights):
    m_objects(QVector<Object>()),  m_pointLights(listOfPointLights)
{
    for(int i = 0 ; i<listOfObjectNames.size() ; i++)
    {
       m_objects.push_back(Object(listOfObjectNames[i]));
    }
}


/**
  * Destructor
  */
Scene::~Scene()
{

}

/**
 * Build the scene by loading the geometry and setting the light sources.
 * @brief buildScene
 */
void Scene::buildScene()
{
    Object ground = Object(string("square"));
   // ground.rotateX(-90);
    m_objects.push_back(ground);

    //Be careful not to put the light inside the object
    m_pointLights.push_back(Light(QVector4D(0.0,0.0,30.0, 1.0), QVector3D(1.0,1.0,1.0), 1.0));
}

/**
 * Remove all the objects.
 * @brief removeObjects
 */
void Scene::removeObjects()
{
    m_objects.clear();
}

/**
 * Reset the objects and the lights to their original position
 * @brief resetScene
 */
void Scene::resetScene()
{
    //Reset the objects
    for(int k = 0 ; k<m_objects.size() ; k++)
    {
        m_objects[k].setPositionAndRotation(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    //Reset the lights
    //Be careful not to put the light inside the object
    for(int k = 0 ; k<m_objects.size() ; k++)
    {
       m_pointLights[k].setPosition(0.0, 0.0, 30.0);
    }

}

/**
 * Load the textures of the objects.
 * @brief loadTexturesObjects
 */
void Scene::loadTexturesObjects()
{
    for(int k = 0 ; k<m_objects.size() ; k++)
    {
        m_objects[k].loadTextures();

        //Set the aspect ratio after loading the texture
        m_objects[k].setAspectRatio();
        cout << "Textures of object " << k << " loaded correctly" << endl;
    }
}

/**
 * Set the aspect ratio of the objects.
 * @brief setAspectRatiosObjects
 */
void Scene::setAspectRatiosObjects()
{
    for(int k = 0 ; k<m_objects.size() ; k++)
    {
        //Set the aspect ratio after loading the texture
        m_objects[k].setAspectRatio();
        cout << "Textures of object " << k << " loaded correctly" << endl;
    }
}

/**
 * translate light source lightNumber along the X axis by the amount translationX.
 * @brief translateLightSourceX
 * @param lightNumber
 * @param translationX
 */
void Scene::translateLightSourceX(const int lightNumber, const float translationX)
{
    if(lightNumber<m_pointLights.size())
    {
        m_pointLights[lightNumber].translateX(translationX);
    }
}

/**
 * translate light source lightNumber along the Y axis by the amount translationY.
 * @brief translateLightSourceY
 * @param lightNumber
 * @param translationY
 */
void Scene::translateLightSourceY(const int lightNumber, const float translationY)
{
    if(lightNumber<m_pointLights.size())
    {
        m_pointLights[lightNumber].translateY(translationY);
    }
}

/**
 * translate light source lightNumber along the Z axis by the amount translationZ.
 * @brief translateLightSourceZ
 * @param lightNumber
 * @param translationZ
 */
void Scene::translateLightSourceZ(const int lightNumber, const float translationZ)
{
    if(lightNumber<m_pointLights.size())
    {
        m_pointLights[lightNumber].translateZ(translationZ);
    }
}

/**
 * Sets light source lightNumber to a given position.
 * @brief setLightSourcePosition
 * @param lightNumber
 * @param x
 * @param y
 * @param z
 */
void Scene::setLightSourcePosition(const int lightNumber, const float x, const float y, const float z)
{
    if(lightNumber<m_pointLights.size())
    {
        m_pointLights[lightNumber].setPosition(x,y,z);
    }
}

/**
 * Rotate the object objectNumber around the X axis by the angle rotationX.
 * @brief rotateObjectX
 * @param objectNumber
 * @param rotationX
 */
void Scene::rotateObjectX(const int objectNumber, const int rotationX)
{
    if(objectNumber<m_objects.size())
    {
        m_objects[objectNumber].rotateX(rotationX);
    }
}

/**
 * Rotate the object objectNumber around the Y axis by the angle rotationY.
 * @brief rotateObjectY
 * @param objectNumber
 * @param rotationY
 */
void Scene::rotateObjectY(const int objectNumber, const int rotationY)
{
    if(objectNumber<m_objects.size())
    {
        m_objects[objectNumber].rotateY(rotationY);
    }
}

/**
 * Rotate the object objectNumber around the Z axis by the angle rotationZ.
 * @brief rotateObjectZ
 * @param objectNumber
 * @param rotationZ
 */
void Scene::rotateObjectZ(const int objectNumber, const int rotationZ)
{
    if(objectNumber<m_objects.size())
    {
        m_objects[objectNumber].rotateZ(rotationZ);
    }
}

/**
 * Reset the object model matrix to identity.
 * @brief resetTransformationsObjects
 */
void Scene::resetTransformationsObjects()
{
    for(int k = 0 ; k<m_objects.size() ; k++)
    {
        //Set the aspect ratio after loading the texture
        m_objects[k].resetModelMatrix();
        m_objects[k].setAspectRatio();
    }

}

/**
 * Loads the diffuse map of object objectNumber.
 * returns true if the texture was correctly loaded.
 * @brief loadDiffuseMap
 * @param filePath
 * @param objectNumber
 * @return
 */
bool Scene::loadDiffuseMap(const string filePath, const int objectNumber)
{
    bool loaded = false;

    if(objectNumber<m_objects.size())
    {
        loaded = m_objects[objectNumber].loadDiffuseTexture(filePath);

        if(loaded)
        {
            m_objects[objectNumber].resetModelMatrix();
            m_objects[objectNumber].setAspectRatio();
        }
    }

    return loaded;
}

/**
 * Loads the specular map of object objectNumber.
 * returns true if the texture was correctly loaded.
 * @brief loadSpecularMap
 * @param filePath
 * @param objectNumber
 * @return
 */
bool Scene::loadSpecularMap(const string filePath, const int objectNumber)
{
    bool loaded = false;

    if(objectNumber<m_objects.size())
    {
        loaded = m_objects[objectNumber].loadSpecularTexture(filePath);
    }

    return loaded;
}

/**
 * Loads the normal map of object objectNumber.
 * returns true if the texture was correctly loaded.
 * @brief loadNormalMap
 * @param filePath
 * @param objectNumber
 * @return
 */
bool Scene::loadNormalMap(const string filePath, const int objectNumber)
{
    bool loaded = false;

    if(objectNumber<m_objects.size())
    {
        loaded = m_objects[objectNumber].loadNormalMap(filePath);
    }

    return loaded;
}

/**
 * Loads the roughness map of object objectNumber.
 * returns true if the texture was correctly loaded.
 * @brief loadRoughnessMap
 * @param filePath
 * @param objectNumber
 * @return
 */
bool Scene::loadRoughnessMap(const string filePath, const int objectNumber)
{

    bool loaded = false;

    if(objectNumber<m_objects.size())
    {
        loaded = m_objects[objectNumber].loadRoughnessMap(filePath);
    }

    return loaded;
}

/**
 * Loads the environment map (EM), the EM with diffuse convolution and the EM for rough specular reflection.
 * @brief loadEnvironmentMap
 * @param EMPath
 * @param EMDiffusePath
 * @param EMRoughPath
 * @return
 */
bool Scene::loadEnvironmentMap(const string EMPath, const string EMDiffusePath, const string EMRoughPath)
{
    //Sets the file path of the EM
    m_environmentMap = Texture(EMPath);
    m_environmentMapRough = Texture(EMDiffusePath);
    m_environmentMapDiffuse = Texture(EMRoughPath);

    //Load the EMs
    bool EMLoaded = m_environmentMap.load_32FC3();
    bool EMRoughLoaded = m_environmentMapRough.load_32FC3();
    bool EMDiffuseLoaded = m_environmentMapDiffuse.load_32FC3();

    return EMLoaded && EMRoughLoaded && EMDiffuseLoaded;
}

/**
 * Returns an array of objects.
 * @brief getObjects
 * @return
 */
QVector<Object> Scene::getObjects()
{
    return m_objects;
}

/**
 * Returns an array of point light sources.
 * @brief getPointLightSources
 * @return
 */
QVector<Light> Scene::getPointLightSources()
{
    return m_pointLights;
}


/**
 * Returns the environment map ID.
 * @brief getEnvironmentMapId
 * @return
 */
GLuint Scene::getEnvironmentMapId()
{
    return m_environmentMap.getTextureId();
}

/**
 * Returns the ID of the environment map with rough specular convolution.
 * @brief getEnvironmentMapRoughId
 * @return
 */
GLuint Scene::getEnvironmentMapRoughId()
{
    return m_environmentMapRough.getTextureId();
}


/**
 * Returns the ID of the environment map with diffuse convolution.
 * @brief getEnvironmentMapDiffuseId
 * @return
 */
GLuint Scene::getEnvironmentMapDiffuseId()
{
    return m_environmentMapDiffuse.getTextureId();
}

