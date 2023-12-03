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

#include "DataReaderAndWriter.hpp"
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
DataReaderAndWriter::DataReaderAndWriter() {

}
DataReaderAndWriter::~DataReaderAndWriter() {

}

// **********************************************//
// *** Check input/output legitimacy *** //
// **********************************************//

bool DataReaderAndWriter::isInputDirExist(const QString& inputDir) {
	QDir dir;
	if (!dir.exists(inputDir)) {
		DebugTextPrintErrorString("Input Folder does not exist.");
		return false;
	}
	return true;
}
bool DataReaderAndWriter::isInputFileExist(const QString& inputFilePath, const QString& suffix) {
	if (suffix != "") {
		if (!inputFilePath.endsWith(suffix, Qt::CaseInsensitive)) {
			DebugTextPrintErrorString("Suffix does not meet the requirements for input files.");
			return false;
		}
	}
	QFile file_input(inputFilePath);
	if (!file_input.exists()) {
		DebugTextPrintErrorString("Input file does not exist.");
		return false;
	}
	return true;
}
bool DataReaderAndWriter::isInputMhdFileExist(const QString& inputFilePath) {
	return isInputFileExist(inputFilePath, ".mhd");
}
bool DataReaderAndWriter::isInputFeimosFileExist(const QString& inputFilePath) {
	return isInputFileExist(inputFilePath, ".feimos");
}
bool DataReaderAndWriter::checkOutputDir_Mhd(const QString& outputDir, const QString& OutputFileName) {
	QDir dir;
	if (!dir.exists(outputDir)) {
		if (!dir.mkpath(outputDir)) {
			DebugTextPrintErrorString("Folder creation failed.");
			return false;
		}
	}

	QFile file_mhd(outputDir + "/" + OutputFileName + ".mhd");
	if (file_mhd.exists()) {
		DebugTextPrintErrorString("It is mandatory not to overwrite file with the same name.");
		return false;
	}
	QFile file_raw(outputDir + "/" + OutputFileName + ".raw");
	if (file_raw.exists()) {
		DebugTextPrintErrorString("It is mandatory not to overwrite file with the same name.");
		return false;
	}

	return true;
}
bool DataReaderAndWriter::checkOutputDir_Feimos(const QString& outputDir, const QString& outName) {
	QDir dir;
	if (!dir.exists(outputDir)) {
		if (!dir.mkpath(outputDir)) {
			DebugTextPrintErrorString("Folder creation failed.");
			return false;
		}
	}

	QFile file_feimos(outputDir + "/" + outName + ".feimos");
	if (file_feimos.exists()) {
		DebugTextPrintErrorString("It is mandatory not to overwrite file with the same name.");
		return false;
	}
	QFile file_raw(outputDir + "/" + outName + ".raw");
	if (file_raw.exists()) {
		DebugTextPrintErrorString("It is mandatory not to overwrite file with the same name.");
		return false;
	}

	return true;
}
bool DataReaderAndWriter::getInputDcmFileList(const QString& inputDir, std::vector<QString>& fileList) {
	QDir dir(inputDir);
	unsigned int file_count = 0;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			DebugTextPrintErrorString("Fatal: Dir does not exist!");
			return false;
		}

		fileList.clear();
		fileList.shrink_to_fit();

		QStringList filters;
		filters << "*.dcm";
		//Set type filters only for file formats, do not use symbolic links
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		//Set file name filters, filters format
		dir.setNameFilters(filters);
		//Number of record files
		file_count = dir.count();
		if (file_count == 0) {
			DebugTextPrintErrorString("No .dcm file exists");
			return false;
		}
		_DirPath = inputDir;
		if (inputDir[inputDir.size() - 1] != '/') _DirPath = _DirPath + "/";
	}

	for (unsigned int i = 0; i < file_count; i++) {
		QString file_Path = _DirPath + dir[i];
		fileList.push_back(file_Path);
	}
	return true;
}

bool DataReaderAndWriter::getInputPngsFileList(const QString& inputDir, std::vector<QString>& fileList) {
	QDir dir(inputDir);
	unsigned int file_count = 0;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			DebugTextPrintErrorString("Fatal: Dir does not exist!");
			return false;
		}

		fileList.clear();
		fileList.shrink_to_fit();

		QStringList filters;
		filters << "*.png";
		//Set type filters only for file formats, do not use symbolic links
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		//Set file name filters, filters format
		dir.setNameFilters(filters);
		//Number of record files
		file_count = dir.count();
		if (file_count == 0) {
			DebugTextPrintErrorString("No .png file exists");
			return false;
		}
		_DirPath = inputDir;
		if (inputDir[inputDir.size() - 1] != '/') _DirPath = _DirPath + "/";
	}

	for (unsigned int i = 0; i < file_count; i++) {
		QString file_Path = _DirPath + dir[i];
		fileList.push_back(file_Path);
	}
	return true;

}

bool DataReaderAndWriter::getInputJpgsFileList(const QString& inputDir, std::vector<QString>& fileList) {
	QDir dir(inputDir);
	unsigned int file_count = 0;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			DebugTextPrintErrorString("Fatal: Dir does not exist!");
			return false;
		}

		fileList.clear();
		fileList.shrink_to_fit();

		QStringList filters;
		filters << "*.jpg";
		//Set type filters only for file formats, do not use symbolic links
		dir.setFilter(QDir::Files | QDir::NoSymLinks);
		//Set file name filters, filters format
		dir.setNameFilters(filters);
		//Number of record files
		file_count = dir.count();
		if (file_count == 0) {
			DebugTextPrintErrorString("No .jpg file exists");
			return false;
		}
		_DirPath = inputDir;
		if (inputDir[inputDir.size() - 1] != '/') _DirPath = _DirPath + "/";
	}

	for (unsigned int i = 0; i < file_count; i++) {
		QString file_Path = _DirPath + dir[i];
		fileList.push_back(file_Path);
	}
	return true;
}

