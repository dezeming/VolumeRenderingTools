/*
	Copyright (C) <2023>  <Dezeming>  <feimos@mail.ustc.edu.cn>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or any
	later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	Github site: <https://github.com/dezeming/VolumeRenderingTools.git>
*/

#include "RenderThread.h"
#include "MainGUI/DebugText.hpp"

// DCMTK
#include "dcmdata/dcfilefo.h"
#include "config/osconfig.h"
#include "dcmdata/dctk.h"
#include "dcmimage/diargimg.h"

// GDCM
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmTesting.h"
#include "gdcmGlobal.h"
#include "gdcmDicts.h"
#include "gdcmDict.h"
#include "gdcmStringFilter.h"
#include "gdcmImageReader.h"

// VTK
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>
#include <vtkDataArray.h>
#include <vtkImageImport.h>

// Qt
#include <QDir>
#include <QStringList>
#include <QTime>
// STL
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

// **********************************************//
// *** RenderThread *** //
// **********************************************//

RenderThread::RenderThread() {
	paintFlag = false;
	renderFlag = false;
}

RenderThread::~RenderThread() {
	
}


void RenderThread::run() {

}















