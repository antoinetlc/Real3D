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
 * \file gldisplay.h
 * \brief Implementation of an OpenGL renderer widget.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an OpenGL renderer widget. Loads and renders a 3D scene inside the widget using OpenGL.
 */


#ifndef GLDISPLAY_H
#define GLDISPLAY_H


#define SHADER_NAME "phong"
#define FRAMEBUFFER_WIDTH 1920
#define FRAMEBUFFER_HEIGHT 1080

#include "opengl/material.h"
#include "opengl/object.h"
#include "opengl/light.h"
#include "opengl/scene.h"
#include "opengl/framebuffer.h"
#include "opengl/camera.h"
#include "opengl/openglheaders.h"

#include <QApplication>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <QMatrix>
#include <QVector>
#include <QVector2D>
#include <QSize>
#include <QTimer>
#include <QTime>
#include <QGLFormat>
#include <QKeyEvent>
#include <QFileDialog>
#include <QWheelEvent>
#include <QKeyEvent>

#include <string>
#include <sstream>
#include <iostream>

class GLDisplay : public QGLWidget
{
    Q_OBJECT

    public:
        /**
         * Constructor to create a GLDisplay widget given its parent.
         * @brief GLDisplay
         * @param parent
         */
        GLDisplay(QWidget *parent);

        /**
         * Constructor to create a GLDisplay widget given its parent and an OpenGL Format
         * @brief GLDisplay
         * @param glFormat
         * @param parent
         */
        GLDisplay(const QGLFormat& glFormat, QWidget *parent);

        /**
          * Destructor.
          */
        ~GLDisplay();

        /**
         * Renders the scene.
         * @brief renderScene
         */
        void renderScene();

        /**
         * Renders the color buffer of the framebuffer to a quad.
         * @brief renderToTexture
         */
        void renderToTexture();

        /**
         * Draw the background of the scene
         * @brief renderBackground
         */
        void renderBackground();

        /**
         * Draws the number of frame per second on the OpenGL window.
         * @brief drawFPS
         */
        void drawFPS();

        /**
         * Binds the object data to a shader.
         * @brief sendObjectDataToShaders
         * @param object
         */
        void sendObjectDataToShaders(Object &object);

        /**
         * Creates an animation of the scene.
         * @brief animation
         */
        void animation();

        /**
         * Loads a given vertex and fragment shader given their path.
         * @brief loadShaders
         * @param vertexShaderPath
         * @param fragmentShaderPath
         */
        void loadShaders(QString vertexShaderPath, QString fragmentShaderPath);

        /**
         * Loads an environment map given its path (without the file extension assumed to be PFM).
         * @brief loadEnvironmentMap
         * @param environmentMapPathNoExtension
         * @return
         */
        bool loadEnvironmentMap(QString environmentMapPathNoExtension);

        /**
         * Enables and disables the environment mapping.
         * @brief enableEnvironmentMapping
         * @param enableEM
         */
        void enableEnvironmentMapping(bool enableEM);

    protected:
        /**
         * Initialize the OpenGL context and GLEW.
         * @brief initializeGL
         */
        void initializeGL();

        /**
         * Update the viewport when the OpenGL window is resized.
         * @brief resizeGL
         * @param width
         * @param height
         */
        void resizeGL(int width, int height);

        /**
         * Main rendering loop.
         * @brief paintGL
         */
        void paintGL();

        /**
         * Manages the events of the mouse wheel.
         * @brief wheelEvent
         * @param event
         */
        void wheelEvent(QWheelEvent* event);

        /**
         * Manages the events when the mouse buttons are pressed.
         * @brief mousePressEvent
         * @param event
         */
        void mousePressEvent(QMouseEvent *event);

        /**
         * Manages the events when the mouse is moved.
         * @brief mouseMoveEvent
         * @param event
         */
        void mouseMoveEvent(QMouseEvent *event);

        /**
         * Manages the keyboard events.
         * @brief keyPressEvent
         * @param event
         */
        void keyPressEvent(QKeyEvent *event);

    signals:
        /**
         * Sends the path of the diffuse reflectance map.
         * @brief updateDiffuseMapPath
         */
        void updateDiffuseMapPath(QString);

        /**
         * Sends the path of the specular reflectance map.
         * @brief updateSpecularMapPath
         */
        void updateSpecularMapPath(QString);

