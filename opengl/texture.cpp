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
 * \file texture.h
 * \brief Implementation of a Texture object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a Texture object. Loads a Texture on the GPU with OpenGL.
 */

#include "texture.h"

using namespace std;
using namespace cv;

/**
 * Texture default constructor.
 * @brief Texture
 */
Texture::Texture(): m_textureId(0), m_filePath(""), m_isLoaded(false), m_width(0), m_height(0), m_numberOfComponents(0)
{

}

/**
 * Creates a texture object and sets the path to an image file.
 * @brief Texture
 * @param filePath
 */
Texture::Texture(string filePath): m_textureId(0), m_filePath(filePath), m_isLoaded(false), m_width(0), m_height(0), m_numberOfComponents(0)
{

}

/**
 * Creates a texture object with a width and height and a color number of channels.
 * @brief Texture
 * @param width
 * @param height
 * @param numberOfcomponents
 */
Texture::Texture(int width, int height, int numberOfcomponents): m_textureId(0), m_filePath(string("")), m_isLoaded(false), m_width(width), m_height(height), m_numberOfComponents(numberOfcomponents)
{

}


/**
  * Texture destructor.
  */
Texture::~Texture()
{

}

/**
 * Load an empty texture with a (width,height,  numberOfComponents).
 * The texture is an 8 bit RGB image.
 * @brief loadEmptyTexture_8UC3
 */
void Texture::loadEmptyTexture_8UC3()
{
    //remove an eventual previous picture from the memory
    if(glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Generate a new texture ID
    glGenTextures(1, &m_textureId);

    //Bind the texture and start working on it
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    //Allocate memory for a width*height texture but without data
    glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);

    //Smooth close textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Do not smooth textures that are far away (performance optimisation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_isLoaded = true;
}

/**
 * Load an ampty texture with a (width,height,  numberOfComponents)
 * The texture if a 32 bits RGB image (HDR).
 * @brief loadEmptyTexture_32FC3
 */
void Texture::loadEmptyTexture_32FC3()
{
    //remove an eventual previous picture from the memory
    if(glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Generate a new texture ID
    glGenTextures(1, &m_textureId);

    //Bind the texture and start working on it
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    //Allocate memory for a width*height texture but without data
    glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);

    //Smooth close textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Do not smooth textures that are far away (performance optimisation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_isLoaded = true;
}

/**
 * Load textures saved as 8 bits images.
 * Returns true if the texture has been correctly loaded.
 * @brief load_8UC3
 * @return
 */
