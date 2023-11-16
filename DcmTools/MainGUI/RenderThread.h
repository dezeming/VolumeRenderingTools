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

#ifndef __RenderThread_h__
#define __RenderThread_h__

#include <QThread>

#include <QString>
#include <QStringList>

#include <vector>

#include "Core\FrameBuffer.h"
#include "IMAGraphicsView.h"

class RenderThread :public QThread {

	Q_OBJECT
public:
	RenderThread();
	~RenderThread();

public:
	bool renderFlag;
	bool paintFlag;
	FrameBuffer* p_framebuffer;

signals:
	void PrintString(const char* s);
	void PrintDataD(const char* s, const double data);

	void PrintError(const char* s);
	void PrintWarning(const char* s);

	void PaintBuffer(DcmTools::Uchar* buffer, int width, int height, int channals);

private slots:
	

public:
	void run();


};

enum DataFormat {
	Dez_Origin = 0,
	Dez_UnsignedLong = 1,
	Dez_SignedLong = 2,
	Dez_UnsignedShort = 3,
	Dez_SignedShort = 4,
	Dez_UnsignedChar = 5,
	Dez_SignedChar = 6,
	Dez_Float = 7,
	Dez_Double = 8
};

enum DcmParseLib {
	Dez_GDCM = 0,
	Dez_DCMTK = 1
};

struct DcmFilePixelData {
	char *pixData = nullptr;
	float position = 0.0f;
};

struct ImportFormat {
	unsigned int xLength, yLength, zLength;
	float xPixelSpace, yPixelSpace, zPixelSpace;

	void * data;
	DataFormat format;

	// If the data format needs to be converted, use this pointer
	void * data_aim;
	DataFormat format_aim;

	ImportFormat() {
		data = nullptr;
		data_aim = nullptr;
		format = Dez_Origin;
	}
	~ImportFormat() {
		clear();
	}
	void clear() {
		if (data) {
			switch (format)
			{
			case Dez_Origin:
				break;
			case Dez_UnsignedLong:
				delete[] static_cast<unsigned long*>(data);
				break;
			case Dez_SignedLong:
				delete[] static_cast<signed long*>(data);
				break;
			case Dez_UnsignedShort:
				delete[] static_cast<unsigned short*>(data);
				break;
			case Dez_SignedShort:
				delete[] static_cast<signed short*>(data);
				break;
			case Dez_UnsignedChar:
				delete[] static_cast<unsigned char*>(data);
				break;
			case Dez_SignedChar:
				delete[] static_cast<signed char*>(data);
				break;
			case Dez_Float:
				delete[] static_cast<float*>(data);
				break;
			case Dez_Double:
				delete[] static_cast<double*>(data);
				break;
			default:
				break;
			}
		}
		data = nullptr;

		if (data_aim && format_aim != format) {
			switch (format_aim)
			{
			case Dez_Origin:
				break;
			case Dez_UnsignedLong:
				delete[] static_cast<unsigned long*>(data_aim);
				break;
			case Dez_SignedLong:
				delete[] static_cast<signed long*>(data_aim);
				break;
			case Dez_UnsignedShort:
				delete[] static_cast<unsigned short*>(data_aim);
				break;
			case Dez_SignedShort:
				delete[] static_cast<signed short*>(data_aim);
				break;
			case Dez_UnsignedChar:
				delete[] static_cast<unsigned char*>(data_aim);
				break;
			case Dez_SignedChar:
				delete[] static_cast<signed char*>(data_aim);
				break;
			case Dez_Float:
				delete[] static_cast<float*>(data_aim);
				break;
			case Dez_Double:
				delete[] static_cast<double*>(data_aim);
				break;
			default:
				break;
			}
		}
		data_aim = nullptr;
	}

	void clearAim() {
		if (data_aim && format_aim != format) {
			switch (format_aim)
			{
			case Dez_Origin:
				break;
			case Dez_UnsignedLong:
				delete[] static_cast<unsigned long*>(data_aim);
				break;
			case Dez_SignedLong:
				delete[] static_cast<signed long*>(data_aim);
				break;
			case Dez_UnsignedShort:
				delete[] static_cast<unsigned short*>(data_aim);
				break;
			case Dez_SignedShort:
				delete[] static_cast<signed short*>(data_aim);
				break;
			case Dez_UnsignedChar:
				delete[] static_cast<unsigned char*>(data_aim);
				break;
			case Dez_SignedChar:
				delete[] static_cast<signed char*>(data_aim);
				break;
			case Dez_Float:
				delete[] static_cast<float*>(data_aim);
				break;
			case Dez_Double:
				delete[] static_cast<double*>(data_aim);
				break;
			default:
				break;
			}
		}
		data_aim = nullptr;
	}

