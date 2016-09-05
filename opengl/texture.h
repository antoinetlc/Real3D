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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <QApplication>

#include "opengl/openglheaders.h"

/*----OpenCV-------*/
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#include "maths/imageprocessing.h"

class Texture
{
    public:

        /**
         * Texture default constructor.
         * @brief Texture
         */
        Texture();

        /**
         * Creates a texture object and sets the path to an image file.
         * @brief Texture
         * @param filePath
         */
        Texture(std::string filePath);

        /**
         * Creates a texture object with a width and height and a color number of channels.
         * @brief Texture
         * @param width
         * @param height
         * @param numberOfcomponents
         */
        Texture(int width, int height, int numberOfcomponents);

        /**
          * Texture destructor.
          */
        ~Texture();

        /**
         * Load an empty texture with a (width,height,  numberOfComponents).
         * The texture is an 8 bit RGB image.
         * @brief loadEmptyTexture_8UC3
         */
        void loadEmptyTexture_8UC3();

        /**
         * Load an ampty texture with a (width,height,  numberOfComponents)
         * The texture if a 32 bits RGB image (HDR).
         * @brief loadEmptyTexture_32FC3
         */
        void loadEmptyTexture_32FC3();

        /**
         * Load textures saved as 8 bits images.
         * Returns true if the texture has been correctly loaded.
         * @brief load_8UC3
         * @return
         */
        bool load_8UC3();

        /**
         * Load textures saved as PFM files.
         * Returns true if the texture has been correctly loaded.
         * @brief load_32FC3
         * @return
         */
        bool load_32FC3();

        /**
         * Load a texture from an opencv matrix.
         * Returns true if the texture has been correctly loaded.
         * @brief loadFromMat_32FC3
         * @param matrix
         * @return
         */
        bool loadFromMat_32FC3(cv::Mat &matrix);

        /**
         * Set the filename of the texture.
         * @brief setFileName
         * @param fileName
         */
        void setFileName(std::string fileName);

        /**
         * Returns the texture id.
         * @brief getTextureId
         * @return
         */
        GLuint getTextureId() const;

        /**
         * Returns the aspect ratio.
         * @brief getAspectRatio
         * @return
         */
        float getAspectRatio() const;

        /**
         * Returns the width of the image.
         * @brief getWidth
         * @return
         */
        int getWidth() const;

        /**
         * Returns the height of the image.
         * @brief getHeight
         * @return
         */
        int getHeight() const;

        /**
         * Return true if the texture was loaded previously.
         * @brief isLoaded
         * @return
         */
        bool isLoaded() const;


    private:
        GLuint m_textureId; /*!< Texture ID. */
        std::string m_filePath; /*!< Path to the texture image file. */
        bool m_isLoaded; /*!< Boolean that tells if the texture has previously been loaded. */

        int m_width; /*!< Width of the texture */
        int m_height; /*!< Height of the texture */
        int m_numberOfComponents; /*!< Number of color channels of the texture */

};

#endif // TEXTURE_H
