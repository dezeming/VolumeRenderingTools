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

#include "DataProcess.h"
#include "MainGui/DebugText.hpp"

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

void ProcessVolumeData::process() {

	// DCMTK_Test();
	// GDCM_Test();

	// Dicom to MHD-RAW
	if (0) {	
		DcmMakeMhdFile_DCMTK(
			"D:/Datasets/DicomImages/Heart/", 
			"./Output", 
			"sample");
	}
	if (0) {
		DcmMakeMhdFile_GDCM(
			"E:/Datasets/MaxinData/3D_AX+C_Series0010/",
			"./Output", 
			"PT096");
	}

	// some test
	if (0) {
		for (int i = 1; i < 486; i++) {
			QString inputfolderPath = "S:/CT-DicomDatasets/Cerebral_aneurysm/AIneurysm/R"
				+ QString::number(i / 100) + QString::number(i % 100 / 10) +QString::number(i % 10) + "/";

			QString outputfolderPath = "S:/CT-DicomDatasets/GeneratedMhdData/R"
				+ QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10) + "/";

			QDir dir;
			if (!dir.exists(outputfolderPath)) {
				if (dir.mkpath(outputfolderPath)) {
					emit PrintString("Folder created successfully."); 
				}
				else {
					emit PrintString("Folder creation failed.");
					continue;
				}
			}
			else {
				emit PrintString("Folder already exists.");
			}

			DcmMakeMhdFile_GDCM(
				inputfolderPath,
				outputfolderPath,
				"R" + QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10));

		}

	}


	// down sampling
	if (0) {
		DownSamplingMhdWithInterval(
			"D:/DataSets/OpenScientificVisData-Origin/synthetic_truss_with_five_defects/synthetic_truss.mhd", 
			"./Output", 
			"synthetic_truss_small", 2);
	}

	// MHD clipping
	if (0) {
		double center[3] = { 0.0, 0, 0.0 };
		double bound[3] = { 0.212, 0.5, 0.5 };
		MhdClip(
			"D:/DataSets/OpenScientificVisData/Chameleon/chameleon.mhd", 
			"Chameleon_small", 
			"./Output",
			center, bound);
	}

	// MHD Rotate according to axis
	if (0) {
		int permute[3] = {0,2,1};
		MhdRotateAxis(
			"E:/DataSets/DcmToolsTest/ssas/mhd_abdomen.mhd", 
			"E:/DataSets/DcmToolsTest/dddd/",
			"bunny_r", 
			permute);
	}

	// MHD flips up, down, left, and right based on the selected axis
	if (0) {
		int flip[3] = { 0,1,0 };
		MhdFlipAxis(
			"E:/DataSets/DcmToolsTest/mhd_abdomen.mhd", 
			"E:/DataSets/DcmToolsTest/dddd/",
			"sample", 
			flip);
	}

	// MHD to Feimos format
	if (0) {
		GenerateFormat generateFormat;
		MhdGenerateFeimosData(
			"sample.mhd", 
			"./Output", 
			"sample", generateFormat);
	}
}



// **********************************************//
// *** Check input/output legitimacy *** //
// **********************************************//

