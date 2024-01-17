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

#include "DataProcess.hpp"
#include "Utility/DebugText.hpp"

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
// STL
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <QTime>



// **********************************************//
// *** ProcessVolumeData *** //
// **********************************************//
ProcessVolumeData::ProcessVolumeData() {

}
ProcessVolumeData::~ProcessVolumeData() {

}

// ********************************************** //
// *** Rotate Volume *** //
// ********************************************** //

template <typename T>
bool ProcessVolumeData::__RotateAxis(T indicate, VolumeData& volumeData, int permute[3]) {

	// Dimensional permutation
	unsigned int dimM_origin[3] = { volumeData.xResolution, volumeData.yResolution, volumeData.zResolution };
	unsigned int dimM_new[3];
	double PixelSpace[3] = { volumeData.xPixelSpace, volumeData.yPixelSpace, volumeData.zPixelSpace};
	double PixelSpace_new[3];

	for (int i = 0; i < 3; i++) {
		dimM_new[i] = dimM_origin[permute[i]];
		PixelSpace_new[i] = PixelSpace[permute[i]];
	}

	unsigned int stride[3] = { 1, dimM_origin[0], dimM_origin[0] * dimM_origin[1] };
	T * data_aim = new T[dimM_origin[0] * dimM_origin[1] * dimM_origin[2]];
	if (!data_aim) {
		return false;
	}

	T* data_origin = static_cast<T*>(volumeData.data);

	for (unsigned int i = 0; i < dimM_new[0]; i++) {
		for (unsigned int j = 0; j < dimM_new[1]; j++) {
			for (unsigned int k = 0; k < dimM_new[2]; k++) {
				unsigned int offsetnew = i + j * dimM_new[0] + k * dimM_new[0] * dimM_new[1];

				unsigned int offset = i * stride[permute[0]] + j * stride[permute[1]] + k * stride[permute[2]];

				data_aim[offsetnew] = data_origin[offset];
			}
		}
	}

	delete[] static_cast<T*>(volumeData.data);

	volumeData.data = data_aim;

	volumeData.xResolution = dimM_new[0];
	volumeData.yResolution = dimM_new[1];
	volumeData.zResolution = dimM_new[2];

	volumeData.xPixelSpace = PixelSpace_new[0];
	volumeData.yPixelSpace = PixelSpace_new[1];
	volumeData.zPixelSpace = PixelSpace_new[2];

	return true;
}

