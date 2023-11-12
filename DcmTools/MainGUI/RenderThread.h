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

	Github site: <https://github.com/dezeming/Crystal>
*/

#ifndef __RenderThread_h__
#define __RenderThread_h__

#include <QThread>
#include <QString>

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
	Dez_UnsignedInt = 1,
	Dez_SignedInt = 2,
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

struct ImportFormat {
	DataFormat format;
	unsigned int xLength, yLength, zLength;
	float xPixelSpace, yPixelSpace, zPixelSpace;
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

	void process();


	void DCMTK_Test();
	void GDCM_Test();


	void DcmMakeMhdFile(const QString& dirPath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/** DCMTK version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* outputDir��Output Folder
	* outName: Output file name
	* 
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The free version does not support JPEG2000 compression format (paid version supports)
	*/
	void DcmMakeMhdFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName);

	/** GDCM version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* outputDir��Output Folder
	* outName: Output file name
	*
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The most comprehensive DICOM formats supported
	*/
	void DcmMakeMhdFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName);

	/** GDCM version: Generate Feimos file format from Dicom
	* dirPath: Path to store dicom files
	* outputDir��Output Folder
	* outName: Output file name
	* 
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The most comprehensive DICOM formats supported
	*/
	void DcmMakeFeimosFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName);



	/** Rotate the volume data in MHD format according to the axis set by permute
	* filePath��mhd File Path
	* outputDir��Output Folder
	* outName: Output file name
	* 
	* Fill 0,1,2 into the permute array
	* Store the output results in the outputDir
	*/
	void MhdRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]);


	/** MHD Flip up, down, left, and right based on the selected axis
	* filePath��mhd File Path
	* outputDir��Output Folder
	* outName: Output file name
	*
	* Store the output results in the outputDir
	*/
	void MhdFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]);


	/** Clip the MHD file according to the boundary
	* filePath: Input .mhd file path
	* outputDir��Input .mhd file path
	* outName: Output file name
	* 
	* All three axes of the original boundary are [-bound,bound]=[-0.5,0.5], The center is [center]=[0]��
	* Cutting needs to ensure that the range of each axis [center-round, center+bound] is within [-0.5,0.5]
	*/
	void MhdClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]);


	/** DownSampling MHD files
	* filePath: Input .mhd file path
	* outputDir��Output folder
	* outName: Output file name
	* 
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	void DownSamplingMhdWithInterval(const QString& filePath, const QString& outputDir, const QString& outName, int Interval = 2);


	/**DownSampling "Feimos" data format (without compression)
	* filePath: The path to the input. mhd file
	* outputDir: Output folder
	* outName: The file name of the output
	*
	*/
	void DownSamplingFeimosWithInterval(const QString& filePath, const QString& outputDir, const QString& outName, 
		const GenerateFormat& generateFormat, int Interval = 2);


	/** Generate my readable volume data format "Feimos" (binary uncompressed) from MHD files
	* filePath: Input. mhd file path
	* outputDir��Output Folder
	* outName: Output file name
	*/
	void MhdGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);


	/**Generate MHD files into PBRT volume data format (without compression)
	* filePath: The path to the input. mhd file
	* outputDir: Output folder
	* outName: The file name of the output
	*/
	void MhdGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName);



signals:
	void PrintString(const char* s);
	void PrintDataD(const char* s, const double data);

	void PrintError(const char* s);
	void PrintWarning(const char* s);


private slots:


};






#endif






