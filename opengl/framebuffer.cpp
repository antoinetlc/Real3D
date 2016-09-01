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
 * \file frameBuffer.cpp
 * \brief Implementation of the Framebuffer object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an OpenGL Framebyuffer object.
 */

#include "framebuffer.h"

using namespace std;

/**
 * Dafault framebuffer constructor.
 * @brief FrameBuffer
 */
FrameBuffer::FrameBuffer(): m_framebufferId(0), m_width(0), m_height(0),
    m_colourBuffers(vector<Texture>()), m_depthBufferId(0)
{

}

/**
 * Creates a framebuffer of width x height.
 * @brief FrameBuffer
 * @param width
 * @param height
 */
FrameBuffer::FrameBuffer(int width, int height): m_framebufferId(0), m_width(width), m_height(height),
    m_colourBuffers(vector<Texture>()), m_depthBufferId(0)
{

}

/**
  * Destructor.
  */
FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_framebufferId);
    glDeleteRenderbuffers(1, &m_depthBufferId);
    m_colourBuffers.clear();

}

/**
 * Creates a render buffer given its id and format.
 * @brief createRenderBuffer
 * @param id
 * @param format
 */
void FrameBuffer::createRenderBuffer(GLuint &id, GLenum format)
{
    //Delete any previous buffer
    if(glIsRenderbuffer(id) == GL_TRUE)
    {
        //1 is the number of id to delete
         glDeleteRenderbuffers(1, &id);
    }

    //Generate a new id for the renderbuffer of the Framebuffer
     glGenRenderbuffers(1, &id);

    //Start working with the renderbuffer : bing
     glBindRenderbuffer(GL_RENDERBUFFER, id);

     //Create renderbuffer with same width and height
     glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height);

    //Stop working with it
     glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

/**
 * Load a framebuffer with a color buffer of 8 bits.
 * Returns true if correcly loaded.
 * @brief load_8UC3
 * @return
 */
bool FrameBuffer::load_8UC3()
{

    if(glIsFramebuffer(m_framebufferId) == GL_TRUE)
    {
        glDeleteFramebuffers(1, &m_framebufferId);
        m_colourBuffers.clear(); //Empty colour buffers
    }

    //Generate ID
    glGenFramebuffers(1, &m_framebufferId);

    //Create renderbuffer with same width and height
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

    //Colorbuffer
    Texture colorBuffer = Texture(m_width, m_height, 3);
    colorBuffer.loadEmptyTexture_8UC3();

    m_colourBuffers.push_back(colorBuffer);

    //Renderbuffer
    this->createRenderBuffer(m_depthBufferId, GL_DEPTH24_STENCIL8);

    //Attach colour buffer and render buffer to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffers[0].getTextureId(),0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Error in framebuffer creation. Clear memory." << endl;

        //Clear the memory associated with the framebuffer, renderbuffer and color buffer
        glDeleteFramebuffers(1, &m_framebufferId);
        glDeleteRenderbuffers(1, &m_depthBufferId);
        m_colourBuffers.clear();

        return false;
    }

    //Stop working with it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;

}

/**
 * Load a framebuffer with a color buffer of 32 bits floats.
 * Returns true if correcly loaded.
 * @brief load_32FC3
 * @return
 */
bool FrameBuffer::load_32FC3()
{

    if(glIsFramebuffer(m_framebufferId) == GL_TRUE)
    {
        glDeleteFramebuffers(1, &m_framebufferId);
        m_colourBuffers.clear(); //Empty colour buffers
    }

    //Generate ID
    glGenFramebuffers(1, &m_framebufferId);

    //Create renderbuffer with same width and height
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId);

    //Colorbuffer
    Texture colorBuffer = Texture(m_width, m_height, 3);


    colorBuffer.loadEmptyTexture_32FC3();


    m_colourBuffers.push_back(colorBuffer);

    //Renderbuffer
    this->createRenderBuffer(m_depthBufferId, GL_DEPTH24_STENCIL8);

    //Attach colour buffer and render buffer to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffers[0].getTextureId(),0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Error in framebuffer creation. Clear memory." << endl;

        //Clear the memory associated with the framebuffer, renderbuffer and color buffer
        glDeleteFramebuffers(1, &m_framebufferId);
        glDeleteRenderbuffers(1, &m_depthBufferId);
        m_colourBuffers.clear();

        return false;
    }

    //Stop working with it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;

}

/**
 * Returns the framebuffer id.
 * @brief getFramebufferID
 * @return
 */
GLuint FrameBuffer::getFramebufferID() const
{
    return m_framebufferId;
}

/**
 * Gets the ID of the color buffer number index.
 * @brief getColorBufferID
 * @param index
 * @return
 */
GLuint FrameBuffer::getColorBufferID(unsigned int index) const
{
    return m_colourBuffers[index].getTextureId();
}

/**
 * Gets the width of the framebuffer.
 * @brief getWidth
 * @return
 */
int FrameBuffer::getWidth() const
{
    return m_width;
}


/**
 * Gets the height of the framebuffer.
 * @brief getHeight
 * @return
 */
int FrameBuffer::getHeight() const
{
    return m_height;
}
