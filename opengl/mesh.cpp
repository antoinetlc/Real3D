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
 * \file mesh.cpp
 * \brief Implementation of a Mesh.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Implementation of a Mesh. Stores the vertices, triangles, normals and texture coordinates.
 */

#include "opengl/mesh.h"

using namespace std;

/**
 * Default Mesh constructor.
 * @brief Mesh
 */
Mesh::Mesh():m_vertices(QVector<QVector3D>()), m_indices(QVector<QVector3D>()),
             m_indicesArray(QVector<GLuint>()), m_triangleNormals( QVector<QVector3D>()), m_vertexNormals( QVector<QVector3D>()),
             m_textureCoordinates(QVector<QVector2D>())
{

}

/**
 * Loads a Mesh given its name.
 * @brief Mesh
 * @param objectName
 */
Mesh::Mesh(const string& objectName):m_vertices(QVector<QVector3D>()), m_indices(QVector<QVector3D>()),
                            m_indicesArray(QVector<GLuint>()), m_triangleNormals( QVector<QVector3D>()), m_vertexNormals( QVector<QVector3D>()),
                            m_textureCoordinates(QVector<QVector2D>())
{
    string fileName = loadPathAndTextureCoordinates(objectName);
    offReader(fileName);
}

/**
 * Reads the triangles from a .off file.
 * @brief offReader
 * @param fileName
 */
void Mesh::offReader(string fileName)
{
    ifstream file(fileName.c_str(), ios::in);

    string fileType;
    string numberOfVertices, numberOfTriangles, numberOfEdges;
    string data;
    string xVertex, yVertex, zVertex;
    string vertex1, vertex2, vertex3;
    int index1 = 0, index2 = 0, index3 = 0;
    string numberOfIndices;

    file >> fileType;
    //Add and exception


    file >> numberOfVertices >> numberOfTriangles >> numberOfEdges;
    m_vertices.resize(atoi(numberOfVertices.c_str()));
    m_indices.resize(atoi(numberOfTriangles.c_str()));

    //Blank line
    getline(file, data);

     //Vertices
    for(int i = 0 ; i< atoi(numberOfVertices.c_str()) ; i++)
    {
        file >> xVertex >> yVertex >>zVertex;
        m_vertices[i] = QVector3D(atof(xVertex.c_str()),atof(yVertex.c_str()),atof(zVertex.c_str()));
    }

    //Indices and normals for each triangle
    QVector3D crossProduct;
    for(int i = 0 ; i< atoi(numberOfTriangles.c_str()) ; i++)
    {
         file >> numberOfIndices >> vertex1 >> vertex2 >> vertex3;
         index1 = atoi(vertex1.c_str());
         index2 = atoi(vertex2.c_str());
         index3 = atoi(vertex3.c_str());

         //Indices for each triangle
         m_indices[i] = QVector3D(index1, index2, index3);

         //List of indices for OpenGL rendering
         m_indicesArray.push_back(index1);
         m_indicesArray.push_back(index2);
         m_indicesArray.push_back(index3);

         //Compute the normal NORMALIZED
         /*   v3
          * v1__v2
          * normal = (v2-v1)^(v3-v1)
          */
         crossProduct = QVector3D::crossProduct(m_vertices[index1]-m_vertices[index2],m_vertices[index1]-m_vertices[index3]);
         crossProduct.normalize();
         m_triangleNormals.push_back(crossProduct);
    }

    m_vertexNormals.resize(m_vertices.size());
    QVector3D vector1;
    QVector3D vector2;

    //Compute the normals for each vertex
    for(int k = 0 ; k<m_vertices.size() ; k++)
    {
        m_vertexNormals[k] = QVector3D(0.0, 0.0, 0.0);
        for(int i = 0 ; i<m_indices.size() ; i++)
        {
            //If the vertex k belongs to triangle i
            if(m_indices[i].x() == k)
            {
                /*   z
                 * kx__y
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].y()]- m_vertices[k];
                vector2 = m_vertices[m_indices[i].z()]- m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
            else if(m_indices[i].y() == k)
            {
                /*   z
                 * x__ky
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].z()]- m_vertices[k];
                vector2 = m_vertices[m_indices[i].x()]- m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
            else if(m_indices[i].z() == k)
            {
                /*   kz
                 * x__y
                 * angle = acos(kxy.kxz)
                 */
                vector1 = m_vertices[m_indices[i].x()]- m_vertices[k];
                vector2 = m_vertices[m_indices[i].y()]- m_vertices[k];
                vector1.normalize();
                vector2.normalize();
                m_vertexNormals[k] += acos(QVector3D::dotProduct(vector1, vector2))*m_triangleNormals[i];
            }
        }
        m_vertexNormals[k].normalize();
    }
}