	bool setFormatUsingString(QString str) {
		if (str == "UnsignedLong") { format = Dez_UnsignedLong; return true; }
		else if (str == "SignedLong") { format = Dez_SignedLong; return true; }
		else if (str == "UnsignedShort") { format = Dez_UnsignedShort; return true; }
		else if (str == "SignedShort") { format = Dez_SignedShort; return true; }
		else if (str == "UnsignedChar") { format = Dez_UnsignedChar; return true; }
		else if (str == "SignedChar") { format = Dez_SignedChar; return true; }
		else if (str == "Float") { format = Dez_Float; return true; }
		else if (str == "Double") { format = Dez_Double; return true; }
		else { return false; }
	}

	QString getFormatString() {
		QString form;
		switch (format)
		{
		case Dez_Origin:
			form = "Origin";
			break;
		case Dez_UnsignedLong:
			form = "UnsignedLong";
			break;
		case Dez_SignedLong:
			form = "SignedLong";
			break;
		case Dez_UnsignedShort:
			form = "UnsignedShort";
			break;
		case Dez_SignedShort:
			form = "SignedShort";
			break;
		case Dez_UnsignedChar:
			form = "UnsignedChar";
			break;
		case Dez_SignedChar:
			form = "SignedChar";
			break;
		case Dez_Float:
			form = "Float";
			break;
		case Dez_Double:
			form = "Double";
			break;
		default:
			form = "Error";
			break;
		}
		return form;
	}

	QString toString() {
		QString s = "xLength:[" + QString::number(xLength) + "] "
			"yLength:[" + QString::number(yLength) + "] "
			"zLength:[" + QString::number(zLength) + "]; "
			"xPixelSpace:[" + QString::number(xPixelSpace) + "] "
			"yPixelSpace:[" + QString::number(yPixelSpace) + "] "
			"zPixelSpace:[" + QString::number(zPixelSpace) + "] "
			"form:[" + getFormatString() + "] ";
		return s;
	}
};

struct GenerateFormat {
	DataFormat format = Dez_Origin;
	DcmParseLib parseLib = Dez_GDCM;
};

class ProcessVolumeData: public QObject {
	Q_OBJECT
public:
	ProcessVolumeData();
	~ProcessVolumeData();

public:

	/*******************************************************/
	/* Check if the directory or target file exists. 
	* Check if the generated target file already exists to prevent overwriting.
	* 
	* inputDir: Input Folder
	* inputFilePath: Input File Path
	* outputDir£ºOutput Folder
	* outName: Output File Name */
	/*******************************************************/
	
	/**
	* Check if the input directory exists.
	*/
	bool isInputDirExist(const QString& inputDir);

	/**
	* Check if the input file path exists.
	*/
	bool isInputFileExist(const QString& inputFilePath);

	/**
	* Check if the output directory exists.
	*/
	bool checkOutputDir_Mhd(const QString& outputDir, const QString& outName);

	/**
	* Check if the output directory exists.
	*/
	bool checkOutputDir_Feimos(const QString& outputDir, const QString& outName);

	/**
	* Obtain the sequence of dcm files in the specified directory
	*/
	bool getInputDcmFileList(const QString& inputDir, std::vector<QString>& fileList);


	/*******************************************************/
	/* Generate input data to ImportFormat object
	*
	* inputDir: Input Folder
	* inputFilePath: Input File Path */
	/*******************************************************/

	/**
	* Generate ImportFormat Object From (.dcm) files Using GDCM
	*/
	bool GenerateInput_GDCM(const std::vector<QString>& fileList, ImportFormat& importFormat);

	/**
	* Generate ImportFormat Object From (.dcm) files Using DCMTK
	*/
	bool GenerateInput_DCMTK(const std::vector<QString>& fileList, ImportFormat& importFormat);

	/**
	* Copy data to importFormat.data with format T
	*/
	template <typename T>
	bool CopyUncompressedRawData(const T* data, ImportFormat& importFormat);

	/**
	* Generate ImportFormat Object From (.mhd,.raw) file Using VTK
	*/
	bool GenerateInput_Mhd(const QString& inputFilePath, ImportFormat& importFormat);

	/**
	* Generate ImportFormat Object From (.feimos,.raw) file
	*/
	bool GenerateInput_Feimos(const QString& inputFilePath, ImportFormat& importFormat);


	/*******************************************************/
	/** Process input data
	* Please refer to the instruction document for details */
	/*******************************************************/

	/** Rotate the volume data in MHD format according to the axis set by permute
	* Fill 0,1,2 into the permute array
	*/
	bool RotateAxis(ImportFormat& importFormat, int permute[3]);

	/** MHD Flip up, down, left, and right based on the selected axis
	*/
	bool FlipAxis(ImportFormat& importFormat, int flip[3]);

