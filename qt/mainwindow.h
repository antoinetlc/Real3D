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
 * \file mainwindow.h
 * \brief Mainwindow.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Mainwindow that contains the OpenGL widget and the user interface.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qt/gldisplay.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        /**
         * Main window constructor.
         * @brief MainWindow
         * @param parent
         */
        explicit MainWindow(QWidget *parent = 0);

        /**
          * Mainwindow destructor.
          */
        ~MainWindow();

        /**
         * Given an environment map name returns a file path to a PFM image.
         * @brief EMNameToFilePath
         * @param environmentMapName
         * @return
         */
        QString EMNameToFilePath(QString environmentMapName);

    public slots:

        /**
         * Loads the vertex and fragment shaders.
         * @brief loadShaders
         */
        void loadShaders();

        /**
         * Load an environment map.
         * @brief loadEnvironmentMap
         */
        void loadEnvironmentMap();

        /**
         * Load an environment map given its name.
         * @brief loadEnvironmentMap
         * @param environmentMapName
         */
        void loadEnvironmentMap(QString environmentMapName);



    private:
        Ui::MainWindow *ui; /*!< Designer user interface. */

};

#endif // MAINWINDOW_H