// **********************************************//
// *** Generate input data to VolumeData object *** //
// **********************************************//

bool DcmFilePosCompare(const DcmFilePixelData &f1, const DcmFilePixelData &f2) {
	if (f1.position < f2.position) return true;
	else return false;
}

bool DataReaderAndWriter::GenerateInput_GDCM(const std::vector<QString>& fileList, VolumeData& volumeData) {
	
	std::vector<DcmFilePixelData> dcmFileVec;
	unsigned int width, height, imageNum;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	unsigned int bits_perPixel;

	std::vector<unsigned int> correctImagesNum;
	for (int i = 0; i < fileList.size(); i++) {
		gdcm::Reader reader;

		QString file_Path = fileList[i];
		reader.SetFileName(file_Path.toStdString().c_str());
		if (!reader.Read()) continue;
		// The output of gdcm::Reader is a gdcm::File
		gdcm::File &file = reader.GetFile();
		// the dataset is the the set of element we are interested in:
		gdcm::DataSet &ds = file.GetDataSet();

		const gdcm::Global& g = gdcm::Global::GetInstance();
		const gdcm::Dicts &dicts = g.GetDicts();
		const gdcm::Dict &pubdict = dicts.GetPublicDict();

		// Rows
		{
			gdcm::Attribute<0x0028, 0x0010> at_rows;
			at_rows.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0010>::ArrayType v_rows = at_rows.GetValue();
			unsigned int height_t = v_rows;

			if (correctImagesNum.size() != 0 && height != height_t) {
				DebugTextPrintErrorString(("The height of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				height = height_t;
				volumeData.yResolution = height;
			}
		}

		// Columns
		{
			gdcm::Attribute<0x0028, 0x0011> at_columns;
			at_columns.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0011>::ArrayType v_columns = at_columns.GetValue();
			unsigned int width_t = v_columns;

			if (correctImagesNum.size() != 0 && width != width_t) {
				DebugTextPrintErrorString(("The width of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				width = width_t;
				volumeData.xResolution = width;
			}
		}

		// Spacing between slices
		{
			gdcm::Attribute<0x0018, 0x0050> at_Spacing_slices;
			at_Spacing_slices.SetFromDataSet(ds);
			gdcm::Attribute<0x0018, 0x0050>::ArrayType v_Spacing_slices = at_Spacing_slices.GetValue();

			double pixelSpacing_Z_t = v_Spacing_slices;

			if (correctImagesNum.size() != 0 && pixelSpacing_Z != pixelSpacing_Z_t) {
				DebugTextPrintErrorString(("The slice thickness of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_Z = pixelSpacing_Z_t;
				volumeData.zPixelSpace = pixelSpacing_Z;
			}
		}

		// Spacing between pixels  Unit:mm
		{
			gdcm::Attribute<0x0028, 0x0030> at_Pixel_Spacing;
			at_Pixel_Spacing.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0030>::ArrayType vv_Pixel_Spacing = at_Pixel_Spacing.GetValue();

			double pixelSpacing_X_t = vv_Pixel_Spacing;

			if (correctImagesNum.size() != 0 && pixelSpacing_X != pixelSpacing_X_t) {
				DebugTextPrintErrorString(("The pixel spacing of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_X = pixelSpacing_X_t;
				pixelSpacing_Y = pixelSpacing_X_t;
				volumeData.xPixelSpace = pixelSpacing_X;
				volumeData.yPixelSpace = pixelSpacing_Y;
			}
		}

		// bits per pixel
		{
			gdcm::Attribute<0x0028, 0x0100> at_bit;
			at_bit.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0100>::ArrayType v_bit = at_bit.GetValue();
			unsigned int bits_perPixel_t = v_bit;
			if (correctImagesNum.size() == 0) {
				bits_perPixel = v_bit;
			}
			else if (bits_perPixel_t != bits_perPixel) {
				DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
		}

		// format
		{
			gdcm::ImageReader imageReader;
			imageReader.SetFileName(file_Path.toStdString().c_str());
			if (!imageReader.Read()) {
				DebugTextPrintErrorString(("Unable to read image data from file" + file_Path).toStdString().c_str());
				continue;
			}

			gdcm::Image &image = imageReader.GetImage();
			gdcm::PixelFormat pixelFormat = image.GetPixelFormat();

			bool formatFlag = true;
			switch (pixelFormat.GetScalarType())
			{
			case gdcm::PixelFormat::INT8:
				if (correctImagesNum.size() == 0) 
					volumeData.format = Dez_SignedChar;
				else if (volumeData.format != Dez_SignedChar) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT8:
				if (correctImagesNum.size() == 0) 
					volumeData.format = Dez_UnsignedChar;
				else if (volumeData.format != Dez_UnsignedChar) formatFlag = false;
				break;
			case gdcm::PixelFormat::INT16:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_SignedShort;
				else if (volumeData.format != Dez_SignedShort) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT16:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_UnsignedShort;
				else if (volumeData.format != Dez_UnsignedShort) formatFlag = false;
				break;
			case gdcm::PixelFormat::INT32:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_SignedLong;
				else if (volumeData.format != Dez_SignedLong) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT32:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_UnsignedLong;
				else if (volumeData.format != Dez_UnsignedLong) formatFlag = false;
				break;
			case gdcm::PixelFormat::FLOAT32:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_Float;
				else if (volumeData.format != Dez_Float) formatFlag = false;
				break;
			case gdcm::PixelFormat::FLOAT64:
				if (correctImagesNum.size() == 0)
					volumeData.format = Dez_Double;
				else if (volumeData.format != Dez_Double) formatFlag = false;
				break;
			default:
				DebugTextPrintErrorString(("Unknown pixel data type in File " + file_Path).toStdString().c_str());
				formatFlag = false;
				break;
			}

			if (!formatFlag) {
				DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
				
		}

		correctImagesNum.push_back(i);
	}

	if (correctImagesNum.size() == 0) {
		DebugTextPrintErrorString("No suitable DCM images to read");
		return false;
	}

	volumeData.zResolution = correctImagesNum.size();
	imageNum = correctImagesNum.size();

	// check data format
	unsigned int bytesOnPixel = 0;
	{
		if (volumeData.format == Dez_Float) {
			bytesOnPixel = sizeof(float);
		}
		else if (volumeData.format == Dez_Double) {
			bytesOnPixel = sizeof(double);
		}
		else if (volumeData.format == Dez_UnsignedLong) {
			bytesOnPixel = sizeof(unsigned long);
		}
		else if (volumeData.format == Dez_SignedLong) {
			bytesOnPixel = sizeof(long);
		}
		else if (volumeData.format == Dez_UnsignedShort) {
			if (bits_perPixel != 16) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned short);
		}
		else if (volumeData.format == Dez_SignedShort) {
			if (bits_perPixel != 16) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(short);
		}
		else if (volumeData.format == Dez_UnsignedChar) {
			if (bits_perPixel != 8) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned char);
		}
		else if (volumeData.format == Dez_SignedChar) {
			if (bits_perPixel != 8) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(char);
		}
		else {
			DebugTextPrintErrorString("Temporarily unsupported data format: error format");
			return false;
		}
	}

	// Print import data Info
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	char * m_data = new char[width * height * imageNum * bytesOnPixel];
	if (!m_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return false;
	}

	bool readFlag = true;
	for (int i = 0; i < correctImagesNum.size(); i++) {
		
		gdcm::Reader reader;
		QString file_Path = fileList[correctImagesNum[i]];
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
		double imagePos_temp;
		{
			gdcm::Attribute<0x0020, 0x1041> at_position;
			at_position.SetFromDataSet(ds);
			gdcm::Attribute<0x0020, 0x1041>::ArrayType v_position = at_position.GetValue();
			imagePos_temp = v_position;
		}

		gdcm::ImageReader ir;
		ir.SetFileName(file_Path.toStdString().c_str());
		if (!ir.Read()) {
			//Read failed
			DebugTextPrintErrorString(("Unable to read image data from File " + file_Path).toStdString().c_str());
			readFlag = false;
			break;
		}

		const gdcm::Image &gimage = ir.GetImage();
		std::vector<char> vbuffer;
		vbuffer.resize(gimage.GetBufferLength());
		char *buffer = &vbuffer[0];
		gimage.GetBuffer(buffer);

		DcmFilePixelData dcmfile;
		dcmfile.position = imagePos_temp;
		dcmfile.pixData = m_data + i * width * height * bytesOnPixel;
		
		memcpy(m_data + i * width * height * bytesOnPixel, buffer, width * height * bytesOnPixel);

		dcmFileVec.push_back(dcmfile);
	}

	if (readFlag) {
		std::sort(dcmFileVec.begin(), dcmFileVec.end(), DcmFilePosCompare);

		volumeData.data = new char[width * height * imageNum * bytesOnPixel];
		if (!volumeData.data) {
			DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
			readFlag = false;
		}

		if (readFlag)
			for (unsigned int i = 0; i < imageNum; i++) {
				if (dcmFileVec[i].pixData == nullptr) {
					DebugTextPrintErrorString("(dcmFileVec[i].pixData16 == nullptr) ");
					readFlag = false;
					break;
				}
				memcpy((char*)volumeData.data + i * width * height * bytesOnPixel, 
					dcmFileVec[i].pixData, 
					width * height * bytesOnPixel);
			}
	}

	delete[] m_data;
	return readFlag;
}

bool DataReaderAndWriter::GenerateInput_DCMTK(const std::vector<QString>& fileList, VolumeData& volumeData) {

	unsigned int width = 0, height = 0;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	Uint16 bitsAllocated, bitsStored, pixelRepresentation;

	// valid 
	std::vector<unsigned int> correctImagesNum;
	for (unsigned int i = 0; i < fileList.size(); i++)
	{

		QString file_Path = fileList[i];
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
				volumeData.xPixelSpace = pixelSpacing_X;
				volumeData.yPixelSpace = pixelSpacing_Y;
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
				volumeData.zPixelSpace = pixelSpacing_Z;
			}
		}

		// Get Bit Allocated
		{
			Uint16 bitsAllocated_temp, bitsStored_temp, pixelRepresentation_temp;
			if (correctImagesNum.size() == 0) {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored).good() && 
					data->findAndGetUint16(DcmTagKey(0x0028, 0x0103), pixelRepresentation).good()) 
				{
					DebugTextPrintNum("bitsAllocated", bitsAllocated);
					DebugTextPrintNum("bitsStored", bitsStored);
					DebugTextPrintNum("pixelRepresentation", pixelRepresentation);
				}
				else 
				{
					DebugTextPrintErrorString(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated_temp).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored_temp).good() &&
					data->findAndGetUint16(DcmTagKey(0x0028, 0x0103), pixelRepresentation_temp).good()) 
				{
					if (bitsAllocated_temp != bitsAllocated || 
						bitsStored_temp != bitsStored || 
						pixelRepresentation_temp != pixelRepresentation) 
					{
						DebugTextPrintErrorString(("The bits allocated or stored is inconsistent in File" + file_Path).toStdString().c_str());
						continue;
					}
				}
				else 
				{
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
				// std::cout << "Value Representation: " << DcmVR(valueRepresentation).getVRName() << std::endl;

				// Float
				if (valueRepresentation == EVR_FL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_Float;
					else if (volumeData.format != Dez_Float) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// Double
				else if (valueRepresentation == EVR_FD) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_Double;
					else if (volumeData.format != Dez_Double) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
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
						continue;
					}
				}
				// signed long
				else if (valueRepresentation == EVR_SL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_SignedLong;
					else if (volumeData.format != Dez_SignedLong) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// unsigned long
				else if (valueRepresentation == EVR_UL) {
					if (correctImagesNum.size() == 0) volumeData.format = Dez_UnsignedLong;
					else if (volumeData.format != Dez_UnsignedLong) {
						DebugTextPrintErrorString(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
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

	if (correctImagesNum.size() == 0) {
		DebugTextPrintErrorString("No suitable DCM images to read");
		return false;
	}

	// check data format
	unsigned int bytesOnPixel = 0;
	{
		if (volumeData.format == Dez_Float) {
			bytesOnPixel = sizeof(float);
		}
		else if (volumeData.format == Dez_Double) {
			bytesOnPixel = sizeof(double);
		}
		else if (volumeData.format == Dez_UnsignedLong) {
			bytesOnPixel = sizeof(unsigned long);
		}
		else if (volumeData.format == Dez_SignedLong) {
			bytesOnPixel = sizeof(long);
		}
		else if (volumeData.format == Dez_UnsignedShort) {
			if (bitsAllocated != 16) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned short);
		}
		// When using DCMTK, there may be a mismatch between signed and unsigned values. 
		// It is reasonable to use pixelRepresentation and bitsStored to determine the accurate result, 
		// which is consistent with GDCM parsing. The results determined by getVR() are inconsistent.
		else if (volumeData.format == Dez_SignedShort) {
			if (bitsAllocated != 16) {
				DebugTextPrintErrorString("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(short);
		}
		else if (volumeData.format == Dez_UnsignedChar) {
			DebugTextPrintErrorString("Temporarily unsupported data format: unsigend char");
			return false;
		}
		else if (volumeData.format == Dez_SignedChar) {
			DebugTextPrintErrorString("Temporarily unsupported data format: char");
			return false;
		}
		else {
			DebugTextPrintErrorString("Temporarily unsupported data format: error format");
			return false;
		}
	}

	unsigned int imageNum = correctImagesNum.size();
	volumeData.zResolution = imageNum;

	// Print import data Info
	DebugTextPrintString(volumeData.toString().toStdString().c_str());

	void * m_data = new char[width * height * imageNum * bytesOnPixel];
	if (!m_data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return false;
	}

	// Start processing each image
	bool readDataFlag = true;
	std::vector<DcmFilePixelData> dcmFileVec;
	for (unsigned int i = 0; i < imageNum; i++) {
		// File Path
		QString file_Path = fileList[correctImagesNum[i]];

		// DcmFileFormat will automatically release memory
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
		if (!Metalnfo) {
			DebugTextPrintErrorString(("No Metalnfo in File " + file_Path).toStdString().c_str());
			readDataFlag = false;
			break;
		}
		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			DebugTextPrintErrorString(("No data in File " + file_Path).toStdString().c_str());
			readDataFlag = false;
			break;
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

		if (volumeData.format == Dez_UnsignedShort) {
			Uint16* dat_t;
			element->getUint16Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Uint16));
		}
		else if (volumeData.format == Dez_SignedShort) {
			Sint16* dat_t;
			element->getSint16Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Sint16));
		}
		else if (volumeData.format == Dez_Float) {
			Float32* dat_t;
			element->getFloat32Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Float32));
		}
		else if (volumeData.format == Dez_Double) {
			Float64* dat_t;
			element->getFloat64Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Float64));
		}
		else {
			DebugTextPrintErrorString("Temporarily unsupported data format!");
			readDataFlag = false;
		}

		DcmFilePixelData dcmF;
		dcmF.pixData = static_cast<char*>(m_data) + i * width * height * bytesOnPixel;
		dcmF.position = atof(ImagePos.data());
		dcmFileVec.push_back(dcmF);
	}

	// sort images by their positions and save to volumeData.data
	if (readDataFlag) {
		// Sort by image position
		std::sort(dcmFileVec.begin(), dcmFileVec.end(), DcmFilePosCompare);

		volumeData.data = new char[width * height * imageNum * bytesOnPixel];
		if (!volumeData.data) {
			DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		}
		else {
			for (unsigned int i = 0; i < imageNum; i++) {
				if (dcmFileVec[i].pixData == nullptr) {
					DebugTextPrintErrorString("(dcmFileVec[i].pixData == nullptr) ");
					readDataFlag = false;
					break;
				}
				else {
					memcpy(static_cast<char*>(volumeData.data) + i * width * height * bytesOnPixel,
						dcmFileVec[i].pixData,
						width * height * bytesOnPixel);
				}
			}
		}

	}

	delete [] static_cast<char *>(m_data);
	return readDataFlag;
}

template <typename T>
bool DataReaderAndWriter::CopyUncompressedRawData(const T* data, VolumeData& volumeData)
{
	if (!data) return false;
	
	volumeData.data = new T[volumeData.xResolution * volumeData.yResolution * volumeData.zResolution];
	if (!volumeData.data) return false;
	for (unsigned int i = 0; i < volumeData.zResolution; i++) {
		memcpy((T*)volumeData.data + i * volumeData.xResolution * volumeData.yResolution,
			(T*)data + i * volumeData.xResolution * volumeData.yResolution, 
			volumeData.xResolution * volumeData.yResolution * sizeof(T));
	}
	return true;
}

bool DataReaderAndWriter::GenerateInput_Mhd(const QString& inputFilePath, VolumeData& volumeData) {

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(inputFilePath.toStdString().c_str());
	reader->Update();
	
	vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();
	int dataType = imageData->GetScalarType();

	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData(imageData);
	ImageCast->Update();

	int * volumeDataShape = ImageCast->GetOutput()->GetExtent();
	volumeData.xResolution = volumeDataShape[1] + 1;
	volumeData.yResolution = volumeDataShape[3] + 1;
	volumeData.zResolution = volumeDataShape[5] + 1;

	double* spacing = ImageCast->GetOutput()->GetSpacing();
	volumeData.xPixelSpace = spacing[0];
	volumeData.yPixelSpace = spacing[1];
	volumeData.zPixelSpace = spacing[2];

	bool flag = true;
	const bool DebugCopyInfo = true;
	switch (dataType)
	{
	case VTK_UNSIGNED_CHAR:
		volumeData.format = Dez_UnsignedChar;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned char*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy unsigned char data from Mhd to array");
		break;
	case VTK_SIGNED_CHAR:
		volumeData.format = Dez_SignedChar;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed char*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy signed char data from Mhd to array");
		break;
	case VTK_UNSIGNED_SHORT:
		volumeData.format = Dez_UnsignedShort;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned short*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy unsigned short data from Mhd to array");
		break;
	case VTK_SHORT:
		volumeData.format = Dez_SignedShort;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed short*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy signed short data from Mhd to array");
		break;
	case VTK_UNSIGNED_INT:
		volumeData.format = Dez_UnsignedLong;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned int*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy unsigned long data from Mhd to array");
		break;
	case VTK_INT:
		volumeData.format = Dez_SignedLong;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed int*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy signed long data from Mhd to array");
		break;
	case VTK_FLOAT:
		volumeData.format = Dez_Float;
		flag = flag && CopyUncompressedRawData(
			static_cast<float*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy [float] data from Mhd to array");
		break;
	case VTK_DOUBLE:
		volumeData.format = Dez_Double;
		flag = flag && CopyUncompressedRawData(
			static_cast<double*>(ImageCast->GetOutput()->GetScalarPointer()),
			volumeData);
		if (DebugCopyInfo)
			DebugTextPrintString("Copy [double] data from Mhd to array");
		break;
	default:
		DebugTextPrintErrorString("Failed to read data from Mhd file.");
		flag = false;
		break;
	}

	// I don't know why this version of the smart pointer cannot properly release memory, 
	// so I manually released it here
	ImageCast->Delete();

	return flag;
}

bool ReadUncompressedRawData(std::string filename, unsigned int bytesOneScalar, VolumeData& volumeData) {
	unsigned int width = volumeData.xResolution, height = volumeData.yResolution, imageNUm = volumeData.zResolution;

	volumeData.data = new char[volumeData.xResolution * volumeData.yResolution * volumeData.zResolution * bytesOneScalar];
	if (!volumeData.data) return false;

	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;
	try {
		file.read(reinterpret_cast<char*>(volumeData.data), bytesOneScalar * width * height * imageNUm);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return false;
	}

	file.close();
	return true;
}

bool DataReaderAndWriter::GenerateInput_Feimos(const QString& inputFilePath, VolumeData& volumeData) {

	std::ifstream file(inputFilePath.toStdString());
	std::string name;
	std::string rawDataFileName;

	while (file >> name) {
		QString qName = name.c_str();
		if (qName.compare("xResolution", Qt::CaseInsensitive) == 0) {
			file >> volumeData.xResolution;
		}
		else if (qName.compare("yResolution", Qt::CaseInsensitive) == 0) {
			file >> volumeData.yResolution;
		}
		else if (qName.compare("zResolution", Qt::CaseInsensitive) == 0) {
			file >> volumeData.zResolution;
		}
		else if (qName.compare("xPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> volumeData.xPixelSpace;
		}
		else if (qName.compare("yPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> volumeData.yPixelSpace;
		}
		else if (qName.compare("zPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> volumeData.zPixelSpace;
		}
		else if (qName.compare("format", Qt::CaseInsensitive) == 0) {
			file >> name;
			if (!volumeData.setFormatUsingString(name.c_str())) {
				DebugTextPrintErrorString("Non compliant format input when parsing (.feimos) file!");
				return false;
			}
		}
		else if (qName.compare("Raw", Qt::CaseInsensitive) == 0) {
			std::getline(file, name);
			name.erase(name.begin());
			rawDataFileName = name;
		}
	}

	size_t dataSize = (size_t)volumeData.xResolution * (size_t)volumeData.yResolution * (size_t)volumeData.zResolution;
	if (dataSize > (size_t)1024 * (size_t)1024 * (size_t)1024) {
		DebugTextPrintErrorString("The image size read in is too large and currently not supported. Please use other reading functions.");
		return false;
	}

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	if (!isInputFileExist(absoluteDirPath + "/" + rawDataFileName.c_str())) {
		DebugTextPrintErrorString("Raw data does not exist when parsing (.feimos) file!");
		return false;
	}

	bool readbinaryFlag = true;
	switch (volumeData.format)
	{
	case Dez_Origin:
		DebugTextPrintErrorString("Non compliant data format");
		return false;
		break;
	case Dez_UnsignedLong:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned long), volumeData);
		break;
	case Dez_SignedLong:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed long), volumeData);
		break;
	case Dez_UnsignedShort:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned short), volumeData);
		break;
	case Dez_SignedShort:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed short), volumeData);
		break;
	case Dez_UnsignedChar:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned char), volumeData);
		break;
	case Dez_SignedChar:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed char), volumeData);
		break;
	case Dez_Float:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(float), volumeData);
		break;
	case Dez_Double:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(double), volumeData);
		break;
	default:
		DebugTextPrintErrorString("Non compliant data output format");
		return false;
		break;
	}

	if (!readbinaryFlag) {
		DebugTextPrintErrorString("Fail to Read to binary file");
		return false;
	}

	return true;
}


