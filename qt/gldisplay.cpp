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
 * \file gldisplay.cpp
 * \brief Implementation of an OpenGL renderer widget.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an OpenGL renderer widget. Loads and renders a 3D scene inside the widget using OpenGL.
 */

#include "qt/gldisplay.h"

using namespace std;
using namespace cv;

/**
 * Constructor to create a GLDisplay widget given its parent.
 * @brief GLDisplay
 * @param parent
 */
GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(QGLFormat(), parent),
    m_cameraScene(Camera()), m_cameraQuad(Camera()),
    m_mousePos(0,0),
    m_shaderName(SHADER_NAME), m_backgroundProgram(), m_shaderProgram(), m_shaderProgramDisplay(),
    m_timeFPS(QTime()), m_lastFPSUpdate(0), m_frameCounter(0), m_FPS(0),
    m_environmentMapping(false), m_exposure(0.0),
    m_animationStarted(false), m_updateDisplayTimer(), m_animationTime(QTime())
{
    m_scene = Scene();

    //Starts the FPS timer
    m_timeFPS.start();

    //Timer to update the display at each frame
    //Otherwise the animation is not shown on the screen
    QObject::connect(&m_updateDisplayTimer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
}

/**
 * Constructor to create a GLDisplay widget given its parent and an OpenGL Format
 * @brief GLDisplay
 * @param glFormat
 * @param parent
 */
GLDisplay::GLDisplay(const QGLFormat& glFormat, QWidget *parent) : QGLWidget(glFormat, parent),
    m_cameraScene(Camera()), m_cameraQuad(Camera()),
    m_mousePos(0,0),
    m_shaderName(SHADER_NAME), m_backgroundProgram(), m_shaderProgram(), m_shaderProgramDisplay(),
    m_timeFPS(QTime()), m_lastFPSUpdate(0), m_frameCounter(0), m_FPS(0),
    m_environmentMapping(false), m_exposure(0.0),
    m_animationStarted(false), m_updateDisplayTimer(), m_animationTime(QTime())
{
    m_scene = Scene();

    //Starts the FPS timer
    m_timeFPS.start();

    //Timer to update the display at each frame
    //Otherwise the animation is not shown on the screen
    QObject::connect(&m_updateDisplayTimer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
}

/**
  * Destructor.
  */
GLDisplay::~GLDisplay()
{

}

/**
 * Initialize the OpenGL context and GLEW.
 * @brief initializeGL
 */
void GLDisplay::initializeGL()
{
    QString openGLInfo;
    openGLInfo = QString("Widget OpenGl: %1.%2\n").arg(format().majorVersion()).arg(format().minorVersion());

    openGLInfo += QString("Context valid : %1\n").arg(context()->isValid() ? "true" : "false");
    openGLInfo += QString("Really used OpenGl : %1.%2\n").arg(context()->format().majorVersion()).arg(context()->format().minorVersion());
    openGLInfo += QString("OpenGl information : \n \tVENDOR :       %1\n").arg((const char*)glGetString(GL_VENDOR));
    openGLInfo += QString("\tRENDERER :    %1\n").arg((const char*)glGetString(GL_RENDERER));
    openGLInfo += QString("\tVERSION :      %1\n").arg((const char*)glGetString(GL_VERSION));
    openGLInfo += QString("\tGLSL VERSION : %1\n\n").arg((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    updateLog(openGLInfo);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    qglClearColor(QColor(Qt::black));

    QString path = qApp->applicationDirPath();

#ifdef _WIN32
    path += "\\shaders\\";
#endif

#ifdef __gnu_linux__
    path += "/shaders/";
#endif

#if defined(__APPLE__) && defined(__MACH__)
    path += "/../../../shaders/";
#endif

    m_backgroundProgram.addShaderFromSourceFile(QGLShader::Vertex, path + "background.vsh");
    m_backgroundProgram.addShaderFromSourceFile(QGLShader::Fragment, path + "background.fsh");

    m_shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, path + m_shaderName + ".vsh");
    m_shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, path + m_shaderName + ".fsh");

    m_shaderProgramDisplay.addShaderFromSourceFile(QGLShader::Vertex, path + "texture.vsh");
    m_shaderProgramDisplay.addShaderFromSourceFile(QGLShader::Fragment, path + "texture.fsh");

    if(!m_backgroundProgram.link())
    {
        QString error = m_backgroundProgram.log();
        emit updateLog(error);
        qDebug() << error << endl;
        qApp->exit(EXIT_FAILURE);
    }

    if(!m_shaderProgramDisplay.link())
    {
        QString error = m_shaderProgramDisplay.log();
        emit updateLog(error);
        qDebug() << error << endl;
        qApp->exit(EXIT_FAILURE);
    }

    if(!m_shaderProgram.link())
    {
        QString error = m_shaderProgram.log();
        emit updateLog(error);
        qDebug() << error << endl;
        qApp->exit(EXIT_FAILURE);
    }

    //Initialisation of GLEW
    GLenum glewError = glewInit();

    if( glewError != GLEW_OK )
        cout << "Error in GLEW initialisation" << endl;

    //Create a framebuffer and load it (empty but creates its ID)
    m_framebuffer = FrameBuffer(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    m_framebuffer.load_8UC3();

    /*---- Camera initialisation to render----*/
    //The camera that displays the final square is the moving camera
    //Compute the transformation of the camera
    QVector4D positionQuad = QVector4D(0.0,0.0, 1.0, 1.0);
    QVector4D upVectorQuad = QVector4D(0.0,1.0,0.0, 1.0);
    QVector4D centerQuad = QVector4D(0.0, 0.0,0.0, 1.0);

    m_cameraQuad = Camera(positionQuad, upVectorQuad, centerQuad, true, (float)this->width()/(float)this->height(),30.0);

    QVector4D positionScene = QVector4D(0.0,0.0, 1.0, 1.0);
    QVector4D upVectorScene = QVector4D(0.0,1.0,0.0, 1.0);
    QVector4D centerScene = QVector4D(0.0, 0.0,0.0, 1.0);

    m_cameraScene = Camera(positionScene, upVectorScene, centerScene, true, (float)m_framebuffer.getWidth()/(float)m_framebuffer.getHeight(),45.0);
}

/**
 * Update the viewport when the OpenGL window is resized.
 * @brief resizeGL
 * @param width
 * @param height
 */
void GLDisplay::resizeGL(int width, int height)
{
    //Avoid division by 0
    if(height == 0)
        height = 1;

    //Recompute projection matrix
    m_cameraQuad.projectionMatrix((float)width/(float)height, 30.0); //Reset the projection matrix

    //Map the projection to the GLWidget window
    glViewport(0, 0, width, height);

}

/**
 * Main rendering loop.
 * @brief paintGL
 */
void GLDisplay::paintGL()
{
    //Render to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.getFramebufferID());

    //Clear screen
    //Clear the color and the z buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, m_framebuffer.getWidth(), m_framebuffer.getHeight());

    //Render the background
    if(m_environmentMapping)
        this->renderBackground();

    //If the animation has started
    if(m_animationStarted)
    {
        this->animation();
    }

    //Render the scene
    this->renderScene();

    //Render the framebuffer on a quad
    this->renderToTexture();

    //Draw the number of FPS
    this->drawFPS();

    if(m_animationStarted)
    {
        //Timer to update the display at each frame
        //Otherwise the animation is not shown on the screen
        m_updateDisplayTimer.start(20);
    }

}

/**
 * Renders the scene.
 * @brief renderScene
 */
void GLDisplay::renderScene()
{


    /*---load the scene and draw it ---*/
    m_shaderProgram.bind();

    /*---Camera and matrices---*/

    //Matrices for the Frame buffer object
    //The camera is fixed
    QMatrix4x4 projectionScene, viewMatrixScene;

    viewMatrixScene =  m_cameraScene.getViewMatrix();
    projectionScene =  m_cameraScene.getProjectionMatrix();

    //The textures must be loaded at each frame
    //Send the time of animation in milliseconds
    m_shaderProgram.setUniformValue("timeMs", m_animationTime.elapsed());

    //Load the scene
    QVector<Object> objectList = m_scene.getObjects();
    QVector<Light> pointLights = m_scene.getPointLightSources();

    //Repeat that for each object
    QMatrix4x4 modelMatrixObject = QMatrix4x4();
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoordinates;
    QVector<GLuint> indicesArray;
    QVector4D lightPosition = pointLights[0].getLightPosition();
    QMatrix4x4 lightModelMatrix = pointLights[0].getModelMatrix();


    for(int k = 0 ; k<objectList.size() ; k++)
    {
        //Get the data
        modelMatrixObject = objectList[k].getModelMatrix();
        vertices =  objectList[k].getMesh().getVertices();
        normals = objectList[k].getMesh().getVertexNormals();
        textureCoordinates = objectList[k].getTextureCoordinates();
        indicesArray = objectList[k].getMesh().getIndicesArray();

        //Send uniform data to shaders
        //Do the maximum of matrix multiplication on the CPU for better efficiency
        m_shaderProgram.setUniformValue("mvMatrix", viewMatrixScene*modelMatrixObject);
        m_shaderProgram.setUniformValue("vMatrix", viewMatrixScene); //Inverse of the view matrix for environment mapping
        m_shaderProgram.setUniformValue("pMatrix", projectionScene);
        m_shaderProgram.setUniformValue("normalMatrix", (viewMatrixScene*modelMatrixObject).normalMatrix()); //Normals are in the camera space
        m_shaderProgram.setUniformValue("lightPosition_camSpace", viewMatrixScene*lightModelMatrix*lightPosition); //Light position in the camera space

        m_shaderProgram.setUniformValue("environmentMapping", m_environmentMapping); //Is environmentMapping activated

        //sendData
        this->sendObjectDataToShaders(objectList[k]);

        m_shaderProgram.setUniformValue("exposure", m_exposure);

        /*---------------- Vertices, texture coordinates and normals ---------------------*/

        m_shaderProgram.setAttributeArray("vertex_worldSpace", vertices.constData());
        m_shaderProgram.enableAttributeArray("vertex_worldSpace");

        m_shaderProgram.setAttributeArray("textureCoordinate_input", textureCoordinates.constData());
        m_shaderProgram.enableAttributeArray("textureCoordinate_input");

        m_shaderProgram.setAttributeArray("normal_worldSpace", normals.constData());
        m_shaderProgram.enableAttributeArray("normal_worldSpace");

        //Draw the current object
        glDrawElements(GL_TRIANGLES, indicesArray.size(), GL_UNSIGNED_INT, indicesArray.constData());

        //Unbind the textures
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    m_shaderProgram.disableAttributeArray("vertex_worldSpace");
    m_shaderProgram.disableAttributeArray("normal_worldSpace");
    m_shaderProgram.disableAttributeArray("textureCoordinates_input");
    glFlush();
    m_shaderProgram.release();

}

/**
 * Renders the color buffer of the framebuffer to a quad.
 * @brief renderToTexture
 */
void GLDisplay::renderToTexture()
{

    /*------ Display the framebuffer on the screen -----*/
    //Release memory associated with the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glViewport(0, 0, this->width(), this->height());

     //Switch to the display shader
    //Always bind before sending the textures to the shader
    if(!m_shaderProgramDisplay.bind())
    {
        cout << "m_shaderProgramDisplay not bound" << endl;
    }

    //Send the texture to shaders
    GLint textureRenderedId = glGetUniformLocation(m_shaderProgramDisplay.programId(), "textureRendered");

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(textureRenderedId, 0); // 0 is the texture number

    //Bind the texture so that it can be used by the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_framebuffer.getColorBufferID(0));

    //Render a quad
    Object square = Object(string("square"));
    square.setAspectRatio((float)m_framebuffer.getWidth()/(float)m_framebuffer.getHeight());
    square.scale(2.0);

    QMatrix4x4 viewMatrixQuad = m_cameraQuad.getViewMatrix();
    QMatrix4x4 projectionMatrixQuad = m_cameraQuad.getProjectionMatrix();

    m_shaderProgramDisplay.setUniformValue("mvMatrix", viewMatrixQuad*square.getModelMatrix());
    m_shaderProgramDisplay.setUniformValue("pMatrix", projectionMatrixQuad);

    m_shaderProgramDisplay.setAttributeArray("vertex_worldSpace", square.getMesh().getVertices().constData());
    m_shaderProgramDisplay.enableAttributeArray("vertex_worldSpace");

    m_shaderProgramDisplay.setAttributeArray("textureCoordinate_input", square.getTextureCoordinates().constData());
    m_shaderProgramDisplay.enableAttributeArray("textureCoordinate_input");

    m_shaderProgramDisplay.setAttributeArray("normal_worldSpace", square.getMesh().getVertexNormals().constData());
    m_shaderProgramDisplay.enableAttributeArray("normal_worldSpace");

    //Draw the current object
    glDrawElements(GL_TRIANGLES, square.getMesh().getIndicesArray().size(), GL_UNSIGNED_INT, square.getMesh().getIndicesArray().constData());

    m_shaderProgramDisplay.disableAttributeArray("vertex_worldSpace");
    m_shaderProgramDisplay.disableAttributeArray("normal_worldSpace");
    m_shaderProgramDisplay.disableAttributeArray("textureCoordinates_input");
    glFlush();

    m_shaderProgramDisplay.release();
}

/**
 * Draw the background of the scene
 * @brief renderBackground
 */
void GLDisplay::renderBackground()
{
    //Disable depth test to draw at infinity
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    /*------ Display the framebuffer on the screen -----*/
    //Release memory associated with the framebuffer

    //Switch to the display shader
    if(!m_backgroundProgram.bind())
    {
        cerr << "m_shaderProgramDisplay not bound" << endl;
    }

    //Send the time to the background shader for environment map rotation
    m_backgroundProgram.setUniformValue("timeMs", m_animationTime.elapsed());

    GLint environmentMapBackgroundId = glGetUniformLocation(m_backgroundProgram.programId(), "backgroundEnvMap");

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(environmentMapBackgroundId, 0); // 0 is the texture number

    //Bind the texture so that it can be used by the shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_scene.getEnvironmentMapId());

    //Display it on a rectangle  with correct aspect ratio
    Object square = Object(string("square"));
    square.setAspectRatio((float)m_framebuffer.getWidth()/(float)m_framebuffer.getHeight());
    square.scale(2.0);//The square covers the entire screen

    m_backgroundProgram.setUniformValue("mvMatrix", m_cameraQuad.getViewMatrix()*square.getModelMatrix());
    m_backgroundProgram.setUniformValue("vMatrix", m_cameraQuad.getViewMatrix()); //Inverse of the view matrix for environment mapping
    m_backgroundProgram.setUniformValue("pMatrix", m_cameraQuad.getProjectionMatrix());

    m_backgroundProgram.setAttributeArray("vertex_worldSpace", square.getMesh().getVertices().constData());
    m_backgroundProgram.enableAttributeArray("vertex_worldSpace");

    m_backgroundProgram.setAttributeArray("textureCoordinate_input", square.getTextureCoordinates().constData());
    m_backgroundProgram.enableAttributeArray("textureCoordinate_input");

    m_backgroundProgram.setAttributeArray("normal_worldSpace", square.getMesh().getVertexNormals().constData());
    m_backgroundProgram.enableAttributeArray("normal_worldSpace");

    //Draw the current object
    glDrawElements(GL_TRIANGLES, square.getMesh().getIndicesArray().size(), GL_UNSIGNED_INT, square.getMesh().getIndicesArray().constData());

    m_backgroundProgram.disableAttributeArray("vertex_worldSpace");
    m_backgroundProgram.disableAttributeArray("normal_worldSpace");
    m_backgroundProgram.disableAttributeArray("textureCoordinates_input");
    glFlush();

    m_backgroundProgram.release();

    //Renable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

/**
 * Draws the number of frame per second on the OpenGL window.
 * @brief drawFPS
 */
void GLDisplay::drawFPS()
{
    //Count the current frame
    ++m_frameCounter;
    int currentTime = m_timeFPS.elapsed();

    //Every 1 second update the number of FPS
    if(currentTime-m_lastFPSUpdate>=1000)
    {
        m_FPS = m_frameCounter;
        m_frameCounter = 0;
        m_lastFPSUpdate = currentTime;
    }

    QString textFPS = QString( "%1 FPS" ).arg(m_FPS);

    //Set the color to white for to draw the FPS
    glColor3f(1.0, 1.0, 1.0);
    renderText(width() - textFPS.size()- 40, 20, textFPS);
}

/**
 * Binds the object data to a shader.
 * @brief sendObjectDataToShaders
 * @param object
 */
void GLDisplay::sendObjectDataToShaders(Object &object)
{
    Material material = Material();
    material = object.getMaterial();

    m_shaderProgram.setUniformValue("ambientColor", material.getAmbientColor());
    m_shaderProgram.setUniformValue("diffuseColor", material.getDiffuseColor());
    m_shaderProgram.setUniformValue("specularColor", material.getSpecularColor());
    m_shaderProgram.setUniformValue("ambientCoefficient", material.getAmbientCoefficient());
    m_shaderProgram.setUniformValue("diffuseCoefficient", material.getDiffuseCoefficient());
    m_shaderProgram.setUniformValue("specularCoefficient", material.getSpecularCoefficient());
    m_shaderProgram.setUniformValue("shininess", material.getShininess());

    GLint diffuseMapId = glGetUniformLocation(m_shaderProgram.programId(), "diffuse_texture");
    GLint specularMapId = glGetUniformLocation(m_shaderProgram.programId(), "specular_texture");
    GLint normalMapId = glGetUniformLocation(m_shaderProgram.programId(), "normal_map");
    GLint roughnessMapId = glGetUniformLocation(m_shaderProgram.programId(), "roughness_map");
    GLint envMapId = glGetUniformLocation(m_shaderProgram.programId(), "environmentMap");
    GLint envMapRoughId = glGetUniformLocation(m_shaderProgram.programId(), "environmentMapRough");
    GLint envMapDiffuseId = glGetUniformLocation(m_shaderProgram.programId(), "environmentMapDiffuse");

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(diffuseMapId, 0); // 0 is the texture number

    glActiveTexture(GL_TEXTURE0+1);
    glUniform1i(specularMapId, 1); // 1 is the texture number

    glActiveTexture(GL_TEXTURE0+2);
    glUniform1i(normalMapId, 2); // 2 is the texture number

    glActiveTexture(GL_TEXTURE0+3);
    glUniform1i(roughnessMapId, 3); // 3 is the texture number

    glActiveTexture(GL_TEXTURE0+4);
    glUniform1i(envMapId, 4); // 4 is the texture number

    glActiveTexture(GL_TEXTURE0+5);
    glUniform1i(envMapRoughId, 5); // 5 is the texture number

    glActiveTexture(GL_TEXTURE0+6);
    glUniform1i(envMapDiffuseId, 6); // 6 is the texture number

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object.getDiffuseTexture().getTextureId());

    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, object.getSpecularTexture().getTextureId());

    glActiveTexture(GL_TEXTURE0+2);
    glBindTexture(GL_TEXTURE_2D, object.getNormalMap().getTextureId());

    glActiveTexture(GL_TEXTURE0+3);
    glBindTexture(GL_TEXTURE_2D, object.getRoughnessMap().getTextureId());

    glActiveTexture(GL_TEXTURE0+4);
    glBindTexture(GL_TEXTURE_2D, m_scene.getEnvironmentMapId());

    glActiveTexture(GL_TEXTURE0+5);
    glBindTexture(GL_TEXTURE_2D, m_scene.getEnvironmentMapRoughId());

    glActiveTexture(GL_TEXTURE0+6);
    glBindTexture(GL_TEXTURE_2D, m_scene.getEnvironmentMapDiffuseId());

}

/**
 * Creates an animation of the scene.
 * @brief animation
 */
void GLDisplay::animation()
{
    float maxDistanceZ = 25.0;
    float scaleFactor = 300.0;
    float radius = 30.0;
    float theta = 0.0;

    /*----Animation with with rotating light-----*/

    if(m_animationTime.elapsed()>=0 && m_animationTime.elapsed()<1000) //2000 miliseconds
    {
        theta = 0.0;
        m_scene.setLightSourcePosition(0, 0.0, radius*sin(theta), radius*cos(theta));
    }


    if(m_animationTime.elapsed()>=1000 && m_animationTime.elapsed()<3000) //2000 miliseconds
    {
        theta = (m_animationTime.elapsed()-1000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, 0.0, radius*sin(theta), radius*cos(theta));
    }


    if(m_animationTime.elapsed()>=3000 && m_animationTime.elapsed()<5000) //2000 miliseconds
    {
        theta = M_PI/12.0-(m_animationTime.elapsed()-3000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, 0.0, radius*sin(theta), radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=5000 && m_animationTime.elapsed()<7000.0) //2000 miliseconds
    {
        theta = -(m_animationTime.elapsed()-5000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, 0.0, radius*sin(theta), radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=7000 && m_animationTime.elapsed()<9000) //2000 miliseconds
    {
        theta = -M_PI/12.0+(m_animationTime.elapsed()-7000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, 0.0, radius*sin(theta), radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=9000 && m_animationTime.elapsed()<11000) //2000 miliseconds
    {
        theta = (m_animationTime.elapsed()-9000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, radius*sin(theta), 0.0, radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=11000 && m_animationTime.elapsed()<13000) //2000 miliseconds
    {
        theta = M_PI/12.0-(m_animationTime.elapsed()-11000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, radius*sin(theta), 0.0, radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=13000 && m_animationTime.elapsed()<15000.0) //2000 miliseconds
    {
        theta = -(m_animationTime.elapsed()-13000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, radius*sin(theta), 0.0, radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=15000 && m_animationTime.elapsed()<17000) //2000 miliseconds
    {
        theta = -M_PI/12.0+(m_animationTime.elapsed()-15000.0)/2000.0*(M_PI/12.0);
        m_scene.setLightSourcePosition(0, radius*sin(theta), 0.0, radius*cos(theta));
    }

    if(m_animationTime.elapsed()>=17000 && m_animationTime.elapsed()<19000) // miliseconds
         m_scene.translateLightSourceZ(0, -5.0*maxDistanceZ/scaleFactor);

    if(m_animationTime.elapsed()>=19000 && m_animationTime.elapsed()<21000)
    {
         m_scene.translateLightSourceZ(0, 5.0*maxDistanceZ/scaleFactor);
    }

}

/*--------------------------Mouse events-----------------------------------*/

/**
 * Manages the events of the mouse wheel.
 * @brief wheelEvent
 * @param event
 */
void GLDisplay::wheelEvent(QWheelEvent* event)
{
    int variation = event->delta();

    //Control the Camera if CTRL NOT pressed
    if(event->orientation() == Qt::Vertical && !(QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        if(variation<0) //increase the distance
        {
            m_cameraScene.translateAlongOriginAxis((float)1.1);
        }
        else//Decrease the distance
        {
            m_cameraScene.translateAlongOriginAxis((float)0.9);
        }
    }

     //Control the light if CTRL is pressed
    if(event->orientation() == Qt::Vertical && (QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
        if(variation<0) //Decrease the distance
        {
            m_scene.translateLightSourceZ(0, variation/1000.0);

            cout << "Decrease z " << variation/1000.0 << endl;
        }
        else//Increase the distance
        {
            m_scene.translateLightSourceZ(0, variation/1000.0);
            cout << "Increase z " << variation/1000.0 << endl;
        }
    }

    updateGL();
    event->accept();
}

/**
 * Manages the events when the mouse buttons are pressed.
 * @brief mousePressEvent
 * @param event
 */
void GLDisplay::mousePressEvent(QMouseEvent *event)
{
    //When the mouse is pressed, save its position
    m_mousePos = QVector2D(event->pos().x(), event->pos().y());
    event->accept();
}

/**
 * Manages the events when the mouse is moved.
 * @brief mouseMoveEvent
 * @param event
 */
void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    //Mouse move camera
    //CTRL+mouse move light

    //When the mouse is moved after it has been pressed,
    //calculate the number of pixels it has moved and rotate the scene
    if(event->buttons() == Qt::LeftButton &&  !(QApplication::keyboardModifiers() == Qt::ControlModifier))
    {
       m_cameraScene.rotateY((m_mousePos.x()-event->pos().x()));
       m_cameraScene.rotateX(-(m_mousePos.y()-event->pos().y()));//- to follow the mouse
       updateGL();//Update openGL
    }
    else if(event->buttons() == Qt::LeftButton && QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        m_scene.translateLightSourceX(0, (m_mousePos.x()-event->pos().x())/100.0);
        m_scene.translateLightSourceY(0, (m_mousePos.y()-event->pos().y())/100.0);
        updateGL();//Update openGL
    }

    //Update the position of the mouse
    m_mousePos = QVector2D(event->pos().x(), event->pos().y());
    event->accept();
}

/**
 * Manages the keyboard events.
 * @brief keyPressEvent
 * @param event
 */
void GLDisplay::keyPressEvent(QKeyEvent *event)
{

    //Quick translation of light source
    if(event->key() == Qt::Key_Z)
    {
        m_scene.translateLightSourceZ(0, -5.0);
    }

    if(event->key() == Qt::Key_X)
    {
        m_scene.translateLightSourceZ(0, 5.0);
    }

    if(event->key() == Qt::Key_K)
    {
        this->takeScreenshot();
    }

    //Animation
    if(event->key() == Qt::Key_S)
    {
        this->startStopAnimation();
    }

    //Reset scene and animation
    if(event->key() == Qt::Key_D)
    {
        m_animationStarted = false;
        m_animationTime = QTime();

        //Reset the camera
        m_cameraScene.resetCamera();

        //Reset the scene
        m_scene.resetScene();
        qDebug() << "Reset scene" << endl;
    }

    updateGL();//Update openGL
    event->accept();
}

/*--SLOTS---*/

/**
 * Updates the OpenGL display.
 * @brief updateDisplay
 */
void GLDisplay::updateDisplay()
{
    updateGL();
}

/**
 * Takes a screenshot and saves it to the folder /screenshot/.
 * @brief takeScreenshot
 */
void GLDisplay::takeScreenshot()
{
    int width = m_framebuffer.getWidth();
    int height = m_framebuffer.getHeight();
    float *data = new float[width*height*3];
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.getFramebufferID());

    //Read all the pixels
    glReadPixels(0,0, width,height, GL_RGB, GL_FLOAT, data);

    //Convert to an openCV matrix
    Mat picture = Mat(height, width, CV_32FC3);
    for(int i = 0 ; i<height ; i++)
    {
        for(int j = 0 ; j<width ; j++)
        {
            picture.at<Vec3f>(i,j).val[2] = floor(255.0*data[3*((height-1-i)*width+j)]);
            picture.at<Vec3f>(i,j).val[1] = floor(255.0*data[3*((height-1-i)*width+j)+1]);
            picture.at<Vec3f>(i,j).val[0] = floor(255.0*data[3*((height-1-i)*width+j)+2]);
        }
    }

    //Save
    imwrite(qApp->applicationDirPath().toStdString()+"/screenshot/screenshot.jpg", picture);

    //Update the log
    QString log;
    log = QString("Screenshot saved : %1").arg(qApp->applicationDirPath()+"/screenshot/screenshot.jpg");
    updateLog(log);
}

/**
 * Changes the exposure of the rendering.
 * @brief changeExposure
 * @param exposureSlider
 */
void GLDisplay::changeExposure(int exposureSlider)
{
    //Slider varies between -100 and 100;
    m_exposure = (float) exposureSlider/10.0;

    updateGL();
}

/**
 * Starts and stops the animation.
 * @brief startStopAnimation
 */
void GLDisplay::startStopAnimation()
{
    if(!m_animationStarted)
    {
        m_animationStarted = true;
        m_animationTime.start();
        m_updateDisplayTimer.start(20);
    }
    else
    {
       // m_animationTime = QTime();
        m_animationStarted = false;

        m_updateDisplayTimer.stop();
    }
    updateGL();
}


/**
 * Opens a QFileDialog to choose the image corresponding to the diffuse reflectance map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief chooseDiffuseMap
 */
void GLDisplay::chooseDiffuseMap()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose diffuse map"),
                            QDir::currentPath(),
                            QString(tr("All Images files (*.jpg *.jpeg *.png *.bmp *.tif *.pfm);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TIFF (*.tif);;PFM (*.pfm)")));

    if (!chosenFile.isEmpty()){
        emit updateDiffuseMapPath(chosenFile);
    }
}

/**
 * Loads the image given in the file path as the diffuse map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief loadDiffuseMap
 * @param filePath
 */
void GLDisplay::loadDiffuseMap(QString filePath)
{

  if(filePath.size()>0)
  {
       bool loaded = m_scene.loadDiffuseMap(filePath.toStdString(), 0);

       if(!loaded)
       {
           QString error = QString("Could not load texture : \n%1\n\n").arg(filePath);
           emit updateLog(error);
       }
       else
       {
           QString text = QString("Texture correctly loaded : \n%1\n\n").arg(filePath);
           emit updateLog(text);
       }

       updateGL();
  }
}

/**
 * Opens a QFileDialog to choose the image corresponding to the specular reflectance map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief chooseSpecularMap
 */
void GLDisplay::chooseSpecularMap()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose specular map"),
                            QDir::currentPath(),
                            QString(tr("All Images files (*.jpg *.jpeg *.png *.bmp *.tif *.pfm);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TIFF (*.tif);;PFM (*.pfm)")));

    if (!chosenFile.isEmpty()){
        emit updateSpecularMapPath(chosenFile);
    }
}

/**
 * Loads the image given in the file path as the specular map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief loadSpecularMap
 * @param filePath
 */
void GLDisplay::loadSpecularMap(QString filePath)
{
  if(filePath.size()>0)
  {
       bool loaded = m_scene.loadSpecularMap(filePath.toStdString(), 0);

       if(!loaded)
       {
           QString error = QString("Could not load texture : \n%1\n\n").arg(filePath);
           emit updateLog(error);
       }
       else
       {
           QString text = QString("Texture correctly loaded : \n%1\n\n").arg(filePath);
           emit updateLog(text);
       }

       updateGL();
  }
}

/**
 * Opens a QFileDialog to choose the image corresponding to the normal map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief chooseNormalMap
 */
void GLDisplay::chooseNormalMap()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose normal map"),
                            QDir::currentPath(),
                            QString(tr("All Images files (*.jpg *.jpeg *.png *.bmp *.tif *.pfm);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TIFF (*.tif);;PFM (*.pfm)")));

    if (!chosenFile.isEmpty()){
        emit updateNormalMapPath(chosenFile);
    }
}

/**
 * Loads the image given in the file path as the normal map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief loadNormalMap
 * @param filePath
 */
void GLDisplay::loadNormalMap(QString filePath)
{
  if(filePath.size()>0)
  {
       bool loaded = m_scene.loadNormalMap(filePath.toStdString(), 0);

       if(!loaded)
       {
           QString error = QString("Could not load texture : \n%1\n\n").arg(filePath);
           emit updateLog(error);
       }
       else
       {
           QString text = QString("Texture correctly loaded : \n%1\n\n").arg(filePath);
           emit updateLog(text);
       }

       updateGL();
  }
}

/**
 * Opens a QFileDialog to choose the image corresponding to the roughness map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief chooseRoughnessMap
 */
void GLDisplay::chooseRoughnessMap()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose roughness map"),
                            QDir::currentPath(),
                            QString(tr("All Images files (*.jpg *.jpeg *.png *.bmp *.tif *.pfm);;JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp);;TIFF (*.tif);;PFM (*.pfm)")));

    if (!chosenFile.isEmpty()){
        emit updateRoughnessMapPath(chosenFile);
    }
}

/**
 * Loads the image given in the file path as the roughness map.
 * The image can be a 8 bit image or an HDR 32 bits image.
 * @brief loadRoughnessMap
 * @param filePath
 */
void GLDisplay::loadRoughnessMap(QString filePath)
{
  if(filePath.size()>0)
  {
       bool loaded = m_scene.loadRoughnessMap(filePath.toStdString(), 0);

       if(!loaded)
       {
           QString error = QString("Could not load texture : \n%1\n\n").arg(filePath);
           emit updateLog(error);
       }
       else
       {
           QString text = QString("Texture correctly loaded : \n%1\n\n").arg(filePath);
           emit updateLog(text);
       }

       updateGL();
  }
}

/**
 * Opens a QFileDialog to choose the vertex shader.
 * @brief chooseVertexShader
 */
void GLDisplay::chooseVertexShader()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose roughness map"),
                            QDir::currentPath(),
                            QString(tr("All vertex shader files (*.vsh *.vert);;VSH (*.vsh);;VERT (*.vert)")));

    if (!chosenFile.isEmpty()){
        emit updateVertexShaderPath(chosenFile);
    }
}