bool Texture::load_8UC3()
{
    //remove an eventual previous picture from the memory
    if(glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Load the texture in BGR format
    cout << m_filePath << endl;
    Mat texture = imread(m_filePath, CV_LOAD_IMAGE_COLOR);

    texture.convertTo(texture, CV_32FC3);
    texture /= 255.0; //Divide by 255 to have float values

    //If the texture cannot be loaded
    if(!texture.data)
    {
        cout << "Could not load the texture : " << m_filePath << endl;
        m_isLoaded = false;
        return m_isLoaded;
    }
    else
    {
        m_width = texture.cols;
        m_height = texture.rows;
        m_numberOfComponents = texture.channels();

        //Generate the texture id
        glGenTextures(1, &m_textureId);

        //Bind a texture 2D to the texture
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        //Send the data to the memory
        Mat inversedTexture = texture.clone();

        //The texture has to be inverted as the coordinate system for the (u,v) coordinate and the OpenCV image are different
        inverseYAxis(texture, inversedTexture);

        //The first GL_RGB says that the data used will be in the RGB format
        // !!!!!!!!!!!!!!!!!!!!!! GL_RGB clamps the texture between 0 and 1 range.
        // To use floats above 1 : us GL_RGB32F
        //The second one says that texture.data is stored in BGR format (OpenCV)
        glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_FLOAT, inversedTexture.data);

        //Smooth close textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Do not smooth textures that are far away (performance optimisation)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    m_isLoaded = true;
    //Texture correctly loaded
    return m_isLoaded;
}

/**
 * Load textures saved as PFM files.
 * Returns true if the texture has been correctly loaded.
 * @brief load_32FC3
 * @return
 */
bool Texture::load_32FC3()
{
    //remove an eventual previous picture from the memory
    if(glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Load the texture in BGR format
    cout << m_filePath << endl;
    Mat texture = loadPFM(m_filePath);
    texture.convertTo(texture, CV_32FC3);

    //If the texture cannot be loaded
    if(!texture.data)
    {
        cout << "Could not load the texture : " << m_filePath << endl;
        m_isLoaded = false;
        return m_isLoaded;
    }
    else
    {
        m_width = texture.cols;
        m_height = texture.rows;
        m_numberOfComponents = texture.channels();

        //Generate the texture id
        glGenTextures(1, &m_textureId);

        //Bind a texture 2D to the texture
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        //Send the data to the memory
        Mat inversedTexture = texture.clone();

        //The texture has to be inverted as the coordinate system for the (u,v) coordinate and the OpenCV image are different
        inverseYAxis(texture, inversedTexture);

        //The first GL_RGB says that the data used will be in the RGB format
        // !!!!!!!!!!!!!!!!!!!!!! GL_RGB clamps the texture between 0 and 1 range.
        // To use floats above 1 : us GL_RGB32F
        //The second one says that texture.data is stored in BGR format (OpenCV)
        glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB32F, m_width, m_height, 0, GL_BGR, GL_FLOAT, inversedTexture.data);


        //Smooth close textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Do not smooth textures that are far away (performance optimisation)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    //Texture correctly loaded
    m_isLoaded = true;
    return m_isLoaded;
}

/**
 * Load a texture from an opencv matrix.
 * Returns true if the texture has been correctly loaded.
 * @brief loadFromMat_32FC3
 * @param matrix
 * @return
 */
bool Texture::loadFromMat_32FC3(Mat &matrix)
{
    //remove an eventual previous picture from the memory
    if(glIsTexture(m_textureId) == GL_TRUE)
    {
        glDeleteTextures(1, &m_textureId);
    }

    //Load the texture in BGR format
    //If the texture cannot be loaded
    if(!matrix.data)
    {
        cout << "Could not load from empty opencv matrix " << endl;
        m_isLoaded = false;
        return m_isLoaded;
    }
    else
    {
        m_width = matrix.cols;
        m_height = matrix.rows;
        m_numberOfComponents = matrix.channels();

        //Generate the texture id
        glGenTextures(1, &m_textureId);

        //Bind a texture 2D to the texture
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        //Send the data to the memory
        Mat inversedTexture = matrix.clone();

        //The texture has to be inverted as the coordinate system for the (u,v) coordinate and the OpenCV image are different
        inverseYAxis(matrix, inversedTexture);

        //The first GL_RGB says that the data used will be in the RGB format
        // !!!!!!!!!!!!!!!!!!!!!! GL_RGB clamps the texture between 0 and 1 range.
        // To use floats above 1 : us GL_RGB32F
        //The second one says that texture.data is stored in BGR format (OpenCV)
        glTexImage2D(GL_TEXTURE_2D , 0, GL_RGB32F, m_width, m_height, 0, GL_BGR, GL_FLOAT, inversedTexture.data);

        //Smooth close textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //Do not smooth textures that are far away (performance optimisation)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //Texture correctly loaded
    m_isLoaded = true;
    return m_isLoaded;
}


/**
 * Set the filename of the texture.
 * @brief setFileName
 * @param fileName
 */
void Texture::setFileName(string filePath)
{
    cout << m_filePath << endl;
    this->m_filePath = filePath;
}

/**
 * Returns the texture id.
 * @brief getTextureId
 * @return
 */
GLuint Texture::getTextureId() const
{
    return m_textureId;
}

/**
 * Returns the aspect ratio.
 * @brief getAspectRatio
 * @return
 */
float Texture::getAspectRatio() const
{
    return (float)m_width/(float)m_height;
}


/**
 * Returns the width of the image.
 * @brief getWidth
 * @return
 */
int Texture::getWidth() const
{
    return m_width;
}

/**
 * Returns the height of the image.
 * @brief getHeight
 * @return
 */
int Texture::getHeight() const
{
    return m_height;
}

/**
 * Return true if the texture was loaded previously.
 * @brief isLoaded
 * @return
 */
bool Texture::isLoaded() const
{
    return m_isLoaded;
}