// ********************************************** //
// *** Generate output data *** //
// ********************************************** //

template <typename T, typename U>
bool __DataFormatConvert(const T* data, U* aimdata, VolumeData& volumeData) {
	unsigned int width = volumeData.xResolution, height = volumeData.yResolution, images = volumeData.zResolution;

	aimdata = new U[width * height * images];
	if (!aimdata) return false;

	for (unsigned int k = 0; k < volumeData.zResolution; k++) {

		T* imageP = (T *)data + k * width * height;
		U* imageAimP = (U *)aimdata + k * width * height;

		for (unsigned int i = 0; i < volumeData.xResolution; i++) {
			for (unsigned int j = 0; j < volumeData.yResolution; j++) {
				imageAimP[i + j * volumeData.xResolution] = 
					static_cast<U>(imageP[i + j * volumeData.xResolution]);
			}
		}
	}
	return true;
}

bool DataReaderAndWriter::DataFormatConvertToWrite(const GenerateFormat& generateFormat, VolumeData& volumeData) {

	// To ensure data accuracy, only a small number of conversions are supported.

	if (!volumeData.data) {
		DebugTextPrintErrorString("volumeData has no data");
		return false;
	}

	if (generateFormat.format == Dez_Origin || volumeData.format == generateFormat.format) {
		volumeData.data_toWrite = volumeData.data;
		volumeData.format_toWrite = volumeData.format;
		DebugTextPrintString("No need to convert data format.");
		return true;
	}
	else {
		DebugTextPrintString("Need to convert data format.");
	}

	bool convertFlag = true;

	if (volumeData.format == Dez_UnsignedChar && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvert(
				static_cast<unsigned char*>(volumeData.data),
				static_cast<float*>(volumeData.data_toWrite),
				volumeData);
		if (convertFlag)
			volumeData.format_toWrite = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_SignedChar && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvert(
				static_cast<signed char*>(volumeData.data),
				static_cast<float*>(volumeData.data_toWrite),
				volumeData);
		if (convertFlag)
			volumeData.format_toWrite = Dez_Float;
		return convertFlag;
	}


	if (volumeData.format == Dez_SignedShort && generateFormat.format == Dez_Float) {
		
		convertFlag = convertFlag && 
			__DataFormatConvert(
				static_cast<signed short*>(volumeData.data),
				static_cast<float*>(volumeData.data_toWrite),
				volumeData);
		if (convertFlag)
			volumeData.format_toWrite = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_UnsignedShort && generateFormat.format == Dez_Float) {
		
		convertFlag = convertFlag &&
			__DataFormatConvert(
				static_cast<unsigned short*>(volumeData.data),
				static_cast<float*>(volumeData.data_toWrite),
				volumeData);
		if (convertFlag)
			volumeData.format_toWrite = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_Float && generateFormat.format == Dez_SignedShort) {

		convertFlag = convertFlag &&
			__DataFormatConvert(
				static_cast<float*>(volumeData.data),
				static_cast<short*>(volumeData.data_toWrite),
				volumeData);
		if (convertFlag)
			volumeData.format_toWrite = Dez_Float;
		return convertFlag;
	}

	DebugTextPrintErrorString("Unsupported conversion");
	return false;
}

