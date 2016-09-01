# Real3D

Real3D is an open-source software for real-time realistic renderings.
It can load reflectance maps (diffuse, specular, roughness and normal maps) in order to make photorealistic renderings in real-time. The program allows rendering and animation under a point light source and an environment map.
This software was implemented during my PhD at Imperial College London.

###Version
Version 1.0

###Compilation
This program has been compiled and tested both on Windows and Linux environments.
It requires the following libraries/API in order to compile :

* OpenGL and GLEW
* OpenCV (tested with version 2.4.11)
* Qt (tested with version 5.4)

A "Real3D.pro" file is provided for compilation with QtCreator IDE. Please update the libraries paths to match your installation.

###Installation
Please copy the "shaders" and "off" folders in the same directory where the program is compiled.
By default the program loads a phong shader. Different shaders and reflectance maps can be loaded from the user interface.

####Environment mapping
For the environment mapping to work you will have to download the **latitude longitude maps** of the environment.
Some are available at the following links :

* http://www.pauldebevec.com/Probes/
* http://gl.ict.usc.edu/Data/HighResProbes/

Create a folder called "EnvironmentMaps" in the directory where the program is compiled and name the environment maps as follows : 

| Environment  | File Name |
| ------------- | ------------- |
| Grace Cathedral | grace  |
| Uffizi Gallery  | uffizi  |
| St Peter's Basilica | stpeters  |
| Pisa Courtyard | pisa  |

The diffuse convolution and rough specular convolutions of the environment map have to be precomputed and put in the same folder. These must have the following names : 

* EM_diffuse 
* EM_rough

where EM is the file name of your environment map.

For example for a rendering in the grace cathedral, three environment maps have to be in the "EnvironmentMaps" folder with the names : "grace.pfm", "grace_diffuse.pfm" and "grace_rough.pfm".


###User interface
The camera can be rotated by using the mouse left click and moving the mouse. The mouse wheel makes the camera closer or further away from the origin.

The light source can be translated along the x and y axis by with pressing CTRL+left mouse click and moving the mouse. It can be translated along the z axis by using CTRL and the mouse wheel.

###License

Real3D. Author :  Antoine TOISOUL LE CANN. Copyright © 2016 Antoine TOISOUL LE CANN. All rights reserved.

Real3D is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. PFM_ReadWrite is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

###Known Bugs

* In Debug mode textures cannot be loaded due to incorrect OpenCV dlls
* The object is not at the same distance on the screen and on a screenshot