/**
 * Opens a QFileDialog to choose the fragment shader.
 * @brief chooseFragmentShader
 */
void GLDisplay::chooseFragmentShader()
{
    //Let the user choose a file
    QString chosenFile = QFileDialog::getOpenFileName(this,
                            tr("Choose roughness map"),
                            QDir::currentPath(),

                                                      QString(tr("All vertex shader files (*.fsh *.frag);;FSH (*.fsh);;FRAG (*.frag)")));
    if (!chosenFile.isEmpty()){
        emit updateFragmentShaderPath(chosenFile);
    }
}

/**
 * Loads a given vertex and fragment shader given their path.
 * @brief loadShaders
 * @param vertexShaderPath
 * @param fragmentShaderPath
 */
void GLDisplay::loadShaders(QString vertexShaderPath, QString fragmentShaderPath)
{
    if(vertexShaderPath.size()>0 && fragmentShaderPath.size()>0)
    {
        //Removes the previous shaders and load the new ones
        m_shaderProgram.removeAllShaders();

        m_shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, vertexShaderPath);
        m_shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, fragmentShaderPath);

        //Compiles
        if(!m_shaderProgram.link())
        {
            QString error = m_shaderProgram.log();
            emit updateLog(error);
            qDebug() << error << endl;
            qApp->exit(EXIT_FAILURE);
        }
        else
        {
           emit updateLog(QString("Shaders loaded : \n%1\n%2\n\n").arg(vertexShaderPath).arg(fragmentShaderPath));
        }
    }
    else
    {
        emit updateLog(QString("Shaders could not be loaded : \n%1\n%2\n\n").arg(vertexShaderPath).arg(fragmentShaderPath));
    }

    updateGL();
}