template <typename T, typename U>
bool __DataFormatConvertSelf(T indicate1, U indicate2, VolumeData& volumeData) {
	unsigned int width = volumeData.xResolution, height = volumeData.yResolution, images = volumeData.zResolution;

	T* data = static_cast<T *>(volumeData.data);
	void* aimdata = new U[width * height * images];
	if (!aimdata) return false;

	for (unsigned int k = 0; k < volumeData.zResolution; k++) {

		T* imageP = static_cast<T *>(data) + k * width * height;
		U* imageAimP = static_cast<U *>(aimdata) + k * width * height;

		for (unsigned int i = 0; i < volumeData.xResolution; i++) {
			for (unsigned int j = 0; j < volumeData.yResolution; j++) {
				imageAimP[i + j * volumeData.xResolution] =
					static_cast<U>(imageP[i + j * volumeData.xResolution]);
			}
		}
	}

	delete[] static_cast<T *>(volumeData.data);
	volumeData.data = static_cast<void *>(aimdata);
	return true;
}
bool DataReaderAndWriter::DataFormatConvertToInteract(const GenerateFormat& generateFormat, VolumeData& volumeData) {
	// To ensure data accuracy, only a small number of conversions are supported.

	if (!volumeData.data) {
		DebugTextPrintErrorString("volumeData has no data");
		return false;
	}

	if (generateFormat.format == Dez_Origin || volumeData.format == generateFormat.format) {
		DebugTextPrintString("No need to convert data format.");
		return true;
	}
	else {
		DebugTextPrintString("Need to convert data format.");
	}

	bool convertFlag = true;

	if (volumeData.format == Dez_UnsignedChar && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvertSelf(
				static_cast<unsigned char>(1),
				static_cast<float>(1),
				volumeData);
		if (convertFlag)
			volumeData.format = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_SignedChar && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvertSelf(
				static_cast<signed char>(1),
				static_cast<float>(1),
				volumeData);
		if (convertFlag)
			volumeData.format = Dez_Float;
		return convertFlag;
	}


	if (volumeData.format == Dez_SignedShort && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvertSelf(
				static_cast<signed short>(1),
				static_cast<float>(1),
				volumeData);
		if (convertFlag)
			volumeData.format = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_UnsignedShort && generateFormat.format == Dez_Float) {

		convertFlag = convertFlag &&
			__DataFormatConvertSelf(
				static_cast<unsigned short>(1),
				static_cast<float>(1),
				volumeData);
		if (convertFlag)
			volumeData.format = Dez_Float;
		return convertFlag;
	}

	if (volumeData.format == Dez_Float && generateFormat.format == Dez_SignedShort) {

		convertFlag = convertFlag &&
			__DataFormatConvertSelf(
				static_cast<float>(1),
				static_cast<signed short>(1),
				volumeData);
		if (convertFlag)
			volumeData.format = Dez_SignedShort;
		return convertFlag;
	}

	DebugTextPrintErrorString("Unsupported conversion");
	return false;

}

