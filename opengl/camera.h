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
 * \file camera.h
 * \brief Implementation of the Camera object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of the Camera object.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

#include <string>
#include <sstream>
#include <iostream>

class Camera
{
    public:

        /**
         * Default constructor of the camera class.
         * @brief Camera
         */
        Camera();

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
        Camera(QVector4D position, QVector4D upVector, QVector4D center, bool perspectiveCamera = true, float aspectRatio = 16.0/9.0, float fieldOfView = 30.0);

        /**
         * Creates the view matrix given a position, a up vector and a center.
         * @brief viewMatrix
         * @param position
         * @param upVector
         * @param center
         */
        void viewMatrix(QVector4D position, QVector4D upVector, QVector4D center);

        /**
         * Creates a projection matrix with an aspect ratio and a field of view in the case of a perspective camera.
         * @brief projectionMatrix
         * @param aspectRatio
         * @param fieldOfView
         */
        void projectionMatrix(float aspectRatio = 16.0/9.0, float fieldOfView = 30.0);

        /**
         * Rotates the camera along the X axis.
         * @brief rotateX
         * @param xRotation
         */
        void rotateX(float xRotation);

        /**
         * Rotates the camera along the Y axis.
         * @brief rotateY
         * @param yRotation
         */
        void rotateY(float yRotation);

        /**
         * Translates the camera along the axis cameraPosition-cameraCenter
         * @brief translateAlongOriginAxis
         * @param zTranslation
         */
        void translateAlongOriginAxis(float zTranslation);

        /**
         * Reset the camera position, upVector and center.
         * @brief resetCamera
         */
        void resetCamera();

        /**
         * Returns the camera view matrix.
         * @brief getViewMatrix
         * @return
         */
        QMatrix4x4 getViewMatrix();

        /**
         * Returns the camera projection matrix.
         * @brief getProjectionMatrix
         * @return
         */
        QMatrix4x4 getProjectionMatrix();

        /**
         * Returns the camera position.
         * @brief getPosition
         * @return
         */
        QVector4D getPosition();

        /**
         * Returns the camera up vector
         * @brief getUpVector
         * @return
         */
        QVector4D getUpVector();

        /**
         * Returns the camera center.
         * @brief getCenter
         * @return
         */
        QVector4D getCenter();

    private:

        QVector4D m_position; /*!< Camera position*/
        QVector4D m_upVector; /*!< Camera up vector*/
        QVector4D m_center; /*!< Camera center point : point at which the camera is looking*/

        bool m_perspectiveCamera; /*!< The camera is a perspective camera if this boolean is true. Otherwise it is an orthographic camera */
        float m_aspectRatio; /*!< Camera's aspect ratio*/
        float m_fieldOfView; /*!< Camera's field of view*/

        QMatrix4x4 m_viewMatrix;  /*!< Camera's view matrix*/
        QMatrix4x4 m_projectionMatrix; /*!< Camera's projection matrix*/
};

#endif // CAMERA_H
