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
 * \file imageprocessing.h
 * \brief Implementation of functions related to image processing.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of few functions related to image processing including applying
 * and removing gamma correction and flipping an image vertically.
 */

#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#define M_PI 3.14159265358979323846

#include "maths/mathfunctions.h"
#include "other/PFMReadWrite.h"

#include <iostream>
#include <cmath>
#include <vector>

#include <QApplication>
#include <QVector3D>

#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>

/**
 * Apply a gamma correction to a RGB image (OpenCV Mat image).
 * @param INPUT : rgbImage is the image to which the gamma correction is applied.
 * @param OUTPUT : rgbImageWithGamma is the rgbImage with the gamma applied.
 * @param INPUT : gamma is a double corresponding to the value of the gamma correction.
 */
void gammaCorrection(const cv::Mat &rgbImage, cv::Mat &rgbImageWithGamma, double gamma);

/**
 * Apply a gamma correction to a RGB image stored in an array of floats.
 * @param INPUT : image is the image to which the gamma correction is applied. It is an array of floats.
 * @param INPUT : width is the width of the image.
 * @param INPUT : height is the height of the image.
 * @param INPUT : numberOfComponents is the number of components of the image (3 for a RGB image).
 * @param INPUT : gamma is a float corresponding to the value of the gamma correction.
 */
void gammaCorrection(float* image, unsigned int& width, unsigned int& height, unsigned int& numberOfComponents,const float& gamma);

/**
 * Remove the gamma correction of a RGB image (OpenCV Mat image).
 * @param INPUT : rgbImage is the image to which the gamma correction is removed. rgbImage is an OpenCV CV_8UC3 matrix. (matrix of 3 channels of 8 bits unsigned char).
 * @param OUTPUT : rgbImageWithGamma is the rgbImage with the gamma removed. It is a CV_32FC3 matrix (matrix of 3 channels of 32 bits floats).
 * @param INPUT : gamma is a double corresponding to the value of the gamma correction.
 */
void removeGammaCorrection(const cv::Mat &rgbImage, cv::Mat &rgbImageWithoutGamma, double gamma);

/**
 * Flips imageSource vertically.
 * @brief inverseYAxis
 * @param imageSource
 * @param result
 */
void inverseYAxis(const cv::Mat& imageSource, cv::Mat& result);

#endif // IMAGEPROCESSING_H