bool DataReaderAndWriter::GenerateOutput_Mhd(const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat, VolumeData& volumeData) {

	if (!DataFormatConvertToWrite(generateFormat, volumeData)) {
		return false;
	}

	int dimM[3] = { volumeData.xResolution, volumeData.yResolution, volumeData.zResolution };
	double PixelSpace[3] = { volumeData.xPixelSpace, volumeData.yPixelSpace, volumeData.zPixelSpace};

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace[0], PixelSpace[1], PixelSpace[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM[0] - 1, 0, dimM[1] - 1, 0, dimM[2] - 1);
	imageImport->SetDataExtentToWholeExtent();

	switch (volumeData.format_toWrite)
	{
	case Dez_Origin:
		DebugTextPrintErrorString("Error Mhd output format");
		return false;
		break;
	case Dez_UnsignedLong:
		DebugTextPrintErrorString("Undefined Mhd output format in VTK");
		return false;
		break;
	case Dez_SignedLong:
		imageImport->SetDataScalarTypeToInt();
		break;
	case Dez_UnsignedShort:
		imageImport->SetDataScalarTypeToUnsignedShort();
		break;
	case Dez_SignedShort:
		imageImport->SetDataScalarTypeToShort();
		break;
	case Dez_UnsignedChar:
		imageImport->SetDataScalarTypeToUnsignedChar();
		break;
	case Dez_SignedChar:
		DebugTextPrintErrorString("Undefined Mhd output format in VTK");
		return false;
		break;
	case Dez_Float:
		imageImport->SetDataScalarTypeToFloat();
		break;
	case Dez_Double:
		imageImport->SetDataScalarTypeToDouble();
		break;
	default:
		DebugTextPrintErrorString("Error Mhd output format");
		return false;
		break;
	}
	
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(volumeData.data_toWrite);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();

	DebugTextPrintString("Write to Mhd file successfully!");
	return true;
}