/**
 * Function that returns the path of the .off file corresponding to the object.
 * Also sets the texture coordinates
 * @brief loadPathAndTextureCoordinates
 * @param objectName
 * @return
 */
string Mesh::loadPathAndTextureCoordinates(const string &objectName)
{
    string objectPath;

    if(objectName == "square")
    {
        #ifdef _WIN32
                objectPath = string(qApp->applicationDirPath().toStdString()+ "\\off\\square.off");
        #endif

        #ifdef __gnu_linux__
                objectPath = qApp->applicationDirPath().toStdString()+string( "/off/square.off");
        #endif

        #if defined(__APPLE__) && defined(__MACH__)
                objectPath = qApp->applicationDirPath().toStdString()+string( "/../../../off/square.off");
        #endif
                /*The square is defined as follows :
                 * v1------v0
                 * |        |
                 * |        |
                 * v2------v3
                 * First triangle v2-v3-v0
                 * Second triangle v0-v1-v2
                 */

                 //Texture coordinates
                //The vertices are stored in the order v0, v1, v2, v3
                 //Then the triangles are formed using the indices : First triangle v2-v3-v0 Second triangle v0-v1-v2
                 //Hence only 4 texture coordinates are required : one for each vertex (and not 6).

                 m_textureCoordinates.push_back(QVector2D(1.0,1.0));
                 m_textureCoordinates.push_back(QVector2D(0.0,1.0));
                 m_textureCoordinates.push_back(QVector2D(0.0,0.0));
                 m_textureCoordinates.push_back(QVector2D(1.0,0.0));
    }

    return objectPath;
}

/**
 * Set texture coordinates.
 * @brief setTextureCoordinates
 * @param textureCoordinates
 * @return
 */
void Mesh::setTextureCoordinates(QVector<QVector2D> textureCoordinates)
{
    m_textureCoordinates = textureCoordinates;
}

/**
 * Return the vertices.
 * @brief getVertices
 * @return
 */
QVector<QVector3D> Mesh::getVertices() const
{
    return m_vertices;
}


/**
 * Returns the array of indices that makes all the triangles in the mesh.
 * Each triangle has 3 indices that corresponds to the three vertives that makes the triangle.
 * @brief getIndices
 * @return
 */
QVector<QVector3D> Mesh::getIndices() const
{
    return m_indices;
}

/**
 * Returns the array of indices that makes all the triangles in the mesh.
 * Each triangle has 3 indices that corresponds to the three vertives that makes the triangle.
 * These are stored as triplets. Each consecutive triplet corresponds to a triangle
 * @brief getIndicesArray
 * @return
 */
QVector<GLuint> Mesh::getIndicesArray() const
{
    return m_indicesArray;
}

/**
 * Returns the surface normals at each vertex.
 * @brief getVertexNormals
 * @return
 */
QVector<QVector3D> Mesh::getVertexNormals() const
{
    return m_vertexNormals;
}

/**
 * Returns the texture coordinates at each vertex.
 * @brief getTextureCoordinates
 * @return
 */
QVector<QVector2D> Mesh::getTextureCoordinates() const
{
    return m_textureCoordinates;
}

