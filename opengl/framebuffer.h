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
 * \file frameBuffer.h
 * \brief Implementation of the Framebuffer object.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of an OpenGL Framebuffer object.
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "opengl/texture.h"
#include "opengl/openglheaders.h"

#include <iostream>
#include <string>
#include <vector>

#include <QApplication>

class FrameBuffer
{
    public:

        /**
         * Dafault framebuffer constructor.
         * @brief FrameBuffer
         */
        FrameBuffer();

        /**
         * Creates a framebuffer of width x height.
         * @brief FrameBuffer
         * @param width
         * @param height
         */
        FrameBuffer(int width, int height);

        /**
          * Destructor.
          */
        ~FrameBuffer();

        /**
         * Creates a render buffer given its id and format.
         * @brief createRenderBuffer
         * @param id
         * @param format
         */
        void createRenderBuffer(GLuint &id, GLenum format);

        /**
         * Load a framebuffer with a color buffer of 8 bits.
         * Returns true if correcly loaded.
         * @brief load_8UC3
         * @return
         */
        bool load_8UC3();

        /**
         * Load a framebuffer with a color buffer of 32 bits floats.
         * Returns true if correcly loaded.
         * @brief load_32FC3
         * @return
         */
        bool load_32FC3();

        /**
         * Returns the framebuffer id.
         * @brief getFramebufferID
         * @return
         */
        GLuint getFramebufferID() const;

        /**
         * Gets the ID of the color buffer number index.
         * @brief getColorBufferID
         * @param index
         * @return
         */
        GLuint getColorBufferID(unsigned int index) const;

        /**
         * Gets the width of the framebuffer.
         * @brief getWidth
         * @return
         */
        int getWidth() const;

        /**
         * Gets the height of the framebuffer.
         * @brief getHeight
         * @return
         */
        int getHeight() const;

    private:

        GLuint m_framebufferId; /*!< Framebuffer ID*/
        int m_width; /*!< Frambuffer width*/
        int m_height; /*!< Framebuffer height*/

        //A framebuffer contains a color buffer, a depth buffer and a stencil buffer
        std::vector<Texture> m_colourBuffers; /*!< Framebuffer color buffers*/
        GLuint m_depthBufferId; /*!< Framebuffer depth buffer Id*/
};

#endif // FRAMEBUFFER_H