bool SaveUncompressedRawData(std::string filename, unsigned int bytesOneScalar, const VolumeData& volumeData) {
	unsigned int width = volumeData.xResolution, height = volumeData.yResolution, imageNUm = volumeData.zResolution;

	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;
	try {
		file.write(reinterpret_cast<const char*>(volumeData.data_toWrite), bytesOneScalar * width * height * imageNUm);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return false;
	}

	file.close();
	return true;
}

bool DataReaderAndWriter::GenerateOutput_Feimos(const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat, VolumeData& volumeData) {

	if (!DataFormatConvertToWrite(generateFormat, volumeData)) {
		return false;
	}

	std::string format; bool writebinaryFlag = true;
	switch (volumeData.format_toWrite)
	{
	case Dez_Origin:
		format = "Error";
		DebugTextPrintErrorString("Non compliant data output format");
		return false;
		break;
	case Dez_UnsignedLong:
		format = "UnsignedLong";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned long), volumeData);
		break;
	case Dez_SignedLong:
		format = "SignedLong";
		writebinaryFlag = writebinaryFlag &&
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed long), volumeData);
		break;
	case Dez_UnsignedShort:
		format = "UnsignedShort";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned short), volumeData);
		break;
	case Dez_SignedShort:
		format = "SignedShort";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed short), volumeData);
		break;
	case Dez_UnsignedChar:
		format = "UnsignedChar";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned char), volumeData);
		break;
	case Dez_SignedChar:
		format = "SignedChar";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed char), volumeData);
		break;
	case Dez_Float:
		format = "Float";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(float), volumeData);
		break;
	case Dez_Double:
		format = "Double";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(double), volumeData);
		break;
	default:
		DebugTextPrintErrorString("Non compliant data output format");
		return false;
		break;
	}
	if (!writebinaryFlag) {
		DebugTextPrintErrorString("Fail to write to binary file");
		return false;
	}

	{
		std::ofstream fileInfo((outputDir + "/" + outName + ".feimos").toStdString());
		if (!fileInfo.is_open()) {
			DebugTextPrintErrorString("Fail to write to info file");
			return false;
		}
		fileInfo << "xResolution " << volumeData.xResolution << std::endl;
		fileInfo << "yResolution " << volumeData.yResolution << std::endl;
		fileInfo << "zResolution " << volumeData.zResolution << std::endl;
		fileInfo << "xPixelSpace " << volumeData.xPixelSpace << std::endl;
		fileInfo << "yPixelSpace " << volumeData.yPixelSpace << std::endl;
		fileInfo << "zPixelSpace " << volumeData.zPixelSpace << std::endl;
		fileInfo << "format " << format << std::endl;
		fileInfo << "Raw " << (outName.toStdString() + ".raw") << std::endl;
		fileInfo.close();
	}
	
	DebugTextPrintString("Write to Feimos file successfully!");
	return true;
}


