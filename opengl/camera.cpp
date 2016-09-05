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
 * \file camera.cpp
 * \brief Implementation of the Camera object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of the Camera object.
 */

#include "camera.h"

using namespace std;

/**
 * Default constructor of the camera class.
 * @brief Camera
 */
Camera::Camera():
    m_position(QVector4D()), m_upVector(QVector4D()), m_center(QVector4D()),
    m_perspectiveCamera(false), m_aspectRatio(1.0), m_fieldOfView(0.0), m_viewMatrix(QMatrix4x4())
{
    m_viewMatrix.setToIdentity();
    m_projectionMatrix.setToIdentity();
}

/**
 * Setups a camera at a position, with a given upVector and looking at a center point.
 * The camera can be an orthographic camera or perspective camera with an aspect ratio and a field of view.
 * @brief Camera
 * @param position
 * @param upVector
 * @param center
 * @param perspectiveCamera
 * @param aspectRatio
 * @param fieldOfView
 */
Camera::Camera(QVector4D position, QVector4D upVector, QVector4D center,
               bool perspectiveCamera, float aspectRatio , float fieldOfView):
    m_position(position), m_upVector(upVector), m_center(center),
    m_perspectiveCamera(perspectiveCamera), m_aspectRatio(aspectRatio), m_fieldOfView(fieldOfView), m_viewMatrix(QMatrix4x4())
{

    this->viewMatrix(m_position, m_upVector, m_center);
    this->projectionMatrix(m_aspectRatio, m_fieldOfView);
}

/**
 * Creates the view matrix given a position, a up vector and a center.
 * @brief viewMatrix
 * @param position
 * @param upVector
 * @param center
 */
void Camera::viewMatrix(QVector4D position, QVector4D upVector, QVector4D center)
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(position.toVector3D(), center.toVector3D(), upVector.toVector3D());
}


/**
 * Creates a projection matrix with an aspect ratio and a field of view in the case of a perspective camera.
 * @brief projectionMatrix
 * @param aspectRatio
 * @param fieldOfView
 */
void Camera::projectionMatrix(float aspectRatio, float fieldOfView)
{
    //Perspective function multiplies by a matrix : need to reinitialize the matrix to identity first
    m_projectionMatrix.setToIdentity();

    if(m_perspectiveCamera)//If it is a perspective camera
        m_projectionMatrix.perspective(fieldOfView, aspectRatio, (float)0.001, (float)10000.0);
    else
        m_projectionMatrix.ortho(-0.5, 0.5, -0.5, 0.5, 0.001, 10000.0);

}

/**
 * Rotates the camera along the X axis.
 * @brief rotateX
 * @param xRotation
 */
void Camera::rotateX(float xRotation)
{

    //Calculate new position and upVector
    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(xRotation, 1.0, 0.0, 0.0);

    QVector4D newPosition = cameraTransformation*m_position;// = m_position;
    QVector4D newUpVector = cameraTransformation*m_upVector;// = m_position;

    m_position = newPosition;
    m_upVector = newUpVector;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(newPosition.toVector3D(), m_center.toVector3D(), newUpVector.toVector3D());
}

/**
 * Rotates the camera along the Y axis.
 * @brief rotateY
 * @param yRotation
 */
void Camera::rotateY(float yRotation)
{

    //Calculate new position and upVector
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(yRotation, 0.0, 1.0, 0.0);

    QVector4D newPosition = cameraTransformation*m_position;// = m_position;
    QVector4D newUpVector = cameraTransformation*m_upVector;// = m_position;

    m_position = newPosition;
    m_upVector = newUpVector;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(newPosition.toVector3D(), m_center.toVector3D(), newUpVector.toVector3D());
}

/**
 * Translates the camera along the axis cameraPosition-cameraCenter
 * @brief translateAlongOriginAxis
 * @param zTranslation
 */
void Camera::translateAlongOriginAxis(float translation)
{

    //Calculate new position
    QVector4D newPosition = translation*(m_position-m_center);
    m_position = newPosition;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position.toVector3D(), m_center.toVector3D(), m_upVector.toVector3D());
}

/**
 * Reset the camera position, upVector and center.
 * @brief resetCamera
 */
void Camera::resetCamera()
{

    m_position = QVector4D(0.0,0.0, 5.0, 1.0);
    m_upVector = QVector4D(0.0,1.0,0.0, 1.0);
    m_center = QVector4D(0.0, 0.0,0.0, 1.0);

    this->viewMatrix(m_position, m_upVector, m_center);
}

/**
 * Returns the camera view matrix.
 * @brief getViewMatrix
 * @return
 */
QMatrix4x4 Camera::getViewMatrix()
{
    return m_viewMatrix;
}

/**
 * Returns the camera projection matrix.
 * @brief getProjectionMatrix
 * @return
 */
QMatrix4x4 Camera::getProjectionMatrix()
{
    return m_projectionMatrix;
}

/**
 * Returns the camera position.
 * @brief getPosition
 * @return
 */
QVector4D Camera::getPosition()
{
    return m_position;
}

/**
 * Returns the camera up vector
 * @brief getUpVector
 * @return
 */
QVector4D Camera::getUpVector()
{
    return m_upVector;
}

/**
 * Returns the camera center.
 * @brief getCenter
 * @return
 */
QVector4D Camera::getCenter()
{
    return m_center;
}

