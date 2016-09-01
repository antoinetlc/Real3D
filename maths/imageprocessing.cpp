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
 * \file imageprocessing.cpp
 * \brief Implementation of functions related to image processing.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of few functions related to image processing including applying
 * and removing gamma correction and flipping an image vertically.
 */


#include "maths/imageprocessing.h"

using namespace std;
using namespace cv;

/**
 * Apply a gamma correction to a RGB image (OpenCV Mat image).
 * @param INPUT : rgbImage is the image to which the gamma correction is applied.
 * @param OUTPUT : rgbImageWithGamma is the rgbImage with the gamma applied.
 * @param INPUT : gamma is a double corresponding to the value of the gamma correction.
 */
void gammaCorrection(const Mat &rgbImage, Mat &rgbImageWithGamma, double gamma)
{
    Mat channel[3],channel32F[3], channelWithGamma[3];

    split(rgbImage, channel);

    channel[0].convertTo(channel32F[0], CV_32F);
    channel[1].convertTo(channel32F[1], CV_32F);
    channel[2].convertTo(channel32F[2], CV_32F);

    pow(channel32F[0], 1.0/gamma, channelWithGamma[0]);
    pow(channel32F[1], 1.0/gamma, channelWithGamma[1]);
    pow(channel32F[2], 1.0/gamma, channelWithGamma[2]);

    merge(channelWithGamma,3,rgbImageWithGamma);
}

/**
 * Apply a gamma correction to a RGB image stored in an array of floats.
 * @param INPUT : image is the image to which the gamma correction is applied. It is an array of floats.
 * @param INPUT : width is the width of the image.
 * @param INPUT : height is the height of the image.
 * @param INPUT : numberOfComponents is the number of components of the image (3 for a RGB image).
 * @param INPUT : gamma is a float corresponding to the value of the gamma correction.
 */
void gammaCorrection(float* image, unsigned int& width, unsigned int& height, unsigned int& numberOfComponents,const float& gamma)
{
    float pixelR = 0.0f, pixelG = 0.0f, pixelB = 0.0f;

    for(unsigned int i = 0 ; i< height; i++)
    {
        for(unsigned int j = 0 ; j< width; j++)
        {
            pixelR = image[(i*width+j)*numberOfComponents];
            pixelG = image[(i*width+j)*numberOfComponents+1];
            pixelB = image[(i*width+j)*numberOfComponents+2];

            image[(i*width+j)*numberOfComponents] = clamp(pow(pixelR, 1.0/gamma), 0.0, 1.0);
            image[(i*width+j)*numberOfComponents+1] = clamp(pow(pixelG, 1.0/gamma), 0.0 , 1.0);
            image[(i*width+j)*numberOfComponents+2] = clamp(pow(pixelB, 1.0/gamma), 0.0, 1.0);
        }
    }

}

/**
 * Remove the gamma correction of a RGB image (OpenCV Mat image).
 * @param INPUT : rgbImage is the image to which the gamma correction is removed. rgbImage is an OpenCV CV_8UC3 matrix. (matrix of 3 channels of 8 bits unsigned char).
 * @param OUTPUT : rgbImageWithGamma is the rgbImage with the gamma removed. It is a CV_32FC3 matrix (matrix of 3 channels of 32 bits floats).
 * @param INPUT : gamma is a double corresponding to the value of the gamma correction.
 */
void removeGammaCorrection(const Mat &rgbImage, Mat &rgbImageWithoutGamma, double gamma)
{
    Mat channel[3], channel32F[3], channelWithoutGamma[3];

    split(rgbImage, channel);

    channel[0].convertTo(channel32F[0], CV_32F);
    channel[1].convertTo(channel32F[1], CV_32F);
    channel[2].convertTo(channel32F[2], CV_32F);

    pow(channel32F[0], gamma, channelWithoutGamma[0]);
    pow(channel32F[1], gamma, channelWithoutGamma[1]);
    pow(channel32F[2], gamma, channelWithoutGamma[2]);

    merge(channelWithoutGamma,3,rgbImageWithoutGamma);
}

/**
 * Flips imageSource vertically.
 * @brief inverseYAxis
 * @param imageSource
 * @param result
 */
void inverseYAxis(const Mat& imageSource, Mat& result)
{
    result.convertTo(result, CV_32FC3);

    int width = imageSource.cols;
    int height = imageSource.rows;

    for(int i = 0 ; i<height ; i++)
    {
        for(int j = 0 ; j<width ; j++)
        {
            result.at<Vec3f>(height-1-i,j).val[0] = imageSource.at<Vec3f>(i,j).val[0];
            result.at<Vec3f>(height-1-i,j).val[1] = imageSource.at<Vec3f>(i,j).val[1];
            result.at<Vec3f>(height-1-i,j).val[2] = imageSource.at<Vec3f>(i,j).val[2];
        }
    }

}