	/** Clip the MHD file according to the boundary
	* All three axes of the original boundary are [-bound,bound]=[-0.5,0.5], The center is [center]=[0]¡£
	* Cutting needs to ensure that the range of each axis [center-round, center+bound] is within [-0.5,0.5]
	*/
	bool Clip(ImportFormat& importFormat, double center[3], double bound[3]);

	/** DownSampling MHD files
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	bool DownSamplingWithInterval(ImportFormat& importFormat, int Interval = 2);

	/** Resize the volume data by scale
	*/
	bool Resize(ImportFormat& importFormat, float scale);


	/*******************************************************/
	/* Generate output data
	* 
	* outputDir£ºOutput Folder
	* outName: Output File Name */
	/*******************************************************/

	bool DataFormatConvert(const GenerateFormat& generateFormat, ImportFormat& importFormat);

	/**
	* Generate .mhd file
	*/
	bool GenerateOutput_Mhd(const QString& outputDir, const QString& outName,
		const GenerateFormat& generateFormat, ImportFormat& importFormat);

	/**
	* Generate .feimos file
	*/
	bool GenerateOutput_Feimos(const QString& outputDir, const QString& outName,
		const GenerateFormat& generateFormat, ImportFormat& importFormat);


	/*******************************************************/
	/* Call type function
	*
	* inputDir: Input Folder
	* inputFilePath: Input File Path
	* outputDir£ºOutput Folder
	* outName: Output File Name */
	/*******************************************************/

	/**
	* Make (.mhd,.raw) file from (.dcm) files
	*/
	void DcmMakeMhdFile(const QString& inputDir, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.dcm) files
	*/
	void DcmMakeFeimosFile(const QString& inputDir, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.mhd,.raw) file
	*/
	void MhdMakeFeimosFile(const QString& inputFilePath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/**
	* Make (.mhd,.raw) file from (.feimos,.raw) file
	*/
	void FeimosMakeMhdFile(const QString& inputFilePath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/*******************************************************/
	/* Special functions that cannot be performed in steps
	*
	* inputFilePath: Input File Path
	* outputDir£ºOutput Folder
	* outName: Output File Name */
	/*******************************************************/

	/**
	* DownSampling large (.feimos,.raw) file
	*/
	bool DownSamplingLargeFeimosData(const QString& inputFilePath, const QString& outputDir, const QString& outName, int Interval = 5);



	/*******************************************************/
	/******** Old functions *******/
	/*******************************************************/

	/** DCMTK version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* outputDir£ºOutput Folder
	* outName: Output file name
	* 
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The free version does not support JPEG2000 compression format (paid version supports)
	*/
	void DcmMakeMhdFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName);

	/** GDCM version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* outputDir£ºOutput Folder
	* outName: Output file name
	*
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The most comprehensive DICOM formats supported
	*/
	void DcmMakeMhdFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName);


	/** Rotate the volume data in MHD format according to the axis set by permute
	* filePath£ºmhd File Path
	* outputDir£ºOutput Folder
	* outName: Output file name
	* 
	* Fill 0,1,2 into the permute array
	* Store the output results in the outputDir
	*/
	void MhdRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]);

	/** MHD Flip up, down, left, and right based on the selected axis
	* filePath£ºmhd File Path
	* outputDir£ºOutput Folder
	* outName: Output file name
	*
	* Store the output results in the outputDir
	*/
	void MhdFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]);

	/** Clip the MHD file according to the boundary
	* filePath: Input .mhd file path
	* outputDir£ºInput .mhd file path
	* outName: Output file name
	* 
	* All three axes of the original boundary are [-bound,bound]=[-0.5,0.5], The center is [center]=[0]¡£
	* Cutting needs to ensure that the range of each axis [center-round, center+bound] is within [-0.5,0.5]
	*/
	void MhdClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]);

	/** DownSampling MHD files
	* filePath: Input .mhd file path
	* outputDir£ºOutput folder
	* outName: Output file name
	* 
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	void DownSamplingMhdWithInterval(const QString& filePath, const QString& outputDir, const QString& outName, int Interval = 2);


	/** Generate my readable volume data format "Feimos" (binary uncompressed) from MHD files
	* filePath: Input. mhd file path
	* outputDir£ºOutput Folder
	* outName: Output file name
	*/
	void MhdGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/**Generate MHD files into PBRT volume data format (without compression)
	* filePath: The path to the input. mhd file
	* outputDir: Output folder
	* outName: The file name of the output
	*/
	void MhdGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName);

	// test functions
	void process();

	// Obsolete functions
	void DCMTK_Test();
	void GDCM_Test();

signals:
	void PrintString(const char* s);
	void PrintDataD(const char* s, const double data);

	void PrintError(const char* s);
	void PrintWarning(const char* s);


private slots:


};






#endif