/**
 * Loads an environment map given its path (without the file extension assumed to be PFM).
 * @brief loadEnvironmentMap
 * @param environmentMapPathNoExtension
 * @return
 */
bool GLDisplay::loadEnvironmentMap(QString environmentMapPathNoExtension)
{
    //Assumes .pfm extension
    string EMDiffuse = environmentMapPathNoExtension.toStdString() + string("_diffuse.pfm");
    string EMRough = environmentMapPathNoExtension.toStdString() + string("_rough.pfm");
    environmentMapPathNoExtension += QString(".pfm");

    bool correctlyLoaded = m_scene.loadEnvironmentMap(environmentMapPathNoExtension.toStdString(), EMDiffuse, EMRough);

    if(correctlyLoaded)
    {
        QString log = QString("Environment map loaded : \n");
        log += environmentMapPathNoExtension + "\n\n";

        updateLog(log);
    }
    else
    {
        QString log = QString("Could not load environment maps : \n%1\n\n").arg(environmentMapPathNoExtension);

        updateLog(log);
    }
    return correctlyLoaded;
}


/**
 * Enables and disables the environment mapping.
 * @brief enableEnvironmentMapping
 * @param enableEM
 */
void GLDisplay::enableEnvironmentMapping(bool enableEM)
{
    //Enable or disable EM
    m_environmentMapping = enableEM;
    updateGL();
}