void ProcessVolumeData::RotateAxisMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, int permute[3]) {
	// check input and output
	if (!data_rw.isInputMhdFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Mhd(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __RotateAxis(static_cast<unsigned long>(1), volumeData, permute);
			break;
		case Dez_SignedLong:
			parseFlag = __RotateAxis(static_cast<signed long>(1), volumeData, permute);
			break;
		case Dez_UnsignedShort:
			parseFlag = __RotateAxis(static_cast<unsigned short>(1), volumeData, permute);
			break;
		case Dez_SignedShort:
			parseFlag = __RotateAxis(static_cast<signed short>(1), volumeData, permute);
			break;
		case Dez_UnsignedChar:
			parseFlag = __RotateAxis(static_cast<unsigned char>(1), volumeData, permute);
			break;
		case Dez_SignedChar:
			parseFlag = __RotateAxis(static_cast<signed char>(1), volumeData, permute);
			break;
		case Dez_Float:
			parseFlag = __RotateAxis(static_cast<float>(1), volumeData, permute);
			break;
		case Dez_Double:
			parseFlag = __RotateAxis(static_cast<double>(1), volumeData, permute);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}
	
	if (parseFlag) {
		bool writeFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	else {
		DebugTextPrintErrorString("Rotate Axis failed");
	}
		

	volumeData.clear();
}

void ProcessVolumeData::RotateAxisFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, int permute[3]) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Feimos(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = parseFlag && __RotateAxis(static_cast<unsigned long>(1), volumeData, permute);
			break;
		case Dez_SignedLong:
			parseFlag = parseFlag && __RotateAxis(static_cast<signed long>(1), volumeData, permute);
			break;
		case Dez_UnsignedShort:
			parseFlag = parseFlag && __RotateAxis(static_cast<unsigned short>(1), volumeData, permute);
			break;
		case Dez_SignedShort:
			parseFlag = parseFlag && __RotateAxis(static_cast<signed short>(1), volumeData, permute);
			break;
		case Dez_UnsignedChar:
			parseFlag = parseFlag && __RotateAxis(static_cast<unsigned char>(1), volumeData, permute);
			break;
		case Dez_SignedChar:
			parseFlag = parseFlag && __RotateAxis(static_cast<signed char>(1), volumeData, permute);
			break;
		case Dez_Float:
			parseFlag = parseFlag && __RotateAxis(static_cast<float>(1), volumeData, permute);
			break;
		case Dez_Double:
			parseFlag = parseFlag && __RotateAxis(static_cast<double>(1), volumeData, permute);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}

	if (parseFlag) {
		bool writeFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	else {
		DebugTextPrintErrorString("Rotate Axis failed");
	}

	volumeData.clear();
}

// ********************************************** //
// *** Flip Volume *** //
// ********************************************** //

template <typename T>
bool ProcessVolumeData::__FlipAxis(T indicate, VolumeData& volumeData, int flip[3]) {
	unsigned int width = volumeData.xResolution, 
		height = volumeData.yResolution, 
		imageNum = volumeData.zResolution;

	T * data_m = static_cast<T*>(volumeData.data);
	if (!data_m) {
		return false;
	}

	if (flip[0] != 0) {
		for (unsigned int i = 0; i < width / 2; i++) {
			for (unsigned int j = 0; j < height; j++) {
				for (unsigned int k = 0; k < imageNum; k++) {
					T * p1 = (data_m) + i + j * width + k * width * height;
					T * p2 = (data_m) + (width - i - 1) + j * width + k * width * height;
					T a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[1] != 0) {
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height / 2; j++) {
				for (unsigned int k = 0; k < imageNum; k++) {
					T * p1 = (data_m) + i + j * width + k * width * height;
					T * p2 = (data_m) + i + (height - j - 1) * width + k * width * height;
					T a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[2] != 0) {
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				for (unsigned int k = 0; k < imageNum / 2; k++) {
					T * p1 = (data_m) + i + j * width + k * width * height;
					T * p2 = (data_m) + i + j * width + (imageNum - k - 1) * width * height;
					T a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}

	return true;
}

void ProcessVolumeData::FlipAxisMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, int flip[3]) {
	// check input and output
	if (!data_rw.isInputMhdFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Mhd(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __FlipAxis(static_cast<unsigned long>(1), volumeData, flip);
			break;
		case Dez_SignedLong:
			parseFlag = __FlipAxis(static_cast<signed long>(1), volumeData, flip);
			break;
		case Dez_UnsignedShort:
			parseFlag = __FlipAxis(static_cast<unsigned short>(1), volumeData, flip);
			break;
		case Dez_SignedShort:
			parseFlag = __FlipAxis(static_cast<signed short>(1), volumeData, flip);
			break;
		case Dez_UnsignedChar:
			parseFlag = __FlipAxis(static_cast<unsigned char>(1), volumeData, flip);
			break;
		case Dez_SignedChar:
			parseFlag = __FlipAxis(static_cast<signed char>(1), volumeData, flip);
			break;
		case Dez_Float:
			parseFlag = __FlipAxis(static_cast<float>(1), volumeData, flip);
			break;
		case Dez_Double:
			parseFlag = __FlipAxis(static_cast<double>(1), volumeData, flip);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}
	if (parseFlag)
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	else {
		DebugTextPrintErrorString("Flip Axis failed");
	}

	volumeData.clear();

}

void ProcessVolumeData::FlipAxisFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, int flip[3]) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Feimos(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __FlipAxis(static_cast<unsigned long>(1), volumeData, flip);
			break;
		case Dez_SignedLong:
			parseFlag = __FlipAxis(static_cast<signed long>(1), volumeData, flip);
			break;
		case Dez_UnsignedShort:
			parseFlag = __FlipAxis(static_cast<unsigned short>(1), volumeData, flip);
			break;
		case Dez_SignedShort:
			parseFlag = __FlipAxis(static_cast<signed short>(1), volumeData, flip);
			break;
		case Dez_UnsignedChar:
			parseFlag = __FlipAxis(static_cast<unsigned char>(1), volumeData, flip);
			break;
		case Dez_SignedChar:
			parseFlag = __FlipAxis(static_cast<signed char>(1), volumeData, flip);
			break;
		case Dez_Float:
			parseFlag = __FlipAxis(static_cast<float>(1), volumeData, flip);
			break;
		case Dez_Double:
			parseFlag = __FlipAxis(static_cast<double>(1), volumeData, flip);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}
	if (parseFlag)
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	else {
		DebugTextPrintErrorString("Flip Axis failed");
	}

	volumeData.clear();
}

// ********************************************** //
// *** Clip Volume *** //
// ********************************************** //

template <typename T>
bool ProcessVolumeData::__Clip(T indicate, VolumeData& volumeData, double begin[3], double end[3]) {
	int xRange[2] = { (int)(begin[0] * volumeData.xResolution), (int)(end[0] * volumeData.xResolution) };
	int yRange[2] = { (int)(begin[1] * volumeData.yResolution), (int)(end[1] * volumeData.yResolution) };
	int zRange[2] = { (int)(begin[2] * volumeData.zResolution), (int)(end[2] * volumeData.zResolution) };

	unsigned int dimOrigin[3] = { volumeData.xResolution, volumeData.yResolution, volumeData.zResolution };
	unsigned int dimClipped[3] = { xRange[1] - xRange[0], yRange[1] - yRange[0], zRange[1] - zRange[0] };
	
	T * data_clipped = new T[dimClipped[0] * dimClipped[1] * dimClipped[2]];
	if (!data_clipped) {
		return false;
	}
	T * data_src = static_cast<T*>(volumeData.data);

	for (unsigned int i = 0; i < dimClipped[0]; i++) {
		for (unsigned int j = 0; j < dimClipped[1]; j++) {
			for (unsigned int k = 0; k < dimClipped[2]; k++) {

				unsigned int pos_origin[3] = { i + xRange[0],j + yRange[0],k + zRange[0] };
				unsigned int pos_clipped[3] = { i,j,k };
				
				T dat = at_VolumeData(data_src, dimOrigin, pos_origin);
				set_VolumeData(data_clipped, dat, dimClipped, pos_clipped);
			}
		}
	}
	delete[] static_cast<T*>(volumeData.data);

	volumeData.data = data_clipped;

	volumeData.xResolution = dimClipped[0];
	volumeData.yResolution = dimClipped[1];
	volumeData.zResolution = dimClipped[2];

	return true;
}

void ProcessVolumeData::ClipMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, double begin[3], double end[3]) {

	// check input and output
	if (!data_rw.isInputMhdFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Mhd(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}

	if (parseFlag) {
		// Check if the number of image layers between intervals is less than 1
		if (end[0] * volumeData.xResolution - begin[0] * volumeData.xResolution < 1.0) parseFlag = false;
		if (end[1] * volumeData.yResolution - begin[1] * volumeData.yResolution < 1.0) parseFlag = false;
		if (end[2] * volumeData.zResolution - begin[2] * volumeData.zResolution < 1.0) parseFlag = false;

		if (!parseFlag) {
			DebugTextPrintErrorString("At least one layer of images must be included within the scope!");
		}
	}

	if (parseFlag) {
		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __Clip(static_cast<unsigned long>(1), volumeData, begin, end);
			break;
		case Dez_SignedLong:
			parseFlag = __Clip(static_cast<signed long>(1), volumeData, begin, end);
			break;
		case Dez_UnsignedShort:
			parseFlag = __Clip(static_cast<unsigned short>(1), volumeData, begin, end);
			break;
		case Dez_SignedShort:
			parseFlag = __Clip(static_cast<signed short>(1), volumeData, begin, end);
			break;
		case Dez_UnsignedChar:
			parseFlag = __Clip(static_cast<unsigned char>(1), volumeData, begin, end);
			break;
		case Dez_SignedChar:
			parseFlag = __Clip(static_cast<signed char>(1), volumeData, begin, end);
			break;
		case Dez_Float:
			parseFlag = __Clip(static_cast<float>(1), volumeData, begin, end);
			break;
		case Dez_Double:
			parseFlag = __Clip(static_cast<double>(1), volumeData, begin, end);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}
	if (parseFlag)
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	else {
		DebugTextPrintErrorString("Clip failed");
	}	

	volumeData.clear();
}

void ProcessVolumeData::ClipFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, double begin[3], double end[3]) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Feimos(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}

	if (parseFlag) {
		// Check if the number of image layers between intervals is less than 1
		if (end[0] * volumeData.xResolution -  begin[0] * volumeData.xResolution < 1.0) parseFlag = false;
		if (end[1] * volumeData.yResolution - begin[1] * volumeData.yResolution < 1.0) parseFlag = false;
		if (end[2] * volumeData.zResolution - begin[2] * volumeData.zResolution < 1.0) parseFlag = false;

		if (!parseFlag) {
			DebugTextPrintErrorString("At least one layer of images must be included within the scope!");
		}
	}

	if (parseFlag) {

		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __Clip(static_cast<unsigned long>(1), volumeData, begin, end);
			break;
		case Dez_SignedLong:
			parseFlag = __Clip(static_cast<signed long>(1), volumeData, begin, end);
			break;
		case Dez_UnsignedShort:
			parseFlag = __Clip(static_cast<unsigned short>(1), volumeData, begin, end);
			break;
		case Dez_SignedShort:
			parseFlag = __Clip(static_cast<signed short>(1), volumeData, begin, end);
			break;
		case Dez_UnsignedChar:
			parseFlag = __Clip(static_cast<unsigned char>(1), volumeData, begin, end);
			break;
		case Dez_SignedChar:
			parseFlag = __Clip(static_cast<signed char>(1), volumeData, begin, end);
			break;
		case Dez_Float:
			parseFlag = __Clip(static_cast<float>(1), volumeData, begin, end);
			break;
		case Dez_Double:
			parseFlag = __Clip(static_cast<double>(1), volumeData, begin, end);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}
	if (parseFlag)
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	else {
		DebugTextPrintErrorString("Clip failed");
	}

	volumeData.clear();
}

// **********************************************//
// *** Down sampling *** //
// **********************************************//

template <typename T>
bool ProcessVolumeData::__downSampling(T* data, VolumeData& volumeData, unsigned int dimM[3], unsigned int dim_Down[3], unsigned int Interval) {

	T * data_aim = new T[dim_Down[0] * dim_Down[1] * dim_Down[2]];
	if (!data_aim) return false;
	for (unsigned int i = 0; i < dim_Down[0]; i++) {
		for (unsigned int j = 0; j < dim_Down[1]; j++) {
			for (unsigned int k = 0; k < dim_Down[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i * Interval, j * Interval, k * Interval };
				T dat = at_VolumeData(data, dimM, pos_m);
				set_VolumeData(data_aim, dat, dim_Down, pos_aim);
			}
		}
	}

	delete[] static_cast<T*>(volumeData.data);

	volumeData.data = data_aim;

	volumeData.xResolution = dim_Down[0];
	volumeData.yResolution = dim_Down[1];
	volumeData.zResolution = dim_Down[2];

	volumeData.xPixelSpace = volumeData.xPixelSpace * Interval;
	volumeData.yPixelSpace = volumeData.yPixelSpace * Interval;
	volumeData.zPixelSpace = volumeData.zPixelSpace * Interval;
	return true;
}

void ProcessVolumeData::DownSamplingMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
	const GenerateFormat& generateFormat, int Interval) {
	// check input and output
	if (!data_rw.isInputMhdFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Mhd(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		unsigned int dimM[3] = { volumeData.xResolution, volumeData.yResolution, volumeData.zResolution };
		unsigned int dim_Down[3] = { volumeData.xResolution / Interval, volumeData.yResolution / Interval, volumeData.zResolution / Interval };

		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __downSampling(static_cast<unsigned long*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedLong:
			parseFlag = __downSampling(static_cast<signed long*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedShort:
			parseFlag = __downSampling(static_cast<unsigned short*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedShort:
			parseFlag = __downSampling(static_cast<signed short*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedChar:
			parseFlag = __downSampling(static_cast<unsigned char*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedChar:
			parseFlag = __downSampling(static_cast<signed char*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_Float:
			parseFlag = __downSampling(static_cast<float*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_Double:
			parseFlag = __downSampling(static_cast<double*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}

	if (parseFlag) 
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	else 
		DebugTextPrintErrorString("Downsampling failed");
	
	volumeData.clear();
}

void ProcessVolumeData::DownSamplingFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat, int Interval) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Feimos(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());
	if (!volumeData.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		unsigned int dimM[3] = { volumeData.xResolution, volumeData.yResolution, volumeData.zResolution };
		unsigned int dim_Down[3] = { volumeData.xResolution / Interval, volumeData.yResolution / Interval, volumeData.zResolution / Interval };

		switch (volumeData.format)
		{
		case Dez_Origin:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = __downSampling(static_cast<unsigned long*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedLong:
			parseFlag = __downSampling(static_cast<signed long*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedShort:
			parseFlag = __downSampling(static_cast<unsigned short*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedShort:
			parseFlag = __downSampling(static_cast<signed short*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedChar:
			parseFlag = __downSampling(static_cast<unsigned char*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_SignedChar:
			parseFlag = __downSampling(static_cast<signed char*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_Float:
			parseFlag = __downSampling(static_cast<float*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		case Dez_Double:
			parseFlag = __downSampling(static_cast<double*>(volumeData.data), volumeData, dimM, dim_Down, Interval);
			break;
		default:
			DebugTextPrintErrorString("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	else {
		DebugTextPrintErrorString("Downsampling failed");
	}

	volumeData.clear();
}

// **********************************************//
// *** Special functions that cannot be performed in steps *** //
// **********************************************//

template <typename T>
bool ProcessVolumeData::__DownSamplingInOneImage(
	const T* data, const unsigned int width, const unsigned int height,
	T* data_aim, const unsigned int resizedWidth, const unsigned int resizedHeight,
	const unsigned int Interval) {

	for (unsigned int i = 0; i < resizedWidth; i++) {
		for (unsigned int j = 0; j < resizedHeight; j++) {
			unsigned int off_data = (Interval * i) + (Interval * j) * width;
			unsigned int off_data_aim = (i) + (j) * resizedWidth;
			data_aim[off_data_aim] = data[off_data];
		}
	}
	return true;
}

bool ProcessVolumeData::DownSamplingLargeFeimosData(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	unsigned int Interval) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return false;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return false;

	if (Interval <= 0) {
		DebugTextPrintErrorString("Illegal value of Interval");
		return false;
	}

	std::ifstream file(InputFilePath.toStdString());
	std::string name;
	std::string rawDataFileName;

	VolumeData volumeData;

	while (file >> name) {

		if (name == "xResolution") {
			file >> volumeData.xResolution;
		}
		else if (name == "yResolution") {
			file >> volumeData.yResolution;
		}
		else if (name == "zResolution") {
			file >> volumeData.zResolution;
		}
		else if (name == "xPixelSpace") {
			file >> volumeData.xPixelSpace;
		}
		else if (name == "yPixelSpace") {
			file >> volumeData.yPixelSpace;
		}
		else if (name == "zPixelSpace") {
			file >> volumeData.zPixelSpace;
		}
		else if (name == "format") {
			file >> name;
			if (!volumeData.setFormatUsingString(name.c_str())) {
				DebugTextPrintErrorString("Non compliant format input when parsing (.feimos) file!");
				return false;
			}
		}
		else if (name == "Raw") {
			std::getline(file, name);
			name.erase(name.begin());
			rawDataFileName = name;
		}
	}

	QFileInfo fileInfo(InputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	if (!data_rw.isInputFileExist(absoluteDirPath + "/" + rawDataFileName.c_str())) {
		DebugTextPrintErrorString("Raw data does not exist when parsing (.feimos) file!");
		return false;
	}

	unsigned int 
		resizedWidth = volumeData.xResolution / Interval, 
		resizedHeight = volumeData.yResolution / Interval, 
		resizedImageNum = volumeData.zResolution / Interval;

	double
		resizedPixelSpace_x = volumeData.xPixelSpace * Interval,
		resizedPixelSpace_y = volumeData.yPixelSpace * Interval,
		resizedPixelSpace_z = volumeData.zPixelSpace * Interval;

	int bytesOnePixel = volumeData.getFormatOnePixelBytes();
	if (bytesOnePixel <= 0) {
		DebugTextPrintErrorString("Bytes per pixel that do not comply!");
		return false;
	}

	// down sampling
	bool downsamplingFlag = true;
	{
		std::ifstream inputFile((absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), std::ios::binary);
		std::ofstream outputFile((OutputDir + "/" + OutputFileName + ".raw").toStdString(), std::ios::binary);
		if (!inputFile.is_open() || !outputFile.is_open()) {
			DebugTextPrintErrorString("Cannot open raw input and output file!");
			return false;
		}

		const std::streamsize readImageSize = volumeData.xResolution * volumeData.yResolution * (unsigned int)bytesOnePixel;
		void* readBuffer = new char[readImageSize];

		const std::streamsize writeImageSize = resizedWidth * resizedHeight * bytesOnePixel;
		void* writeBuffer = new char[writeImageSize];

		unsigned int currentNum = 0;
		unsigned int writeImageNum = 0;
		
		while (inputFile.read(static_cast<char*>(readBuffer), readImageSize)) {

			if (0 == currentNum % Interval) {

				switch (volumeData.format)
				{
				case Dez_Origin:
					downsamplingFlag = false;
					break;
				case Dez_UnsignedLong:
					__DownSamplingInOneImage(
						static_cast<unsigned long*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<unsigned long*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedLong:
					__DownSamplingInOneImage(
						static_cast<signed long*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<signed long*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_UnsignedShort:
					__DownSamplingInOneImage(
						static_cast<unsigned short*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<unsigned short*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedShort:
					__DownSamplingInOneImage(
						static_cast<signed short*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<signed short*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_UnsignedChar:
					__DownSamplingInOneImage(
						static_cast<unsigned char*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<unsigned char*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedChar:
					__DownSamplingInOneImage(
						static_cast<char*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<char*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_Float:
					__DownSamplingInOneImage(
						static_cast<float*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<float*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_Double:
					__DownSamplingInOneImage(
						static_cast<double*>(readBuffer), volumeData.xResolution, volumeData.yResolution,
						static_cast<double*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				default:
					downsamplingFlag = false;
					break;
				}

				writeImageNum++;
				outputFile.write(static_cast<char*>(writeBuffer), writeImageSize);
			}
			currentNum++;
		}

		if (writeImageNum > resizedImageNum + 1 || writeImageNum < resizedImageNum-1) {
			DebugTextPrintErrorString(("The amount of data does not match the file description! resized required:[" + 
				std::to_string(resizedImageNum) + "], now[" + 
				std::to_string(writeImageNum) + "]").c_str());
			downsamplingFlag = false;
		}

		resizedImageNum = writeImageNum;

		delete [] writeBuffer;
		delete [] readBuffer;
	}

	if (downsamplingFlag)
	{
		std::ofstream fileInfo((OutputDir + "/" + OutputFileName + ".feimos").toStdString());
		if (!fileInfo.is_open()) {
			DebugTextPrintErrorString("Fail to write to info file");
			return false;
		}
		std::string format = volumeData.getFormatString().toStdString();
		fileInfo << "xResolution " << resizedWidth << std::endl;
		fileInfo << "yResolution " << resizedHeight << std::endl;
		fileInfo << "zResolution " << resizedImageNum << std::endl;
		fileInfo << "xPixelSpace " << resizedPixelSpace_x << std::endl;
		fileInfo << "yPixelSpace " << resizedPixelSpace_y << std::endl;
		fileInfo << "zPixelSpace " << resizedPixelSpace_z << std::endl;
		fileInfo << "format " << format << std::endl;
		fileInfo << "Raw " << (OutputFileName.toStdString() + ".raw") << std::endl;
		fileInfo.close();
	}

	return downsamplingFlag;
}


// **********************************************//
// *** Resize Volume *** //
// **********************************************//

bool ProcessVolumeData::Resize(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	float scale) {
	return false;
}

// **********************************************//
// *** Call type function *** //
// **********************************************//

void ProcessVolumeData::DcmMakeMhdFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {

	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputDcmFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;

	if (generateFormat.parseLib == Dez_GDCM) {
		parseFlag = data_rw.GenerateInput_GDCM(fileList, volumeData);
	}
	else if (generateFormat.parseLib == Dez_DCMTK) {
		parseFlag = data_rw.GenerateInput_DCMTK(fileList, volumeData);
	}

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::DcmMakeFeimosFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {

	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputDcmFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;

	if (generateFormat.parseLib == Dez_GDCM) {
		parseFlag = data_rw.GenerateInput_GDCM(fileList, volumeData);
	}
	else if (generateFormat.parseLib == Dez_DCMTK) {
		parseFlag = data_rw.GenerateInput_DCMTK(fileList, volumeData);
	}

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::PngsMakeFeimosFile(
	const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputPngsFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;

	parseFlag = data_rw.GenerateInput_PNGs(fileList, volumeData);

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::PngsMakeMhdFile(
	const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputPngsFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;
	parseFlag = data_rw.GenerateInput_PNGs(fileList, volumeData);

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::JpgsMakeFeimosFile(
	const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputJpgsFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;

	parseFlag = data_rw.GenerateInput_JPGs(fileList, volumeData);

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::JpgsMakeMhdFile(
	const QString& inputDir, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputDirExist(inputDir)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	std::vector<QString> fileList;
	bool parseFlag = data_rw.getInputJpgsFileList(inputDir, fileList);
	if (!parseFlag) return;

	VolumeData volumeData;
	parseFlag = data_rw.GenerateInput_JPGs(fileList, volumeData);

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::MhdMakeFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputMhdFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Feimos(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Mhd(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Feimos(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::FeimosMakeMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!data_rw.isInputFeimosFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;

	bool parseFlag = data_rw.GenerateInput_Feimos(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}

void ProcessVolumeData::PbrtMakeMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
	const GenerateFormat& generateFormat) {

	// check input and output
	if (!data_rw.isInputPbrtFileExist(InputFilePath)) return;
	if (!data_rw.checkOutputDir_Mhd(OutputDir, OutputFileName)) return;

	VolumeData volumeData;
	bool parseFlag = data_rw.GenerateInput_Pbrt(InputFilePath, volumeData);
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	if (parseFlag) {
		parseFlag = data_rw.GenerateOutput_Mhd(OutputDir, OutputFileName, generateFormat, volumeData);
	}
	volumeData.clear();
}


// **********************************************//
// *** Old Functions *** //
// **********************************************//

struct DcmFile {
	Uint16 *pixData16 = nullptr;
	float position = 0.0f;
};
bool DataUpSort(const DcmFile &f1, const DcmFile &f2) {
	if (f1.position < f2.position) return true;
	else return false;
}
void ProcessVolumeData::DcmMakeMhdFile_DCMTK_OldFunc(const QString& dirPath, const QString& OutputDir, const QString& OutputFileName) {
	DebugTextPrintString("Convert .dcm files into (.mhd)-(.raw) format using DCMTK lib.");

	QDir dir(dirPath);
	unsigned int file_count;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			DebugTextPrintErrorString("Dir does not exist!");
			return;
		}

		QStringList filters;
		filters << "*.dcm";
		//Set type filters only for file formats, do not use symbolic links
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		//Set file name filters, filters format
		dir.setNameFilters(filters);
		//Number of record files
		file_count = dir.count();
		if (file_count <= 1) {
			DebugTextPrintErrorString("file count less than 2");
			return;
		}
		DebugTextPrintNum("file count", file_count);
		_DirPath = dirPath;
		if (dirPath[dirPath.size() - 1] != '/') _DirPath = _DirPath + "/";
	}

	VolumeData volumeData;
	unsigned int width = 0, height = 0;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	Uint16 bitsAllocated, bitsStored;
	// valid 
	std::vector<unsigned int> correctImagesNum;
	for (unsigned int i = 0; i < file_count; i++)
	{

		QString file_Path = _DirPath + dir[i];
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();

		if (!Metalnfo) {
			DebugTextPrintErrorString(("No Metalnfo in File " + file_Path).toStdString().c_str());
			continue;
		}

		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			DebugTextPrintErrorString(("No data in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Pixel Data
		DcmElement *element = NULL;
		if (!data->findAndGetElement(DCM_PixelData, element).good()) {
			DebugTextPrintErrorString(("Get PixelData wrong in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Width Info
		{
			OFString ImageWidth;
			if (!data->findAndGetOFString(DCM_Columns, ImageWidth).good()) {
				DebugTextPrintErrorString(("Get DCM_Columns wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && width != static_cast<unsigned int>(atoi(ImageWidth.data()))) {
				DebugTextPrintErrorString(("The width of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				width = static_cast<unsigned int>(atoi(ImageWidth.data()));
				volumeData.xResolution = width;
			}
		}

		// Get Height Info
		{
			OFString ImageHeight;
			if (!data->findAndGetOFString(DCM_Rows, ImageHeight).good()) {
				DebugTextPrintErrorString(("Get DCM_Rows wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && height != static_cast<unsigned int>(atoi(ImageHeight.data()))) {
				DebugTextPrintErrorString(("The height of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				height = static_cast<unsigned int>(atoi(ImageHeight.data()));
				volumeData.yResolution = height;
			}
		}

		// Get Pixel Spacing
		{
			OFString PixelSpacing;
			if (!data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing).good()) {
				DebugTextPrintErrorString(("Get DCM_PixelSpacing wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && pixelSpacing_X != atof(PixelSpacing.data())) {
				DebugTextPrintErrorString(("The pixelSpacing of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_X = atof(PixelSpacing.data());
				pixelSpacing_Y = pixelSpacing_X;
				volumeData.xResolution = pixelSpacing_X;
				volumeData.yResolution = pixelSpacing_Y;
			}
		}

		// Get Slice Thickness
		{
			OFString SliceThickness;
			if (!data->findAndGetOFString(DCM_SliceThickness, SliceThickness).good()) {
				DebugTextPrintErrorString(("Get DCM_SliceThickness wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (i != 0 && pixelSpacing_Z != atof(SliceThickness.data())) {
				DebugTextPrintErrorString(("The SliceThickness of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_Z = atof(SliceThickness.data());
				volumeData.zResolution = pixelSpacing_Z;
			}
		}

		// Get Bit Allocated
		{
			Uint16 bitsAllocated_temp, bitsStored_temp;
			if (correctImagesNum.size() == 0) {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored).good()) {
					DebugTextPrintNum("bitsAllocated_temp", bitsAllocated);
					DebugTextPrintNum("bitsStored_temp", bitsStored);
				}
				else {
					DebugTextPrintErrorString(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated_temp).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored_temp).good()) {
					if (bitsAllocated_temp != bitsAllocated || bitsStored_temp != bitsStored) {
						DebugTextPrintErrorString(("The bits allocated or stored is inconsistent in File" + file_Path).toStdString().c_str());
						continue;
					}
				}
				else {
					DebugTextPrintErrorString(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
					continue;
				}
			}
		}

		// Get Data Format
		{
			// Pixel Representation
			DcmTagKey tagKey(0x0028, 0x0100);
			DcmElement* elementPR;
			if (data->findAndGetElement(tagKey, elementPR).good()) {
				// Obtaining Value Representation of Data Elements (VR)
				DcmEVR valueRepresentation = elementPR->getVR();

				// Print Value Representation (VR) to Console
				std::cout << "Value Representation: " << DcmVR(valueRepresentation).getVRName() << std::endl;

				// Float
				if (valueRepresentation == EVR_FL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_Float;
					else if (volumeData.format != Dez_Float) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// Double
				else if (valueRepresentation == EVR_FD) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_Double;
					else if (volumeData.format != Dez_Double) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// signed short
				else if (valueRepresentation == EVR_SS) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_SignedShort;
					else if (volumeData.format != Dez_SignedShort) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// unsigned short
				else if (valueRepresentation == EVR_US) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_UnsignedShort;
					else if (volumeData.format != Dez_UnsignedShort) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// signed long
				else if (valueRepresentation == EVR_SL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_SignedLong;
					else if (volumeData.format != Dez_SignedLong) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// unsigned long
				else if (valueRepresentation == EVR_UL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_UnsignedLong;
					else if (volumeData.format != Dez_UnsignedLong) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				else {
					DebugTextPrintErrorString(("Temporarily unsupported data format " + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				DebugTextPrintErrorString(("Get Pixel Representation wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
		}

		correctImagesNum.push_back(i);
	}

	unsigned int imageNum = correctImagesNum.size();

	// Print import data Info
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	Uint16 * m_data = new Uint16[width * height * imageNum];
	if (!m_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return;
	}

	// Start processing each image
	std::vector<DcmFile> dcmFileVec;
	for (unsigned int i = 0; i < imageNum; i++) {
		// File Path
		QString file_Path = _DirPath + dir[correctImagesNum[i]];

		// DcmFileFormat will automatically release memory
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
		if (!Metalnfo) {
			DebugTextPrintErrorString(("No Metalnfo in File " + file_Path).toStdString().c_str());
			break;
		}
		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			DebugTextPrintErrorString(("No data in File " + file_Path).toStdString().c_str());
			continue;
		}
		DcmElement *element = NULL;
		data->findAndGetElement(DCM_PixelData, element);

		OFString ImageWidth;
		data->findAndGetOFString(DCM_Columns, ImageWidth);
		width = atoi(ImageWidth.data());
		OFString ImageHeight;
		data->findAndGetOFString(DCM_Rows, ImageHeight);
		height = atoi(ImageHeight.data());
		OFString ImagePos;
		data->findAndGetOFString(DCM_SliceLocation, ImagePos);

		Uint16* dat_t;
		element->getUint16Array(dat_t);
		memcpy(m_data + i * width * height, dat_t, width * height * sizeof(Uint16));

		DcmFile dcmF;
		dcmF.pixData16 = m_data + i * width * height;
		dcmF.position = atof(ImagePos.data());
		dcmFileVec.push_back(dcmF);
	}

	// Sort by image position
	std::sort(dcmFileVec.begin(), dcmFileVec.end(), DataUpSort);

	Uint16 * aim_data = new Uint16[width * height * imageNum];
	if (!aim_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return;
	}
	bool flag = true;
	for (unsigned int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			DebugTextPrintErrorString("(dcmFileVec[i].pixData16 == nullptr) ");
			flag = false;
		}
	}
	if (flag) {

	}

	for (unsigned int i = 0; i < imageNum; i++) {
		memcpy((Uint16*)aim_data + i * width * height, dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, width - 1, 0, height - 1, 0, imageNum - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1);
	imageImport->SetImportVoidPointer(aim_data);
	imageImport->Update();

	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();
	DebugTextPrintNum("Write Finished! ", 0);
	delete[] m_data;
	delete[] aim_data;
}
bool compareInstance(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2)
{
	// Study Instance UID
	gdcm::Attribute<0x0020, 0x000d> at1;
	at1.Set(ds1);
	gdcm::Attribute<0x0020, 0x000d> at2;
	at2.Set(ds2);
	return at1 < at2;
}
void ProcessVolumeData::DcmMakeMhdFile_GDCM_OldFunc(const QString& dirPath, const QString& OutputDir, const QString& OutputFileName) {
	DebugTextPrintString("Convert .dcm files into (.mhd)-(.raw) format using GDCM lib.");
	QDir dir(dirPath);
	if (!dir.exists()) {
		DebugTextPrintErrorString("Dir does not exist!");
		return;
	}

	QStringList filters;
	filters << "*.dcm";
	//Set type filters only for file formats, do not use symbolic links
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	//Set file name filters, filters format
	dir.setNameFilters(filters);
	//Number of record files
	int file_count = dir.count();
	if (file_count <= 1) {
		DebugTextPrintErrorString("file count less than 2");
		return;
	}
	DebugTextPrintNum("file_count : ", file_count);

	QString _DirPath = dirPath;
	if (dirPath[dirPath.size() - 1] != '/') _DirPath = _DirPath + "/";


	std::vector<DcmFile> dcmFileVec;
	int width, height, imageNum = file_count;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	for (int i = 0; i < file_count; i++) {
		gdcm::Reader reader;

		QString file_Path = _DirPath + dir[i];
		reader.SetFileName(file_Path.toStdString().c_str());
		if (!reader.Read()) continue;
		// The output of gdcm::Reader is a gdcm::File
		gdcm::File &file = reader.GetFile();
		// the dataset is the the set of element we are interested in:
		gdcm::DataSet &ds = file.GetDataSet();

		const gdcm::Global& g = gdcm::Global::GetInstance();
		const gdcm::Dicts &dicts = g.GetDicts();
		const gdcm::Dict &pubdict = dicts.GetPublicDict();

		// Image Position
		gdcm::Attribute<0x0020, 0x1041> at_position;
		at_position.SetFromDataSet(ds);
		gdcm::Attribute<0x0020, 0x1041>::ArrayType v_position = at_position.GetValue();

		// Rows
		gdcm::Attribute<0x0028, 0x0010> at_rows;
		at_rows.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0010>::ArrayType v_rows = at_rows.GetValue();
		height = v_rows;

		// Columns
		gdcm::Attribute<0x0028, 0x0011> at_columns;
		at_columns.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0011>::ArrayType v_columns = at_columns.GetValue();
		width = v_columns;

		// Spacing between slices
		gdcm::Attribute<0x0018, 0x0050> at_Spacing_slices;
		at_Spacing_slices.SetFromDataSet(ds);
		gdcm::Attribute<0x0018, 0x0050>::ArrayType v_Spacing_slices = at_Spacing_slices.GetValue();
		pixelSpacing_Z = v_Spacing_slices;

		// Spacing between slices  Unit:mm
		gdcm::Attribute<0x0028, 0x0030> at_Pixel_Spacing;
		at_Pixel_Spacing.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0030>::ArrayType vv_Pixel_Spacing = at_Pixel_Spacing.GetValue();
		pixelSpacing_X = pixelSpacing_Y = vv_Pixel_Spacing;

		// bits per pixel
		gdcm::Attribute<0x0028, 0x0100> at_bit;
		at_bit.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0100>::ArrayType v_bit = at_bit.GetValue();

		DcmFile dcmfile1;
		dcmfile1.position = v_position;
		dcmFileVec.push_back(dcmfile1);

	}

	Uint16 * m_data = new Uint16[width * height * imageNum];
	if (!m_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return;
	}
	for (int i = 0; i < file_count; i++) {
		gdcm::Reader reader;

		QString file_Path = _DirPath + dir[i];

		gdcm::ImageReader ir;
		ir.SetFileName(file_Path.toStdString().c_str());
		if (!ir.Read())
		{
			//Read failed
			continue;
		}
		const gdcm::Image &gimage = ir.GetImage();

		std::vector<char> vbuffer;
		vbuffer.resize(gimage.GetBufferLength());
		char *buffer = &vbuffer[0];

		{
			// 512 * 512 * 2 = 524288
			//std::cout << "gimage.GetBufferLength() = " << gimage.GetBufferLength() << std::endl;
		}

		gimage.GetBuffer(buffer);


		memcpy(m_data + i * width * height, buffer, width * height * sizeof(Uint16));
		dcmFileVec[i].pixData16 = m_data + i * width * height;
	}

	std::sort(dcmFileVec.begin(), dcmFileVec.end(), DataUpSort);

	Uint16 * aim_data = new Uint16[width * height * imageNum];
	if (!aim_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return;
	}
	bool flag = false;
	for (int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			DebugTextPrintErrorString("(dcmFileVec[i].pixData16 == nullptr) ");
			flag = true;
		}
	}

	for (int i = 0; i < imageNum; i++) {
		memcpy((Uint16*)aim_data + i * width * height, dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
	}
	delete[] m_data;

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, width - 1, 0, height - 1, 0, imageNum - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(aim_data);
	imageImport->Update();

	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();
	DebugTextPrintNum("Write Finished! ", 0);
	delete[] aim_data;
}
void ProcessVolumeData::MhdRotateAxis_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int permute[3]) {
	DebugTextPrintString("Permute axis of (.mhd)-(.raw) file.");
	if (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == permute[1]) {
		DebugTextPrintErrorString("Error in MHDRotateAxis: (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == flip[1])");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (permute[i] > 2 || permute[i] < 0) {
			DebugTextPrintErrorString("Error in MHDRotateAxis: (permute[i] > 2 || permute[i] < 0)");
			return;
		}
	}

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace[3];
	PixelSpace[0] = spacing[0];
	PixelSpace[1] = spacing[1];
	PixelSpace[2] = spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	// Dimensional permutation
	int dimM_new[3];
	double PixelSpace_new[3];
	for (int i = 0; i < 3; i++) {
		dimM_new[i] = dimM[permute[i]];
		PixelSpace_new[i] = PixelSpace[permute[i]];
	}
	int stride[3] = { 1, width, width * height };
	short * data_aim = new short[width * height * imageNum];
	for (int i = 0; i < dimM_new[0]; i++) {
		for (int j = 0; j < dimM_new[1]; j++) {
			for (int k = 0; k < dimM_new[2]; k++) {
				int offsetnew = i + j * dimM_new[0] + k * dimM_new[0] * dimM_new[1];

				int offset = i * stride[permute[0]] + j * stride[permute[1]] + k * stride[permute[2]];

				data_aim[offsetnew] = data_m[offset];
			}
		}
	}


	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_new[0], PixelSpace_new[1], PixelSpace_new[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM_new[0] - 1, 0, dimM_new[1] - 1, 0, dimM_new[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();
	DebugTextPrintNum("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;
}
void ProcessVolumeData::MhdFlipAxis_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int flip[3]) {
	DebugTextPrintString("Flip axis of (.mhd)-(.raw) file.");
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace[3];
	PixelSpace[0] = spacing[0];
	PixelSpace[1] = spacing[1];
	PixelSpace[2] = spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	if (flip[0] != 0) {
		for (int i = 0; i < width / 2; i++) {
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < imageNum; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + width - i - 1 + j * width + k * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[1] != 0) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height / 2; j++) {
				for (int k = 0; k < imageNum; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + i + (height - j - 1) * width + k * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[2] != 0) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < imageNum / 2; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + i + j * width + (imageNum - k - 1) * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace[0], PixelSpace[1], PixelSpace[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM[0] - 1, 0, dimM[1] - 1, 0, dimM[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(data_m);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();
	DebugTextPrintNum("Write Finished! ", 0);
	delete[] data_m;
}
void ProcessVolumeData::MhdClip_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, double center[3], double bound[3]) {
	DebugTextPrintString("Clip (.mhd)-(.raw) file.");
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	unsigned int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	int xRange[2] = { (int)((center[0] - bound[0] + 0.5) * width), (int)((center[0] + bound[0] + 0.5) * width) };
	int yRange[2] = { (int)((center[1] - bound[1] + 0.5) * height), (int)((center[1] + bound[1] + 0.5) * height) };
	int zRange[2] = { (int)((center[2] - bound[2] + 0.5) * imageNum), (int)((center[2] + bound[2] + 0.5) * imageNum) };
	unsigned int dimLength[3] = { xRange[1] - xRange[0], yRange[1] - yRange[0], zRange[1] - zRange[0] };
	short * data_aim = new short[dimLength[0] * dimLength[1] * dimLength[2]];
	for (unsigned int i = 0; i < dimLength[0]; i++) {
		for (unsigned int j = 0; j < dimLength[1]; j++) {
			for (unsigned int k = 0; k < dimLength[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i + xRange[0],j + yRange[0],k + zRange[0] };
				short dat = at_VolumeData(data_m, dimM, pos_m);
				set_VolumeData(data_aim, dat, dimLength, pos_aim);
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_x, PixelSpace_y, PixelSpace_z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimLength[0] - 1, 0, dimLength[1] - 1, 0, dimLength[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();

	DebugTextPrintNum("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;
}
void ProcessVolumeData::DownSamplingMhdWithInterval_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int Interval) {
	DebugTextPrintString("Resize (.mhd)-(.raw) file.");

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	unsigned int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	unsigned int dimLength[3] = { width / Interval, height / Interval, imageNum / Interval };
	short * data_aim = new short[dimLength[0] * dimLength[1] * dimLength[2]];
	for (unsigned int i = 0; i < dimLength[0]; i++) {
		for (unsigned int j = 0; j < dimLength[1]; j++) {
			for (unsigned int k = 0; k < dimLength[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i * Interval, j * Interval, k * Interval };
				short dat = at_VolumeData(data_m, dimM, pos_m);
				set_VolumeData(data_aim, dat, dimLength, pos_aim);
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_x, PixelSpace_y, PixelSpace_z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimLength[0] - 1, 0, dimLength[1] - 1, 0, dimLength[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((OutputDir + "/" + OutputFileName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((OutputDir + "/" + OutputFileName + ".raw").toStdString().c_str());
	writer->Write();
	DebugTextPrintNum("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;


}
void ProcessVolumeData::MhdGenerateFeimosData_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName,
	const GenerateFormat& generateFormat) {
	DebugTextPrintString("Convert (.mhd)-(.raw) file into Feimos file format.");
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	std::ofstream fileInfo((OutputDir + "/" + OutputFileName + ".Info" ).toStdString());
	{
		fileInfo << "X_Length " << width << std::endl;
		fileInfo << "Y_Length " << height << std::endl;
		fileInfo << "Z_Length " << imageNum << std::endl;
		fileInfo << "X_Space " << PixelSpace_x << std::endl;
		fileInfo << "Y_Space " << PixelSpace_y << std::endl;
		fileInfo << "Z_Space " << PixelSpace_z << std::endl;
	}
	fileInfo.close();

	std::ofstream file((OutputDir + "/" + OutputFileName + ".data").toStdString(), std::ios::binary);
	for (int i = 0; i < imageNum; i++) {
		file.write((char*)((short *)data_m + i * width * height), sizeof(short) * width * height);
	}
	file.close();

	// Previously written code
	{
		//std::ofstream file("sample.raw", std::ios::binary);
		//for (int i = 0; i < imageNum; i++) file.write((char*)dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
		//file.close();
		//FILE *fp = fopen("./sample.raw", "wb");
		//for (int i = 0; i < imageNum; i++) fwrite(dcmFileVec[i].pixData16, sizeof(Uint16), width * height, fp);
		//fclose(fp);
	}
	// Previously written code
	{
		//std::ofstream file("sample.raw", std::ios::binary);
		//file.write((char*)aim_data, width * height * imageNum * sizeof(Uint16));
		//file.close();
	}

	DebugTextPrintNum("Write Finished! ", 0);
	delete[] data_m;

}
void ProcessVolumeData::MhdGeneratePbrtVolume_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName) {

	DebugTextPrintErrorString("This function has not been implemented yet.");
}

// **********************************************//
// *** Some simple test code *** //
// **********************************************//
void ProcessVolumeData::DCMTK_Test_OldFunc() {

	/*DcmFileFormat dfile;
	dfile.loadFile("Images/aa1.dcm");
	DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
	DcmTag Tag;
	Tag = Metalnfo->getTag();
	Uint16 G_tag = Tag.getGTag();
	DebugTextPrintNum("G_tag: ", G_tag);
	DcmDataset *data = dfile.getDataset();

	DcmElement *element = NULL;
	data->findAndGetElement(DCM_PixelData, element);

	double data_len = data->getLength();
	double element_len = element->getLength();
	DebugTextPrintNum("data_len: ", data_len);
	DebugTextPrintNum("element_len: ", element_len);


	OFString patientName;
	data->findAndGetOFString(DCM_PatientName, patientName);
	DebugTextPrintString("patientName: " + QString(patientName.data()));

	OFString patientId;
	data->findAndGetOFString(DCM_PatientID, patientId);
	DebugTextPrintString("patientId: " + QString(patientId.data()));

	OFString patientAge;
	data->findAndGetOFString(DCM_PatientAge, patientAge);
	DebugTextPrintString("patientAge: " + QString(patientAge.data()));

	OFString PatientPosition;
	data->findAndGetOFString(DCM_PatientPosition, PatientPosition);
	DebugTextPrintString("PatientPosition: " + QString(PatientPosition.data()));

	OFString ImagePositionPatient;
	data->findAndGetOFString(DCM_ImagePositionPatient, ImagePositionPatient);

	OFString PixelSpacing;
	data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing);
	DebugTextPrintString("PixelSpacing: " + QString(PixelSpacing.data()));

	OFString ImageWidth;
	data->findAndGetOFString(DCM_Columns, ImageWidth);
	DebugTextPrintString("ImageWidth: " + QString(ImageWidth.data()));
	OFString ImageHeight;
	data->findAndGetOFString(DCM_Rows, ImageHeight);
	DebugTextPrintString("ImageHeight: " + QString(ImageHeight.data()));

	// image data
	Uint16 *pixData16;
	element->getUint16Array(pixData16);*/

}
void ProcessVolumeData::GDCM_Test_OldFunc() {
	/*
	gdcm::Reader reader;
	reader.SetFileName("Images/IM-0001-0001.dcm");
	if (!reader.Read())
	{
		DebugTextPrintNum("(!reader.Read()) : ", 1);
		return;
	}
	else {
		DebugTextPrintNum("(success reader.Read()) : ", 0);
	}
	*/


}






