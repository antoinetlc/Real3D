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
 * \file mainwindow.cpp
 * \brief Mainwindow.
 * \author Antoine Toisoul Le Cann
 * \date September, 1st, 2016
 *
 * Mainwindow that contains the OpenGL widget and the user interface.
 */


#include "qt/mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

/**
 * Main window constructor.
 * @brief MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //Enable multisampling
    QGLFormat format = ui->m_glWidget->format();
    format.setSamples(16);
    ui->m_glWidget->setFormat(format);

}

/**
  * Mainwindow destructor.
  */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Given an environment map name returns a file path to a PFM image.
 * @brief EMNameToFilePath
 * @param environmentMapName
 * @return
 */
QString MainWindow::EMNameToFilePath(QString environmentMapName)
{

    if(environmentMapName == "Grace Cathedral")
    {
        QString filePathNoExtension = qApp->applicationDirPath() + QString("/EnvironmentMaps/grace");

        return filePathNoExtension;
    }
    else if(environmentMapName == "Uffizi Gallery")
    {
        QString filePathNoExtension = qApp->applicationDirPath() + QString("/EnvironmentMaps/uffizi");

        return filePathNoExtension;
    }
    else if(environmentMapName == "St Peter's Basilica")
    {
        QString filePathNoExtension = qApp->applicationDirPath() + QString("/EnvironmentMaps/stpeters");

        return filePathNoExtension;
    }
    else if(environmentMapName == "Pisa Courtyard")
    {
        QString filePathNoExtension = qApp->applicationDirPath() + QString("/EnvironmentMaps/pisa");

        return filePathNoExtension;
    }
    else if(environmentMapName == "Bar")
    {
        QString filePathNoExtension = qApp->applicationDirPath() + QString("/EnvironmentMaps/bar");

        return filePathNoExtension;
    }

    return QString("");
}

/**
 * Loads the vertex and fragment shaders.
 * @brief loadShaders
 */
void MainWindow::loadShaders()
{
    QString vertexPath = ui->m_vertexShaderLineEdit->text();
    QString fragmentPath = ui->m_fragmentShaderLineEdit->text();

    ui->m_glWidget->loadShaders(vertexPath, fragmentPath);
}

/**
 * Load an environment map.
 * @brief loadEnvironmentMap
 */
void MainWindow::loadEnvironmentMap()
{
    bool isEMEnabled = ui->m_EMCheckbox->isChecked();

    if(isEMEnabled)
    {
        QString filePathNoExtension = EMNameToFilePath(ui->m_EMComboBox->currentText());

        bool correctlyLoaded = ui->m_glWidget->loadEnvironmentMap(filePathNoExtension);

        //Only starts environment mapping if the EM is loaded correctly
        if(correctlyLoaded)
        {
            ui->m_glWidget->enableEnvironmentMapping(isEMEnabled);
        }
    }
    else
    {
        ui->m_glWidget->enableEnvironmentMapping(isEMEnabled);
    }
}

/**
 * Load an environment map given its name.
 * @brief loadEnvironmentMap
 * @param environmentMapName
 */
void MainWindow::loadEnvironmentMap(QString environmentMapName)
{
    bool isEMEnabled = ui->m_EMCheckbox->isChecked();

    //If environment mapping is enable
    if(isEMEnabled)
    {
        QString filePathNoExtension = EMNameToFilePath(environmentMapName);

        bool correctlyLoaded = ui->m_glWidget->loadEnvironmentMap(filePathNoExtension);

        //Only starts environment mapping if the EM is loaded correctly
        if(correctlyLoaded)
        {
            ui->m_glWidget->enableEnvironmentMapping(isEMEnabled);
        }
    }
    else
    {
        ui->m_glWidget->enableEnvironmentMapping(isEMEnabled);
    }

}