#define STB_IMAGE_IMPLEMENTATION
#include "3rdLibs/stb_image.h"

bool DataReaderAndWriter::GenerateInput_PNGs(const std::vector<QString>& fileList, VolumeData& volumeData) {
	std::vector<unsigned int> correctImagesNum;
	std::vector<unsigned char *> dataPoints;
	unsigned int correctImagesCount = 0;
	int width_seq, height_seq, nrChannels_seq;
	for (int i = 0; i < fileList.size(); i++) {

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char *data = stbi_load(fileList[i].toStdString().c_str(), &width, &height, &nrChannels, 0);

		if (data && nrChannels == 4) {
			if (correctImagesCount == 0) {
				width_seq = width;
				height_seq = height;
				nrChannels_seq = nrChannels;

				dataPoints.push_back(data);
				correctImagesCount++;
			}
			else {
				if (width != width_seq || height != height_seq || nrChannels != nrChannels_seq) {
					DebugTextPrintErrorString(("The size of the image sequence is inconsistent in File " + fileList[i]).toStdString().c_str());
				}
				else {
					dataPoints.push_back(data);
					correctImagesCount++;
				}

			}

		}

	}

	if (correctImagesCount == 0) {
		DebugTextPrintErrorString("No suitable PNG images to read");
		return false;
	}

	volumeData.clear();
	volumeData.data = new unsigned char[width_seq * height_seq * correctImagesCount];
	if (!volumeData.data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return false;
	}
	volumeData.xPixelSpace = 1.0;
	volumeData.yPixelSpace = 1.0;
	volumeData.zPixelSpace = 1.0;
	volumeData.xResolution = width_seq;
	volumeData.yResolution = height_seq;
	volumeData.zResolution = correctImagesCount;
	volumeData.format = Dez_UnsignedChar;

	// 0.299R + 0.587G + 0.114B
	for (unsigned int k = 0; k < dataPoints.size(); k++) {
		for (unsigned int i = 0; i < volumeData.xResolution; i++) {
			for (unsigned int j = 0; j < volumeData.yResolution; j++) {
				unsigned char* data_gray = static_cast<unsigned char*>(volumeData.data) + 
					k * volumeData.xResolution * volumeData.yResolution;
				unsigned int offset = j * volumeData.xResolution + i;
				data_gray[offset] = static_cast<unsigned char>(0.299 * dataPoints[k][offset * 4 + 0]
					+ 0.587 * dataPoints[k][offset * 4 + 1]
					+ 0.114 * dataPoints[k][offset * 4 + 2]);

			}
		}

		delete[] dataPoints[k];
	}

	return true;
}