bool ProcessVolumeData::isInputDirExist(const QString& inputDir) {
	QDir dir;
	if (!dir.exists(inputDir)) {
		emit PrintError("Input Folder does not exist.");
		return false;
	}
	return true;
}
bool ProcessVolumeData::isInputFileExist(const QString& inputFilePath, const QString& suffix) {
	if (suffix != "") {
		if (!inputFilePath.endsWith(suffix, Qt::CaseInsensitive)) {
			emit PrintError("Suffix does not meet the requirements for input files.");
			return false;
		}
	}
	QFile file_input(inputFilePath);
	if (!file_input.exists()) {
		emit PrintError("Input file does not exist.");
		return false;
	}
	return true;
}
bool ProcessVolumeData::isInputMhdFileExist(const QString& inputFilePath) {
	return isInputFileExist(inputFilePath, ".mhd");
}
bool ProcessVolumeData::isInputFeimosFileExist(const QString& inputFilePath) {
	return isInputFileExist(inputFilePath, ".feimos");
}
bool ProcessVolumeData::checkOutputDir_Mhd(const QString& outputDir, const QString& outName) {
	QDir dir;
	if (!dir.exists(outputDir)) {
		if (!dir.mkpath(outputDir)) {
			emit PrintError("Folder creation failed.");
			return false;
		}
	}

	QFile file_mhd(outputDir + "/" + outName + ".mhd");
	if (file_mhd.exists()) {
		emit PrintError("It is mandatory not to overwrite file with the same name.");
		return false;
	}
	QFile file_raw(outputDir + "/" + outName + ".raw");
	if (file_raw.exists()) {
		emit PrintError("It is mandatory not to overwrite file with the same name.");
		return false;
	}

	return true;
}
bool ProcessVolumeData::checkOutputDir_Feimos(const QString& outputDir, const QString& outName) {
	QDir dir;
	if (!dir.exists(outputDir)) {
		if (!dir.mkpath(outputDir)) {
			emit PrintError("Folder creation failed.");
			return false;
		}
	}

	QFile file_feimos(outputDir + "/" + outName + ".feimos");
	if (file_feimos.exists()) {
		emit PrintError("It is mandatory not to overwrite file with the same name.");
		return false;
	}
	QFile file_raw(outputDir + "/" + outName + ".raw");
	if (file_raw.exists()) {
		emit PrintError("It is mandatory not to overwrite file with the same name.");
		return false;
	}

	return true;
}
bool ProcessVolumeData::getInputDcmFileList(const QString& inputDir, std::vector<QString>& fileList) {
	QDir dir(inputDir);
	unsigned int file_count = 0;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			emit PrintError("Fatal: Dir does not exist!");
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
			emit PrintError("No .dcm file exists");
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
// *** Generate input data to ImportFormat object *** //
// **********************************************//


bool DcmFilePosCompare(const DcmFilePixelData &f1, const DcmFilePixelData &f2) {
	if (f1.position < f2.position) return true;
	else return false;
}

bool ProcessVolumeData::GenerateInput_GDCM(const std::vector<QString>& fileList, ImportFormat& importFormat) {
	
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
				emit PrintError(("The height of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				height = height_t;
				importFormat.yResolution = height;
			}
		}

		// Columns
		{
			gdcm::Attribute<0x0028, 0x0011> at_columns;
			at_columns.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0011>::ArrayType v_columns = at_columns.GetValue();
			unsigned int width_t = v_columns;

			if (correctImagesNum.size() != 0 && width != width_t) {
				emit PrintError(("The width of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				width = width_t;
				importFormat.xResolution = width;
			}
		}

		// Spacing between slices
		{
			gdcm::Attribute<0x0018, 0x0050> at_Spacing_slices;
			at_Spacing_slices.SetFromDataSet(ds);
			gdcm::Attribute<0x0018, 0x0050>::ArrayType v_Spacing_slices = at_Spacing_slices.GetValue();

			double pixelSpacing_Z_t = v_Spacing_slices;

			if (correctImagesNum.size() != 0 && pixelSpacing_Z != pixelSpacing_Z_t) {
				emit PrintError(("The slice thickness of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_Z = pixelSpacing_Z_t;
				importFormat.zPixelSpace = pixelSpacing_Z;
			}
		}

		// Spacing between pixels  Unit:mm
		{
			gdcm::Attribute<0x0028, 0x0030> at_Pixel_Spacing;
			at_Pixel_Spacing.SetFromDataSet(ds);
			gdcm::Attribute<0x0028, 0x0030>::ArrayType vv_Pixel_Spacing = at_Pixel_Spacing.GetValue();

			double pixelSpacing_X_t = vv_Pixel_Spacing;

			if (correctImagesNum.size() != 0 && pixelSpacing_X != pixelSpacing_X_t) {
				emit PrintError(("The pixel spacing of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_X = pixelSpacing_X_t;
				pixelSpacing_Y = pixelSpacing_X_t;
				importFormat.xPixelSpace = pixelSpacing_X;
				importFormat.yPixelSpace = pixelSpacing_Y;
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
				emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
		}

		// format
		{
			gdcm::ImageReader imageReader;
			imageReader.SetFileName(file_Path.toStdString().c_str());
			if (!imageReader.Read()) {
				emit PrintError(("Unable to read image data from file" + file_Path).toStdString().c_str());
				continue;
			}

			gdcm::Image &image = imageReader.GetImage();
			gdcm::PixelFormat pixelFormat = image.GetPixelFormat();

			bool formatFlag = true;
			switch (pixelFormat.GetScalarType())
			{
			case gdcm::PixelFormat::INT8:
				if (correctImagesNum.size() == 0) 
					importFormat.format = Dez_SignedChar;
				else if (importFormat.format != Dez_SignedChar) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT8:
				if (correctImagesNum.size() == 0) 
					importFormat.format = Dez_UnsignedChar;
				else if (importFormat.format != Dez_UnsignedChar) formatFlag = false;
				break;
			case gdcm::PixelFormat::INT16:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_SignedShort;
				else if (importFormat.format != Dez_SignedShort) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT16:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_UnsignedShort;
				else if (importFormat.format != Dez_UnsignedShort) formatFlag = false;
				break;
			case gdcm::PixelFormat::INT32:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_SignedLong;
				else if (importFormat.format != Dez_SignedLong) formatFlag = false;
				break;
			case gdcm::PixelFormat::UINT32:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_UnsignedLong;
				else if (importFormat.format != Dez_UnsignedLong) formatFlag = false;
				break;
			case gdcm::PixelFormat::FLOAT32:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_Float;
				else if (importFormat.format != Dez_Float) formatFlag = false;
				break;
			case gdcm::PixelFormat::FLOAT64:
				if (correctImagesNum.size() == 0)
					importFormat.format = Dez_Double;
				else if (importFormat.format != Dez_Double) formatFlag = false;
				break;
			default:
				emit PrintError(("Unknown pixel data type in File " + file_Path).toStdString().c_str());
				formatFlag = false;
				break;
			}

			if (!formatFlag) {
				emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
				
		}

		correctImagesNum.push_back(i);
	}

	if (correctImagesNum.size() == 0) {
		emit PrintError("No suitable DCM images to read");
		return false;
	}

	importFormat.zResolution = correctImagesNum.size();
	imageNum = correctImagesNum.size();

	// check data format
	unsigned int bytesOnPixel = 0;
	{
		if (importFormat.format == Dez_Float) {
			bytesOnPixel = sizeof(float);
		}
		else if (importFormat.format == Dez_Double) {
			bytesOnPixel = sizeof(double);
		}
		else if (importFormat.format == Dez_UnsignedLong) {
			bytesOnPixel = sizeof(unsigned long);
		}
		else if (importFormat.format == Dez_SignedLong) {
			bytesOnPixel = sizeof(long);
		}
		else if (importFormat.format == Dez_UnsignedShort) {
			if (bits_perPixel != 16) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned short);
		}
		else if (importFormat.format == Dez_SignedShort) {
			if (bits_perPixel != 16) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(short);
		}
		else if (importFormat.format == Dez_UnsignedChar) {
			if (bits_perPixel != 8) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned char);
		}
		else if (importFormat.format == Dez_SignedChar) {
			if (bits_perPixel != 8) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(char);
		}
		else {
			emit PrintError("Temporarily unsupported data format: error format");
			return false;
		}
	}

	// Print import data Info
	emit PrintString(importFormat.toString().toStdString().c_str());

	char * m_data = new char[width * height * imageNum * bytesOnPixel];
	if (!m_data) {
		emit PrintError("Unable to request sufficient amount of memory!");
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
			emit PrintError(("Unable to read image data from File " + file_Path).toStdString().c_str());
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

		importFormat.data = new char[width * height * imageNum * bytesOnPixel];
		if (!importFormat.data) {
			emit PrintError("Unable to request sufficient amount of memory!");
			readFlag = false;
		}

		if (readFlag)
			for (int i = 0; i < imageNum; i++) {
				if (dcmFileVec[i].pixData == nullptr) {
					emit PrintError("(dcmFileVec[i].pixData16 == nullptr) ");
					readFlag = false;
					break;
				}
				memcpy((char*)importFormat.data + i * width * height * bytesOnPixel, 
					dcmFileVec[i].pixData, 
					width * height * bytesOnPixel);
			}
	}

	delete[] m_data;
	return readFlag;
}

bool ProcessVolumeData::GenerateInput_DCMTK(const std::vector<QString>& fileList, ImportFormat& importFormat) {

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
			emit PrintError(("No Metalnfo in File " + file_Path).toStdString().c_str());
			continue;
		}

		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			emit PrintError(("No data in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Pixel Data
		DcmElement *element = NULL;
		if (!data->findAndGetElement(DCM_PixelData, element).good()) {
			emit PrintError(("Get PixelData wrong in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Width Info
		{
			OFString ImageWidth;
			if (!data->findAndGetOFString(DCM_Columns, ImageWidth).good()) {
				emit PrintError(("Get DCM_Columns wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && width != static_cast<unsigned int>(atoi(ImageWidth.data()))) {
				emit PrintError(("The width of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				width = static_cast<unsigned int>(atoi(ImageWidth.data()));
				importFormat.xResolution = width;
			}
		}

		// Get Height Info
		{
			OFString ImageHeight;
			if (!data->findAndGetOFString(DCM_Rows, ImageHeight).good()) {
				emit PrintError(("Get DCM_Rows wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && height != static_cast<unsigned int>(atoi(ImageHeight.data()))) {
				emit PrintError(("The height of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				height = static_cast<unsigned int>(atoi(ImageHeight.data()));
				importFormat.yResolution = height;
			}
		}

		// Get Pixel Spacing
		{
			OFString PixelSpacing;
			if (!data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing).good()) {
				emit PrintError(("Get DCM_PixelSpacing wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && pixelSpacing_X != atof(PixelSpacing.data())) {
				emit PrintError(("The pixelSpacing of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_X = atof(PixelSpacing.data());
				pixelSpacing_Y = pixelSpacing_X;
				importFormat.xPixelSpace = pixelSpacing_X;
				importFormat.yPixelSpace = pixelSpacing_Y;
			}
		}

		// Get Slice Thickness
		{
			OFString SliceThickness;
			if (!data->findAndGetOFString(DCM_SliceThickness, SliceThickness).good()) {
				emit PrintError(("Get DCM_SliceThickness wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (i != 0 && pixelSpacing_Z != atof(SliceThickness.data())) {
				emit PrintError(("The SliceThickness of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_Z = atof(SliceThickness.data());
				importFormat.zPixelSpace = pixelSpacing_Z;
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
					emit PrintDataD("bitsAllocated", bitsAllocated);
					emit PrintDataD("bitsStored", bitsStored);
					emit PrintDataD("pixelRepresentation", pixelRepresentation);
				}
				else 
				{
					emit PrintError(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
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
						emit PrintError(("The bits allocated or stored is inconsistent in File" + file_Path).toStdString().c_str());
						continue;
					}
				}
				else 
				{
					emit PrintError(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
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
					if (correctImagesNum.size() == 0) importFormat.format = Dez_Float;
					else if (importFormat.format != Dez_Float) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// Double
				else if (valueRepresentation == EVR_FD) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_Double;
					else if (importFormat.format != Dez_Double) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// signed short
				else if (valueRepresentation == EVR_SS) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_SignedShort;
					else if (importFormat.format != Dez_SignedShort) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// unsigned short
				else if (valueRepresentation == EVR_US) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_UnsignedShort;
					else if (importFormat.format != Dez_UnsignedShort) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// signed long
				else if (valueRepresentation == EVR_SL) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_SignedLong;
					else if (importFormat.format != Dez_SignedLong) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				// unsigned long
				else if (valueRepresentation == EVR_UL) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_UnsignedLong;
					else if (importFormat.format != Dez_UnsignedLong) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
						continue;
					}
				}
				else {
					emit PrintError(("Temporarily unsupported data format " + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				emit PrintError(("Get Pixel Representation wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
		}

		correctImagesNum.push_back(i);
	}

	if (correctImagesNum.size() == 0) {
		emit PrintError("No suitable DCM images to read");
		return false;
	}

	// check data format
	unsigned int bytesOnPixel = 0;
	{
		if (importFormat.format == Dez_Float) {
			bytesOnPixel = sizeof(float);
		}
		else if (importFormat.format == Dez_Double) {
			bytesOnPixel = sizeof(double);
		}
		else if (importFormat.format == Dez_UnsignedLong) {
			bytesOnPixel = sizeof(unsigned long);
		}
		else if (importFormat.format == Dez_SignedLong) {
			bytesOnPixel = sizeof(long);
		}
		else if (importFormat.format == Dez_UnsignedShort) {
			if (bitsAllocated != 16) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(unsigned short);
		}
		// When using DCMTK, there may be a mismatch between signed and unsigned values. 
		// It is reasonable to use pixelRepresentation and bitsStored to determine the accurate result, 
		// which is consistent with GDCM parsing. The results determined by getVR() are inconsistent.
		else if (importFormat.format == Dez_SignedShort) {
			if (bitsAllocated != 16) {
				emit PrintError("Mismatched image format");
				return false;
			}
			bytesOnPixel = sizeof(short);
		}
		else if (importFormat.format == Dez_UnsignedChar) {
			emit PrintError("Temporarily unsupported data format: unsigend char");
			return false;
		}
		else if (importFormat.format == Dez_SignedChar) {
			emit PrintError("Temporarily unsupported data format: char");
			return false;
		}
		else {
			emit PrintError("Temporarily unsupported data format: error format");
			return false;
		}
	}

	unsigned int imageNum = correctImagesNum.size();
	importFormat.zResolution = imageNum;

	// Print import data Info
	emit PrintString(importFormat.toString().toStdString().c_str());

	void * m_data = new char[width * height * imageNum * bytesOnPixel];
	if (!m_data) {
		emit PrintError("Unable to request sufficient amount of memory!");
		return false;
	}

	// Start processing each image
	bool readDataFlag = true;
	std::vector<DcmFilePixelData> dcmFileVec;
	for (int i = 0; i < imageNum; i++) {
		// File Path
		QString file_Path = fileList[correctImagesNum[i]];

		// DcmFileFormat will automatically release memory
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
		if (!Metalnfo) {
			emit PrintError(("No Metalnfo in File " + file_Path).toStdString().c_str());
			readDataFlag = false;
			break;
		}
		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			emit PrintError(("No data in File " + file_Path).toStdString().c_str());
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

		if (importFormat.format == Dez_UnsignedShort) {
			Uint16* dat_t;
			element->getUint16Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Uint16));
		}
		else if (importFormat.format == Dez_SignedShort) {
			Sint16* dat_t;
			element->getSint16Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Sint16));
		}
		else if (importFormat.format == Dez_Float) {
			Float32* dat_t;
			element->getFloat32Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Float32));
		}
		else if (importFormat.format == Dez_Double) {
			Float64* dat_t;
			element->getFloat64Array(dat_t);
			memcpy(static_cast<char*>(m_data) + i * width * height * bytesOnPixel, dat_t, width * height * sizeof(Float64));
		}
		else {
			emit PrintError("Temporarily unsupported data format!");
			readDataFlag = false;
		}

		DcmFilePixelData dcmF;
		dcmF.pixData = static_cast<char*>(m_data) + i * width * height * bytesOnPixel;
		dcmF.position = atof(ImagePos.data());
		dcmFileVec.push_back(dcmF);
	}

	// sort images by their positions and save to importFormat.data
	if (readDataFlag) {
		// Sort by image position
		std::sort(dcmFileVec.begin(), dcmFileVec.end(), DcmFilePosCompare);

		importFormat.data = new char[width * height * imageNum * bytesOnPixel];
		if (!importFormat.data) {
			emit PrintError("Unable to request sufficient amount of memory!");
		}
		else {
			for (int i = 0; i < imageNum; i++) {
				if (dcmFileVec[i].pixData == nullptr) {
					emit PrintError("(dcmFileVec[i].pixData == nullptr) ");
					readDataFlag = false;
					break;
				}
				else {
					memcpy(static_cast<char*>(importFormat.data) + i * width * height * bytesOnPixel,
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
bool ProcessVolumeData::CopyUncompressedRawData(const T* data, ImportFormat& importFormat)
{
	if (!data) return false;
	
	importFormat.data = new T[importFormat.xResolution * importFormat.yResolution * importFormat.zResolution];
	if (!importFormat.data) return false;
	for (int i = 0; i < importFormat.zResolution; i++) {
		memcpy((T*)importFormat.data + i * importFormat.xResolution * importFormat.yResolution,
			(T*)data + i * importFormat.xResolution * importFormat.yResolution, 
			importFormat.xResolution * importFormat.yResolution * sizeof(T));
	}
	return true;
}
 
bool ProcessVolumeData::GenerateInput_Mhd(const QString& inputFilePath, ImportFormat& importFormat) {

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(inputFilePath.toStdString().c_str());
	reader->Update();
	
	vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();
	int dataType = imageData->GetScalarType();

	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData(imageData);
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	importFormat.xResolution = volumeData[1] + 1;
	importFormat.yResolution = volumeData[3] + 1;
	importFormat.zResolution = volumeData[5] + 1;

	double* spacing = ImageCast->GetOutput()->GetSpacing();
	importFormat.xPixelSpace = spacing[0];
	importFormat.yPixelSpace = spacing[1];
	importFormat.zPixelSpace = spacing[2];

	bool flag = true;
	const bool DebugCopyInfo = true;
	switch (dataType)
	{
	case VTK_UNSIGNED_CHAR:
		importFormat.format = Dez_UnsignedChar;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned char*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy unsigned char data from Mhd to array");
		break;
	case VTK_SIGNED_CHAR:
		importFormat.format = Dez_SignedChar;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed char*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy signed char data from Mhd to array");
		break;
	case VTK_UNSIGNED_SHORT:
		importFormat.format = Dez_UnsignedShort;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned short*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy unsigned short data from Mhd to array");
		break;
	case VTK_SHORT:
		importFormat.format = Dez_SignedShort;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed short*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy signed short data from Mhd to array");
		break;
	case VTK_UNSIGNED_INT:
		importFormat.format = Dez_UnsignedLong;
		flag = flag && CopyUncompressedRawData(
			static_cast<unsigned int*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy unsigned long data from Mhd to array");
		break;
	case VTK_INT:
		importFormat.format = Dez_SignedLong;
		flag = flag && CopyUncompressedRawData(
			static_cast<signed int*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy signed long data from Mhd to array");
		break;
	case VTK_FLOAT:
		importFormat.format = Dez_Float;
		flag = flag && CopyUncompressedRawData(
			static_cast<float*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy [float] data from Mhd to array");
		break;
	case VTK_DOUBLE:
		importFormat.format = Dez_Double;
		flag = flag && CopyUncompressedRawData(
			static_cast<double*>(ImageCast->GetOutput()->GetScalarPointer()),
			importFormat);
		if (DebugCopyInfo)
			emit PrintString("Copy [double] data from Mhd to array");
		break;
	default:
		emit PrintError("Failed to read data from Mhd file.");
		flag = false;
		break;
	}

	// I don't know why this version of the smart pointer cannot properly release memory, 
	// so I manually released it here
	ImageCast->Delete();

	return flag;
}

bool ReadUncompressedRawData(std::string filename, unsigned int bytesOneScalar, ImportFormat& importFormat) {
	unsigned int width = importFormat.xResolution, height = importFormat.yResolution, imageNUm = importFormat.zResolution;

	importFormat.data = new char[importFormat.xResolution * importFormat.yResolution * importFormat.zResolution * bytesOneScalar];
	if (!importFormat.data) return false;

	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;
	try {
		file.read(reinterpret_cast<char*>(importFormat.data), bytesOneScalar * width * height * imageNUm);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return false;
	}

	file.close();
	return true;
}

bool ProcessVolumeData::GenerateInput_Feimos(const QString& inputFilePath, ImportFormat& importFormat) {

	std::ifstream file(inputFilePath.toStdString());
	std::string name;
	std::string rawDataFileName;

	while (file >> name) {
		QString qName = name.c_str();
		if (qName.compare("xResolution", Qt::CaseInsensitive) == 0) {
			file >> importFormat.xResolution;
		}
		else if (qName.compare("yResolution", Qt::CaseInsensitive) == 0) {
			file >> importFormat.yResolution;
		}
		else if (qName.compare("zResolution", Qt::CaseInsensitive) == 0) {
			file >> importFormat.zResolution;
		}
		else if (qName.compare("xPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> importFormat.xPixelSpace;
		}
		else if (qName.compare("yPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> importFormat.yPixelSpace;
		}
		else if (qName.compare("zPixelSpace", Qt::CaseInsensitive) == 0) {
			file >> importFormat.zPixelSpace;
		}
		else if (qName.compare("format", Qt::CaseInsensitive) == 0) {
			file >> name;
			if (!importFormat.setFormatUsingString(name.c_str())) {
				emit PrintError("Non compliant format input when parsing (.feimos) file!");
				return false;
			}
		}
		else if (qName.compare("Raw", Qt::CaseInsensitive) == 0) {
			std::getline(file, name);
			name.erase(name.begin());
			rawDataFileName = name;
		}
	}

	size_t dataSize = (size_t)importFormat.xResolution * (size_t)importFormat.yResolution * (size_t)importFormat.zResolution;
	if (dataSize > (size_t)1024 * (size_t)1024 * (size_t)1024) {
		emit PrintError("The image size read in is too large and currently not supported. Please use other reading functions.");
		return false;
	}

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	if (!isInputFileExist(absoluteDirPath + "/" + rawDataFileName.c_str())) {
		emit PrintError("Raw data does not exist when parsing (.feimos) file!");
		return false;
	}

	bool readbinaryFlag = true;
	switch (importFormat.format)
	{
	case Dez_Origin:
		emit PrintError("Non compliant data format");
		return false;
		break;
	case Dez_UnsignedLong:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned long), importFormat);
		break;
	case Dez_SignedLong:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed long), importFormat);
		break;
	case Dez_UnsignedShort:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned short), importFormat);
		break;
	case Dez_SignedShort:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed short), importFormat);
		break;
	case Dez_UnsignedChar:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(unsigned char), importFormat);
		break;
	case Dez_SignedChar:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(signed char), importFormat);
		break;
	case Dez_Float:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(float), importFormat);
		break;
	case Dez_Double:
		readbinaryFlag = readbinaryFlag &&
			ReadUncompressedRawData(
			(absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), sizeof(double), importFormat);
		break;
	default:
		emit PrintError("Non compliant data output format");
		return false;
		break;
	}

	if (!readbinaryFlag) {
		emit PrintError("Fail to Read to binary file");
		return false;
	}

	return true;
}


// ********************************************** //
// *** Process input data *** //
// ********************************************** //

bool ProcessVolumeData::RotateAxis(ImportFormat& importFormat, int permute[3]) {
	return false;
}
bool ProcessVolumeData::FlipAxis(ImportFormat& importFormat, int flip[3]) {
	return false;
}
bool ProcessVolumeData::Clip(ImportFormat& importFormat, double center[3], double bound[3]) {
	return false;
}
bool ProcessVolumeData::DownSamplingWithInterval(ImportFormat& importFormat, int Interval) {
	return false;
}
bool ProcessVolumeData::Resize(ImportFormat& importFormat, float scale) {
	return false;
}


// ********************************************** //
// *** Generate output data *** //
// ********************************************** //

template <typename T, typename U>
bool __DataFormatConvert(const T* data, U* aimdata, ImportFormat& importFormat) {
	unsigned int width = importFormat.xResolution, height = importFormat.yResolution, images = importFormat.zResolution;

	aimdata = new U[width * height * images];
	if (!aimdata) return false;

	for (unsigned int k = 0; k < importFormat.zResolution; k++) {

		T* imageP = (T *)data + k * width * height;
		U* imageAimP = (U *)aimdata + k * width * height;

		for (unsigned int i = 0; i < importFormat.xResolution; i++) {
			for (unsigned int j = 0; j < importFormat.yResolution; j++) {
				imageAimP[i + j * importFormat.xResolution] = 
					static_cast<U>(imageP[i + j * importFormat.xResolution]);
			}
		}
	}
	return true;
}

bool ProcessVolumeData::DataFormatConvert(const GenerateFormat& generateFormat, ImportFormat& importFormat) {

	// To ensure data accuracy, only a small number of conversions are supported.

	if (!importFormat.data) {
		emit PrintError("importFormat has no data");
		return false;
	}

	if (generateFormat.format == Dez_Origin) {
		importFormat.data_aim = importFormat.data;
		importFormat.format_aim = importFormat.format;
		emit PrintString("No need to convert data format.");
		return true;
	}
	else {
		emit PrintString("Need to convert data format.");
	}

	bool convertFlag = true;
	if (importFormat.format == Dez_SignedShort && generateFormat.format == Dez_Float) {
		
		convertFlag = convertFlag && 
			__DataFormatConvert(
				static_cast<signed short*>(importFormat.data),
				static_cast<float*>(importFormat.data_aim),
				importFormat);
		return convertFlag;
	}

	if (importFormat.format == Dez_UnsignedShort && generateFormat.format == Dez_Float) {
		
		convertFlag = convertFlag &&
			__DataFormatConvert(
				static_cast<unsigned short*>(importFormat.data),
				static_cast<float*>(importFormat.data_aim),
				importFormat);
		return convertFlag;
	}

	emit PrintError("Unsupported conversion");
	return false;
}

bool ProcessVolumeData::GenerateOutput_Mhd(const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat, ImportFormat& importFormat) {

	if (!DataFormatConvert(generateFormat, importFormat)) {
		return false;
	}

	int dimM[3] = { importFormat.xResolution, importFormat.yResolution, importFormat.zResolution };
	double PixelSpace[3] = { importFormat.xPixelSpace, importFormat.yPixelSpace, importFormat.zPixelSpace};

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace[0], PixelSpace[1], PixelSpace[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM[0] - 1, 0, dimM[1] - 1, 0, dimM[2] - 1);
	imageImport->SetDataExtentToWholeExtent();

	switch (importFormat.format_aim)
	{
	case Dez_Origin:
		emit PrintError("Error Mhd output format");
		return false;
		break;
	case Dez_UnsignedLong:
		emit PrintError("Undefined Mhd output format in VTK");
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
		emit PrintError("Undefined Mhd output format in VTK");
		return false;
		break;
	case Dez_Float:
		imageImport->SetDataScalarTypeToFloat();
		break;
	case Dez_Double:
		imageImport->SetDataScalarTypeToDouble();
		break;
	default:
		emit PrintError("Error Mhd output format");
		return false;
		break;
	}
	
	imageImport->SetNumberOfScalarComponents(1); // channel
	imageImport->SetImportVoidPointer(importFormat.data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();


	return true;
}

bool SaveUncompressedRawData(std::string filename, unsigned int bytesOneScalar, const ImportFormat& importFormat) {
	unsigned int width = importFormat.xResolution, height = importFormat.yResolution, imageNUm = importFormat.zResolution;

	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) return false;
	try {
		file.write(reinterpret_cast<const char*>(importFormat.data_aim), bytesOneScalar * width * height * imageNUm);
	}
	catch (const std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return false;
	}

	file.close();
	return true;
}

bool ProcessVolumeData::GenerateOutput_Feimos(const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat, ImportFormat& importFormat) {

	if (!DataFormatConvert(generateFormat, importFormat)) {
		return false;
	}

	std::string format; bool writebinaryFlag = true;
	switch (importFormat.format_aim)
	{
	case Dez_Origin:
		format = "Error";
		emit PrintError("Non compliant data output format");
		return false;
		break;
	case Dez_UnsignedLong:
		format = "UnsignedLong";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned long), importFormat);
		break;
	case Dez_SignedLong:
		format = "SignedLong";
		writebinaryFlag = writebinaryFlag &&
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed long), importFormat);
		break;
	case Dez_UnsignedShort:
		format = "UnsignedShort";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned short), importFormat);
		break;
	case Dez_SignedShort:
		format = "SignedShort";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed short), importFormat);
		break;
	case Dez_UnsignedChar:
		format = "UnsignedChar";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(unsigned char), importFormat);
		break;
	case Dez_SignedChar:
		format = "SignedChar";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(signed char), importFormat);
		break;
	case Dez_Float:
		format = "Float";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(float), importFormat);
		break;
	case Dez_Double:
		format = "Double";
		writebinaryFlag = writebinaryFlag && 
			SaveUncompressedRawData(
			(outputDir + "/" + outName + ".raw").toStdString(), sizeof(double), importFormat);
		break;
	default:
		emit PrintError("Non compliant data output format");
		return false;
		break;
	}
	if (!writebinaryFlag) {
		emit PrintError("Fail to write to binary file");
		return false;
	}

	{
		std::ofstream fileInfo((outputDir + "/" + outName + ".feimos").toStdString());
		if (!fileInfo.is_open()) {
			emit PrintError("Fail to write to info file");
			return false;
		}
		fileInfo << "xResolution " << importFormat.xResolution << std::endl;
		fileInfo << "yResolution " << importFormat.yResolution << std::endl;
		fileInfo << "zResolution " << importFormat.zResolution << std::endl;
		fileInfo << "xPixelSpace " << importFormat.xPixelSpace << std::endl;
		fileInfo << "yPixelSpace " << importFormat.yPixelSpace << std::endl;
		fileInfo << "zPixelSpace " << importFormat.zPixelSpace << std::endl;
		fileInfo << "format " << format << std::endl;
		fileInfo << "Raw " << (outName.toStdString() + ".raw") << std::endl;
		fileInfo.close();
	}
	

	return true;
}

// **********************************************//
// *** Special functions that cannot be performed in steps *** //
// **********************************************//

template <typename T>
inline void set_Array(T* data, T dat, unsigned int dim[3], unsigned int pos[3]) {
	unsigned int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	data[offset] = dat;
}
template <typename T>
inline T at_Array(T* data, unsigned int dim[3], unsigned int pos[3]) {
	unsigned int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	return data[offset];
}

template <typename T>
bool downSampling(T* data, ImportFormat& importFormat, unsigned int dimM[3], unsigned int dim_Down[3], unsigned int Interval) {

	T * data_aim = new T[dim_Down[0] * dim_Down[1] * dim_Down[2]];
	if (!data_aim) return false;
	for (int i = 0; i < dim_Down[0]; i++) {
		for (int j = 0; j < dim_Down[1]; j++) {
			for (int k = 0; k < dim_Down[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i * Interval, j * Interval, k * Interval };
				T dat = at_Array(data, dimM, pos_m);
				set_Array(data_aim, dat, dim_Down, pos_aim);
			}
		}
	}

	delete[] static_cast<T*>(importFormat.data);

	importFormat.data = data_aim;

	importFormat.xResolution = dim_Down[0];
	importFormat.yResolution = dim_Down[1];
	importFormat.zResolution = dim_Down[2];

	importFormat.xPixelSpace = importFormat.xPixelSpace * Interval;
	importFormat.yPixelSpace = importFormat.yPixelSpace * Interval;
	importFormat.zPixelSpace = importFormat.zPixelSpace * Interval;
	return true;
}

void ProcessVolumeData::DownSamplingMhdFile(const QString& inputFilePath, const QString& outputDir, const QString& outName, 
	const GenerateFormat& generateFormat, int Interval) {
	// check input and output
	if (!isInputMhdFileExist(inputFilePath)) return;
	if (!checkOutputDir_Feimos(outputDir, outName)) return;

	ImportFormat importFormat;

	bool parseFlag = GenerateInput_Mhd(inputFilePath, importFormat);
	emit PrintString(importFormat.toString().toStdString().c_str());
	if (!importFormat.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		unsigned int dimM[3] = { importFormat.xResolution, importFormat.yResolution, importFormat.zResolution };
		unsigned int dim_Down[3] = { importFormat.xResolution / Interval, importFormat.yResolution / Interval, importFormat.zResolution / Interval };

		switch (importFormat.format)
		{
		case Dez_Origin:
			emit PrintError("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = downSampling(static_cast<unsigned long*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedLong:
			parseFlag = downSampling(static_cast<signed long*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedShort:
			parseFlag = downSampling(static_cast<unsigned short*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedShort:
			parseFlag = downSampling(static_cast<signed short*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedChar:
			parseFlag = downSampling(static_cast<unsigned char*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedChar:
			parseFlag = downSampling(static_cast<signed char*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_Float:
			parseFlag = downSampling(static_cast<float*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_Double:
			parseFlag = downSampling(static_cast<double*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		default:
			emit PrintError("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}

	if (parseFlag) 
		parseFlag = GenerateOutput_Mhd(outputDir, outName, generateFormat, importFormat);
	else 
		emit PrintError("Downsampling failed");
	
	importFormat.clear();
}

void ProcessVolumeData::DownSamplingFeimosFile(const QString& inputFilePath, const QString& outputDir, const QString& outName, 
	const GenerateFormat& generateFormat, int Interval) {
	// check input and output
	if (!isInputFeimosFileExist(inputFilePath)) return;
	if (!checkOutputDir_Feimos(outputDir, outName)) return;

	ImportFormat importFormat;

	bool parseFlag = GenerateInput_Feimos(inputFilePath, importFormat);
	emit PrintString(importFormat.toString().toStdString().c_str());
	if (!importFormat.data) {
		parseFlag = false;
	}
	if (parseFlag) {
		unsigned int dimM[3] = { importFormat.xResolution, importFormat.yResolution, importFormat.zResolution };
		unsigned int dim_Down[3] = { importFormat.xResolution / Interval, importFormat.yResolution / Interval, importFormat.zResolution / Interval };

		switch (importFormat.format)
		{
		case Dez_Origin:
			emit PrintError("Non compliant image data format");
			parseFlag = false;
			break;
		case Dez_UnsignedLong:
			parseFlag = downSampling(static_cast<unsigned long*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedLong:
			parseFlag = downSampling(static_cast<signed long*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedShort:
			parseFlag = downSampling(static_cast<unsigned short*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedShort:
			parseFlag = downSampling(static_cast<signed short*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_UnsignedChar:
			parseFlag = downSampling(static_cast<unsigned char*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_SignedChar:
			parseFlag = downSampling(static_cast<signed char*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_Float:
			parseFlag = downSampling(static_cast<float*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		case Dez_Double:
			parseFlag = downSampling(static_cast<double*>(importFormat.data), importFormat, dimM, dim_Down, Interval);
			break;
		default:
			emit PrintError("Non compliant image data format");
			parseFlag = false;
			break;
		}
	}

	if (parseFlag) {
		parseFlag = GenerateOutput_Feimos(outputDir, outName, generateFormat, importFormat);
	}
	else {
		emit PrintError("Downsampling failed");
	}

	importFormat.clear();
}

template <typename T>
bool DownSamplingInOneImage(
	const T* data, const unsigned int width, const unsigned int height,
	T* data_aim, const unsigned int resizedWidth, const unsigned int resizedHeight,
	const unsigned int Interval) {

	for (int i = 0; i < resizedWidth; i++) {
		for (int j = 0; j < resizedHeight; j++) {
			unsigned int off_data = (Interval * i) + (Interval * j) * width;
			unsigned int off_data_aim = (i) + (j) * resizedWidth;
			data_aim[off_data_aim] = data[off_data];
		}
	}
	return true;
}

bool ProcessVolumeData::DownSamplingLargeFeimosData(const QString& inputFilePath, const QString& outputDir, const QString& outName,
	unsigned int Interval) {
	// check input and output
	if (!isInputFeimosFileExist(inputFilePath)) return false;
	if (!checkOutputDir_Feimos(outputDir, outName)) return false;

	if (Interval <= 0) {
		emit PrintError("Illegal value of Interval");
		return false;
	}

	std::ifstream file(inputFilePath.toStdString());
	std::string name;
	std::string rawDataFileName;

	ImportFormat importFormat;

	while (file >> name) {

		if (name == "xResolution") {
			file >> importFormat.xResolution;
		}
		else if (name == "yResolution") {
			file >> importFormat.yResolution;
		}
		else if (name == "zResolution") {
			file >> importFormat.zResolution;
		}
		else if (name == "xPixelSpace") {
			file >> importFormat.xPixelSpace;
		}
		else if (name == "yPixelSpace") {
			file >> importFormat.yPixelSpace;
		}
		else if (name == "zPixelSpace") {
			file >> importFormat.zPixelSpace;
		}
		else if (name == "format") {
			file >> name;
			if (!importFormat.setFormatUsingString(name.c_str())) {
				emit PrintError("Non compliant format input when parsing (.feimos) file!");
				return false;
			}
		}
		else if (name == "Raw") {
			std::getline(file, name);
			name.erase(name.begin());
			rawDataFileName = name;
		}
	}

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	if (!isInputFileExist(absoluteDirPath + "/" + rawDataFileName.c_str())) {
		emit PrintError("Raw data does not exist when parsing (.feimos) file!");
		return false;
	}

	unsigned int 
		resizedWidth = importFormat.xResolution / Interval, 
		resizedHeight = importFormat.yResolution / Interval, 
		resizedImageNum = importFormat.zResolution / Interval;

	double
		resizedPixelSpace_x = importFormat.xPixelSpace * Interval,
		resizedPixelSpace_y = importFormat.yPixelSpace * Interval,
		resizedPixelSpace_z = importFormat.zPixelSpace * Interval;

	int bytesOnePixel = importFormat.getFormatOnePixelBytes();
	if (bytesOnePixel <= 0) {
		emit PrintError("Bytes per pixel that do not comply!");
		return false;
	}

	// down sampling
	bool downsamplingFlag = true;
	{
		std::ifstream inputFile((absoluteDirPath + "/" + rawDataFileName.c_str()).toStdString(), std::ios::binary);
		std::ofstream outputFile((outputDir + "/" + outName + ".raw").toStdString(), std::ios::binary);
		if (!inputFile.is_open() || !outputFile.is_open()) {
			emit PrintError("Cannot open raw input and output file!");
			return false;
		}

		const std::streamsize readImageSize = importFormat.xResolution * importFormat.yResolution * (unsigned int)bytesOnePixel;
		void* readBuffer = new char[readImageSize];

		const std::streamsize writeImageSize = resizedWidth * resizedHeight * bytesOnePixel;
		void* writeBuffer = new char[writeImageSize];

		int currentNum = 0;
		int writeImageNum = 0;
		
		while (inputFile.read(static_cast<char*>(readBuffer), readImageSize)) {

			if (0 == currentNum % Interval) {

				switch (importFormat.format)
				{
				case Dez_Origin:
					downsamplingFlag = false;
					break;
				case Dez_UnsignedLong:
					DownSamplingInOneImage(
						static_cast<unsigned long*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<unsigned long*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedLong:
					DownSamplingInOneImage(
						static_cast<signed long*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<signed long*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_UnsignedShort:
					DownSamplingInOneImage(
						static_cast<unsigned short*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<unsigned short*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedShort:
					DownSamplingInOneImage(
						static_cast<signed short*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<signed short*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_UnsignedChar:
					DownSamplingInOneImage(
						static_cast<unsigned char*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<unsigned char*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_SignedChar:
					DownSamplingInOneImage(
						static_cast<char*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<char*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_Float:
					DownSamplingInOneImage(
						static_cast<float*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
						static_cast<float*>(writeBuffer), resizedWidth, resizedHeight,
						Interval);
					break;
				case Dez_Double:
					DownSamplingInOneImage(
						static_cast<double*>(readBuffer), importFormat.xResolution, importFormat.yResolution,
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
			emit PrintError(("The amount of data does not match the file description! resized required:[" + 
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
		std::ofstream fileInfo((outputDir + "/" + outName + ".feimos").toStdString());
		if (!fileInfo.is_open()) {
			emit PrintError("Fail to write to info file");
			return false;
		}
		std::string format = importFormat.getFormatString().toStdString();
		fileInfo << "xResolution " << resizedWidth << std::endl;
		fileInfo << "yResolution " << resizedHeight << std::endl;
		fileInfo << "zResolution " << resizedImageNum << std::endl;
		fileInfo << "xPixelSpace " << resizedPixelSpace_x << std::endl;
		fileInfo << "yPixelSpace " << resizedPixelSpace_y << std::endl;
		fileInfo << "zPixelSpace " << resizedPixelSpace_z << std::endl;
		fileInfo << "format " << format << std::endl;
		fileInfo << "Raw " << (outName.toStdString() + ".raw") << std::endl;
		fileInfo.close();
	}

	return downsamplingFlag;
}


// **********************************************//
// *** Call type function *** //
// **********************************************//

void ProcessVolumeData::DcmMakeMhdFile(const QString& inputDir, const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat) {

	// check input and output
	if (!isInputDirExist(inputDir)) return;
	if (!checkOutputDir_Mhd(outputDir, outName)) return;

	std::vector<QString> fileList;
	bool parseFlag = getInputDcmFileList(inputDir, fileList);
	if (!parseFlag) return;

	ImportFormat importFormat;

	if (generateFormat.parseLib == Dez_GDCM) {
		parseFlag = GenerateInput_GDCM(fileList, importFormat);
	}
	else if (generateFormat.parseLib == Dez_DCMTK) {
		parseFlag = GenerateInput_DCMTK(fileList, importFormat);
	}

	if (parseFlag) {
		parseFlag = GenerateOutput_Mhd(outputDir, outName, generateFormat, importFormat);
	}
	importFormat.clear();
}

void ProcessVolumeData::DcmMakeFeimosFile(const QString& inputDir, const QString& outputDir, const QString& outName, 
	const GenerateFormat& generateFormat) {

	// check input and output
	if (!isInputDirExist(inputDir)) return;
	if (!checkOutputDir_Feimos(outputDir, outName)) return;

	std::vector<QString> fileList;
	bool parseFlag = getInputDcmFileList(inputDir, fileList);
	if (!parseFlag) return;

	ImportFormat importFormat;

	if (generateFormat.parseLib == Dez_GDCM) {
		parseFlag = GenerateInput_GDCM(fileList, importFormat);
	}
	else if (generateFormat.parseLib == Dez_DCMTK) {
		parseFlag = GenerateInput_DCMTK(fileList, importFormat);
	}

	if (parseFlag) {
		parseFlag = GenerateOutput_Feimos(outputDir, outName, generateFormat, importFormat);
	}
	importFormat.clear();
}

void ProcessVolumeData::MhdMakeFeimosFile(const QString& inputFilePath, const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!isInputMhdFileExist(inputFilePath)) return;
	if (!checkOutputDir_Feimos(outputDir, outName)) return;

	ImportFormat importFormat;

	bool parseFlag = GenerateInput_Mhd(inputFilePath, importFormat);
	emit PrintString(importFormat.toString().toStdString().c_str());

	if (parseFlag) {
		parseFlag = GenerateOutput_Feimos(outputDir, outName, generateFormat, importFormat);
	}
	importFormat.clear();
}

void ProcessVolumeData::FeimosMakeMhdFile(const QString& inputFilePath, const QString& outputDir, const QString& outName,
	const GenerateFormat& generateFormat) {
	// check input and output
	if (!isInputFeimosFileExist(inputFilePath)) return;
	if (!checkOutputDir_Mhd(outputDir, outName)) return;

	ImportFormat importFormat;

	bool parseFlag = GenerateInput_Feimos(inputFilePath, importFormat);
	emit PrintString(importFormat.toString().toStdString().c_str());

	if (parseFlag) {
		parseFlag = GenerateOutput_Mhd(outputDir, outName, generateFormat, importFormat);
	}
	importFormat.clear();
}


// **********************************************//
// *** Old Dicom Functions *** //
// **********************************************//

struct DcmFile {
	Uint16 *pixData16 = nullptr;
	float position = 0.0f;
};
bool DataUpSort(const DcmFile &f1, const DcmFile &f2) {
	if (f1.position < f2.position) return true;
	else return false;
}
void ProcessVolumeData::DcmMakeMhdFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName) {
	emit PrintString("Convert .dcm files into (.mhd)-(.raw) format using DCMTK lib.");

	QDir dir(dirPath);
	unsigned int file_count;
	QString _DirPath;

	// load files info
	{
		if (!dir.exists()) {
			emit PrintError("Dir does not exist!");
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
			emit PrintError("file count less than 2");
			return;
		}
		emit PrintDataD("file count", file_count);
		_DirPath = dirPath;
		if (dirPath[dirPath.size() - 1] != '/') _DirPath = _DirPath + "/";
	}

	ImportFormat importFormat;
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
			emit PrintError(("No Metalnfo in File " + file_Path).toStdString().c_str());
			continue;
		}

		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			emit PrintError(("No data in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Pixel Data
		DcmElement *element = NULL;
		if (!data->findAndGetElement(DCM_PixelData, element).good()) {
			emit PrintError(("Get PixelData wrong in File " + file_Path).toStdString().c_str());
			continue;
		}

		// Get Width Info
		{
			OFString ImageWidth;
			if (!data->findAndGetOFString(DCM_Columns, ImageWidth).good()) {
				emit PrintError(("Get DCM_Columns wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && width != static_cast<unsigned int>(atoi(ImageWidth.data()))) {
				emit PrintError(("The width of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				width = static_cast<unsigned int>(atoi(ImageWidth.data()));
				importFormat.xResolution = width;
			}
		}

		// Get Height Info
		{
			OFString ImageHeight;
			if (!data->findAndGetOFString(DCM_Rows, ImageHeight).good()) {
				emit PrintError(("Get DCM_Rows wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && height != static_cast<unsigned int>(atoi(ImageHeight.data()))) {
				emit PrintError(("The height of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				height = static_cast<unsigned int>(atoi(ImageHeight.data()));
				importFormat.yResolution = height;
			}
		}

		// Get Pixel Spacing
		{
			OFString PixelSpacing;
			if (!data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing).good()) {
				emit PrintError(("Get DCM_PixelSpacing wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (correctImagesNum.size() != 0 && pixelSpacing_X != atof(PixelSpacing.data())) {
				emit PrintError(("The pixelSpacing of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_X = atof(PixelSpacing.data());
				pixelSpacing_Y = pixelSpacing_X;
				importFormat.xResolution = pixelSpacing_X;
				importFormat.yResolution = pixelSpacing_Y;
			}
		}

		// Get Slice Thickness
		{
			OFString SliceThickness;
			if (!data->findAndGetOFString(DCM_SliceThickness, SliceThickness).good()) {
				emit PrintError(("Get DCM_SliceThickness wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
			if (i != 0 && pixelSpacing_Z != atof(SliceThickness.data())) {
				emit PrintError(("The SliceThickness of the image sequence is inconsistent in File " + file_Path).toStdString().c_str());
				continue;
			}
			else if (correctImagesNum.size() == 0) {
				pixelSpacing_Z = atof(SliceThickness.data());
				importFormat.zResolution = pixelSpacing_Z;
			}
		}

		// Get Bit Allocated
		{
			Uint16 bitsAllocated_temp, bitsStored_temp;
			if (correctImagesNum.size() == 0) {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored).good()) {
					emit PrintDataD("bitsAllocated_temp", bitsAllocated_temp);
					emit PrintDataD("bitsStored_temp", bitsStored_temp);
				}
				else {
					emit PrintError(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				if (data->findAndGetUint16(DCM_BitsAllocated, bitsAllocated).good() &&
					data->findAndGetUint16(DCM_BitsStored, bitsStored).good()) {
					if (bitsAllocated_temp != bitsAllocated || bitsStored_temp != bitsStored) {
						emit PrintError(("The bits allocated or stored is inconsistent in File" + file_Path).toStdString().c_str());
						continue;
					}
				}
				else {
					emit PrintError(("Unable to obtain bits information for data storage in File" + file_Path).toStdString().c_str());
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
					if (correctImagesNum.size() == 0) importFormat.format = Dez_Float;
					else if (importFormat.format != Dez_Float) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// Double
				else if (valueRepresentation == EVR_FD) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_Double;
					else if (importFormat.format != Dez_Double) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// signed short
				else if (valueRepresentation == EVR_SS) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_SignedShort;
					else if (importFormat.format != Dez_SignedShort) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// unsigned short
				else if (valueRepresentation == EVR_US) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_UnsignedShort;
					else if (importFormat.format != Dez_UnsignedShort) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// signed long
				else if (valueRepresentation == EVR_SL) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_SignedLong;
					else if (importFormat.format != Dez_SignedLong) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				// unsigned long
				else if (valueRepresentation == EVR_UL) {
					if (correctImagesNum.size() == 0) importFormat.format = Dez_UnsignedLong;
					else if (importFormat.format != Dez_UnsignedLong) {
						emit PrintError(("The value representation is inconsistent in File " + file_Path).toStdString().c_str());
					}
				}
				else {
					emit PrintError(("Temporarily unsupported data format " + file_Path).toStdString().c_str());
					continue;
				}
			}
			else {
				emit PrintError(("Get Pixel Representation wrong in File " + file_Path).toStdString().c_str());
				continue;
			}
		}

		correctImagesNum.push_back(i);
	}

	unsigned int imageNum = correctImagesNum.size();

	// Print import data Info
	emit PrintString(importFormat.toString().toStdString().c_str());

	Uint16 * m_data = new Uint16[width * height * imageNum];
	if (!m_data) {
		emit PrintError("Unable to request sufficient amount of memory!");
		return;
	}

	// Start processing each image
	std::vector<DcmFile> dcmFileVec;
	for (int i = 0; i < imageNum; i++) {
		// File Path
		QString file_Path = _DirPath + dir[correctImagesNum[i]];

		// DcmFileFormat will automatically release memory
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
		if (!Metalnfo) {
			emit PrintError(("No Metalnfo in File " + file_Path).toStdString().c_str());
			break;
		}
		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			emit PrintError(("No data in File " + file_Path).toStdString().c_str());
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
		emit PrintError("Unable to request sufficient amount of memory!");
		return;
	}
	bool flag = true;
	for (int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			emit PrintError("(dcmFileVec[i].pixData16 == nullptr) ");
			flag = false;
		}
	}
	if (flag) {

	}

	for (int i = 0; i < imageNum; i++) {
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
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
void ProcessVolumeData::DcmMakeMhdFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName) {
	emit PrintString("Convert .dcm files into (.mhd)-(.raw) format using GDCM lib.");
	QDir dir(dirPath);
	if (!dir.exists()) {
		emit PrintError("Dir does not exist!");
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
		emit PrintError("file count less than 2");
		return;
	}
	emit PrintDataD("file_count : ", file_count);

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
		emit PrintError("Unable to request sufficient amount of memory!");
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
		emit PrintError("Unable to request sufficient amount of memory!");
		return;
	}
	bool flag = false;
	for (int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			emit PrintError("(dcmFileVec[i].pixData16 == nullptr) ");
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] aim_data;
}

// **********************************************//
// *** Dicom Functions *** //
// **********************************************//

void ProcessVolumeData::MhdRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]) {
	emit PrintString("Permute axis of (.mhd)-(.raw) file.");
	if (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == permute[1]) {
		emit PrintError("Error in MHDRotateAxis: (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == flip[1])");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (permute[i] > 2 || permute[i] < 0) {
			emit PrintError("Error in MHDRotateAxis: (permute[i] > 2 || permute[i] < 0)");
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;
}

void ProcessVolumeData::MhdFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]) {
	emit PrintString("Flip axis of (.mhd)-(.raw) file.");
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_m;
}


void ProcessVolumeData::MhdClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]) {
	emit PrintString("Clip (.mhd)-(.raw) file.");
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
	for (int i = 0; i < dimLength[0]; i++) {
		for (int j = 0; j < dimLength[1]; j++) {
			for (int k = 0; k < dimLength[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i + xRange[0],j + yRange[0],k + zRange[0] };
				short dat = at_Array(data_m, dimM, pos_m);
				set_Array(data_aim, dat, dimLength, pos_aim);
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();

	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;
}

void ProcessVolumeData::DownSamplingMhdWithInterval(const QString& filePath, const QString& outputDir, const QString& outName, int Interval) {
	emit PrintString("Resize (.mhd)-(.raw) file.");

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
	for (int i = 0; i < dimLength[0]; i++) {
		for (int j = 0; j < dimLength[1]; j++) {
			for (int k = 0; k < dimLength[2]; k++) {
				unsigned int pos_aim[3] = { i,j,k };
				unsigned int pos_m[3] = { i * Interval, j * Interval, k * Interval };
				short dat = at_Array(data_m, dimM, pos_m);
				set_Array(data_aim, dat, dimLength, pos_aim);
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
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;


}

void ProcessVolumeData::MhdGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName, 
	const GenerateFormat& generateFormat) {
	emit PrintString("Convert (.mhd)-(.raw) file into Feimos file format.");
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

	std::ofstream fileInfo((outputDir + "/" + outName + ".Info" ).toStdString());
	{
		fileInfo << "X_Length " << width << std::endl;
		fileInfo << "Y_Length " << height << std::endl;
		fileInfo << "Z_Length " << imageNum << std::endl;
		fileInfo << "X_Space " << PixelSpace_x << std::endl;
		fileInfo << "Y_Space " << PixelSpace_y << std::endl;
		fileInfo << "Z_Space " << PixelSpace_z << std::endl;
	}
	fileInfo.close();

	std::ofstream file((outputDir + "/" + outName + ".data").toStdString(), std::ios::binary);
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

	emit PrintDataD("Write Finished! ", 0);
	delete[] data_m;

}

void ProcessVolumeData::MhdGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName) {

	emit PrintError("This function has not been implemented yet.");
}


// **********************************************//
// *** Some simple test code *** //
// **********************************************//
void ProcessVolumeData::DCMTK_Test() {

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
void ProcessVolumeData::GDCM_Test() {
	/*
	gdcm::Reader reader;
	reader.SetFileName("Images/IM-0001-0001.dcm");
	if (!reader.Read())
	{
		emit PrintDataD("(!reader.Read()) : ", 1);
		return;
	}
	else {
		emit PrintDataD("(success reader.Read()) : ", 0);
	}
	*/


}












