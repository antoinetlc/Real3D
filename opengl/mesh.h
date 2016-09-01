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
 * \file mesh.h
 * \brief Implementation of a Mesh.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a Mesh. Stores the vertices, triangles, normals and texture coordinates.
 */

#ifndef MESH_H
#define MESH_H

#include "openglheaders.h"
#include <QApplication>

#include <QVector>

#include <QVector2D>
#include <QVector3D>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

class Mesh
{
    public:
        /**
         * Default Mesh constructor.
         * @brief Mesh
         */
        Mesh();

        /**
         * Loads a Mesh given its name.
         * @brief Mesh
         * @param objectName
         */
        Mesh(const std::string &objectName);

        /**
         * Reads the triangles from a .off file.
         * @brief offReader
         * @param fileName
         */
        void offReader(std::string fileName);

        /**
         * Function that returns the path of the .off file corresponding to the object.
         * Also sets the texture coordinates
         * @brief loadPathAndTextureCoordinates
         * @param objectName
         * @return
         */
        std::string loadPathAndTextureCoordinates(const std::string &objectName);

        /**
         * Set texture coordinates.
         * @brief setTextureCoordinates
         * @param textureCoordinates
         * @return
         */
        void setTextureCoordinates(QVector<QVector2D> textureCoordinates);

        /**
         * Return the vertices.
         * @brief getVertices
         * @return
         */
        QVector<QVector3D> getVertices() const;

        /**
         * Returns the array of indices that makes all the triangles in the mesh.
         * Each triangle has 3 indices that corresponds to the three vertives that makes the triangle.
         * @brief getIndices
         * @return
         */
        QVector<QVector3D> getIndices() const;

        /**
         * Returns the array of indices that makes all the triangles in the mesh.
         * Each triangle has 3 indices that corresponds to the three vertives that makes the triangle.
         * These are stored as triplets. Each consecutive triplet corresponds to a triangle
         * @brief getIndicesArray
         * @return
         */
        QVector<GLuint> getIndicesArray() const;

        /**
         * Returns the surface normals at each vertex.
         * @brief getVertexNormals
         * @return
         */
        QVector<QVector3D> getVertexNormals() const;

        /**
         * Returns the texture coordinates at each vertex.
         * @brief getTextureCoordinates
         * @return
         */
        QVector<QVector2D> getTextureCoordinates() const;

    private:

        QVector<QVector3D> m_vertices; /*!< Array of vertices. Each vertex is a position : QVector3D. */
        QVector<QVector3D> m_indices; /*!< Contains the list of indices for each triangle. QVector3D contains the 3 indices for a given triangle. */
        QVector<GLuint> m_indicesArray; /*!< Contains the list of indices for all the triangles. Each consecutive triplet of integers
                                            correspond to the three indices of the vertices that make a triangle.
                                            This is the QVector that openGL needs for rendering (in glDrawElements). */

        QVector<QVector3D> m_triangleNormals;/*!< Array that contains the normal of each triangle.. */
        QVector<QVector3D> m_vertexNormals;/*!< Array that contains the normal of each vertex. */
        QVector<QVector2D> m_textureCoordinates;/*!< Array that contains the UV texture coordinate of each triangle. */
};

#endif // MESH_H