bool DataReaderAndWriter::GenerateInput_JPGs(const std::vector<QString>& fileList, VolumeData& volumeData) {
	std::vector<unsigned int> correctImagesNum;
	std::vector<unsigned char *> dataPoints;
	unsigned int correctImagesCount = 0;
	int width_seq, height_seq, nrChannels_seq;
	for (int i = 0; i < fileList.size(); i++) {

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char *data = stbi_load(fileList[i].toStdString().c_str(), &width, &height, &nrChannels, 0);

		if (data && nrChannels == 3) {
			if (correctImagesCount == 0) {
				width_seq = width;
				height_seq = height;
				nrChannels_seq = nrChannels;

				dataPoints.push_back(data);
				correctImagesCount++;
			}
			else {
				if (width != width_seq || height != height_seq || nrChannels != nrChannels_seq) {
					DebugTextPrintErrorString(("The size of the image sequence is inconsistent in File " + fileList[i]).toStdString().c_str());
				}
				else {
					dataPoints.push_back(data);
					correctImagesCount++;
				}
			}
		}
	}

	if (correctImagesCount == 0) {
		DebugTextPrintErrorString("No suitable PNG images to read");
		return false;
	}

	volumeData.clear();
	volumeData.data = new unsigned char[width_seq * height_seq * correctImagesCount];
	if (!volumeData.data) {
		DebugTextPrintErrorString("Unable to request sufficient amount of memory!");
		return false;
	}
	volumeData.xPixelSpace = 1.0;
	volumeData.yPixelSpace = 1.0;
	volumeData.zPixelSpace = 1.0;
	volumeData.xResolution = width_seq;
	volumeData.yResolution = height_seq;
	volumeData.zResolution = correctImagesCount;
	volumeData.format = Dez_UnsignedChar;

	// 0.299R + 0.587G + 0.114B
	for (unsigned int k = 0; k < dataPoints.size(); k++) {
		for (unsigned int i = 0; i < volumeData.xResolution; i++) {
			for (unsigned int j = 0; j < volumeData.yResolution; j++) {
				unsigned char* data_gray = static_cast<unsigned char*>(volumeData.data) +
					k * volumeData.xResolution * volumeData.yResolution;
				unsigned int offset = j * volumeData.xResolution + i;
				data_gray[offset] = static_cast<unsigned char>(
					0.299 * dataPoints[k][offset * 3 + 0]
					+ 0.587 * dataPoints[k][offset * 3 + 1]
					+ 0.114 * dataPoints[k][offset * 3 + 2]);

			}
		}

		delete[] dataPoints[k];
	}
}

#undef STB_IMAGE_IMPLEMENTATION