        /**
         * Sends the path of the normal map.
         * @brief updateNormalMapPath
         */
        void updateNormalMapPath(QString);

        /**
         * Sends the path of the roughness reflectance map.
         * @brief updateRoughnessMapPath
         */
        void updateRoughnessMapPath(QString);

        /**
         * Sends the path of the vertex shader.
         * @brief updateVertexShaderPath
         */
        void updateVertexShaderPath(QString);

        /**
         * Sends the path of the fragment shader.
         * @brief updateFragmentShaderPath
         */
        void updateFragmentShaderPath(QString);

        /**
         * Sends an update to the Log window.
         * @brief updateLog
         */
        void updateLog(QString);

    public slots:
        /**
         * Updates the OpenGL display.
         * @brief updateDisplay
         */
        void updateDisplay();

        /**
         * Takes a screenshot and saves it to the folder /screenshot/.
         * @brief takeScreenshot
         */
        void takeScreenshot();

        /**
         * Changes the exposure of the rendering.
         * @brief changeExposure
         * @param exposureSlider
         */
        void changeExposure(int exposureSlider);

        /**
         * Starts and stops the animation.
         * @brief startStopAnimation
         */
        void startStopAnimation();

        /**
         * Opens a QFileDialog to choose the image corresponding to the diffuse reflectance map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief chooseDiffuseMap
         */
        void chooseDiffuseMap();

        /**
         * Loads the image given in the file path as the diffuse map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief loadDiffuseMap
         * @param filePath
         */
        void loadDiffuseMap(QString filePath);

        /**
         * Opens a QFileDialog to choose the image corresponding to the specular reflectance map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief chooseSpecularMap
         */
        void chooseSpecularMap();

        /**
         * Loads the image given in the file path as the specular map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief loadSpecularMap
         * @param filePath
         */
        void loadSpecularMap(QString filePath);

        /**
         * Opens a QFileDialog to choose the image corresponding to the normal map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief chooseNormalMap
         */
        void chooseNormalMap();

        /**
         * Loads the image given in the file path as the normal map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief loadNormalMap
         * @param filePath
         */
        void loadNormalMap(QString filePath);

        /**
         * Opens a QFileDialog to choose the image corresponding to the roughness map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief chooseRoughnessMap
         */
        void chooseRoughnessMap();

        /**
         * Loads the image given in the file path as the roughness map.
         * The image can be a 8 bit image or an HDR 32 bits image.
         * @brief loadRoughnessMap
         * @param filePath
         */
        void loadRoughnessMap(QString filePath);

        /**
         * Opens a QFileDialog to choose the vertex shader.
         * @brief chooseVertexShader
         */
        void chooseVertexShader();

        /**
         * Opens a QFileDialog to choose the fragment shader.
         * @brief chooseFragmentShader
         */
        void chooseFragmentShader();


    private:

        //Framebuffer for highres rendering
        FrameBuffer m_framebuffer;  /*!< Framebuffer. */

        //Camera
        Camera m_cameraScene;  /*!< Scene camera. */
        Camera m_cameraQuad;  /*!< Render to texture camera */

        //Mouse events
        QVector2D m_mousePos;  /*!< Mouse position */

        //Shaders
        QString m_shaderName;  /*!< Name of the rendering shader. */
        QGLShaderProgram m_backgroundProgram;  /*!< Shader program to render the background. */
        QGLShaderProgram m_shaderProgram; /*!< Shader program to render the scene. */
        QGLShaderProgram m_shaderProgramDisplay; /*!< Shader program for render to texture. */

        //Frame per second
        QTime m_timeFPS; /*!< Time for the FPS count. */
        int m_lastFPSUpdate; /*!< Last time the FPS were updated. */
        int m_frameCounter; /*!< Counts the number of frames. */
        int m_FPS; /*!< Number of frames per second. */

        //Scene
        Scene m_scene; /*!< Scene. */
        bool m_environmentMapping; /*!< Boolean that is true if the environment mapping is on. */
        float m_exposure; /*!< Exposure of the rendering. */

        //Animation
        bool m_animationStarted; /*!< Boolean that is true if the animation is on.  */
        QTimer m_updateDisplayTimer; /*!< Timer that starts during the animation. Required to update the display during the animation.  */
        QTime m_animationTime; /*!< Time to make the animation */
};

#endif // GLDISPLAY_H
