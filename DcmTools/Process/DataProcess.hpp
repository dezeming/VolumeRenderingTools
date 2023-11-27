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

#ifndef __DataProcess_h__
#define __DataProcess_h__

#include "Utility/DicomCommon.hpp"
#include "DataReaderAndWriter.hpp"


class ProcessVolumeData: public QObject {
	Q_OBJECT
public:
	ProcessVolumeData();
	~ProcessVolumeData();

private:
	DataReaderAndWriter data_rw;

public:

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
	/* DownSampling functions
	*
	* inputFilePath: Input File Path
	* outputDir£ºOutput Folder
	* outName: Output File Name */
	/*******************************************************/

	void DownSamplingMhdFile(const QString& filePath, const QString& outputDir, const QString& outName, 
		const GenerateFormat& generateFormat, int Interval = 2);

	void DownSamplingFeimosFile(const QString& filePath, const QString& outputDir, const QString& outName, 
		const GenerateFormat& generateFormat, int Interval = 2);

	/**
	* Special functions: cannot be performed in steps
	* DownSampling large (.feimos,.raw) file
	*/
	bool DownSamplingLargeFeimosData(const QString& inputFilePath, const QString& outputDir, const QString& outName, unsigned int Interval = 5);


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
	void DcmMakeMhdFile_DCMTK_OldFunc(const QString& dirPath, const QString& outputDir, const QString& outName);

	/** GDCM version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* outputDir£ºOutput Folder
	* outName: Output file name
	*
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The most comprehensive DICOM formats supported
	*/
	void DcmMakeMhdFile_GDCM_OldFunc(const QString& dirPath, const QString& outputDir, const QString& outName);


	/** Rotate the volume data in MHD format according to the axis set by permute
	* filePath£ºmhd File Path
	* outputDir£ºOutput Folder
	* outName: Output file name
	* 
	* Fill 0,1,2 into the permute array
	* Store the output results in the outputDir
	*/
	void MhdRotateAxis_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]);

	/** MHD Flip up, down, left, and right based on the selected axis
	* filePath£ºmhd File Path
	* outputDir£ºOutput Folder
	* outName: Output file name
	*
	* Store the output results in the outputDir
	*/
	void MhdFlipAxis_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]);

	/** Clip the MHD file according to the boundary
	* filePath: Input .mhd file path
	* outputDir£ºInput .mhd file path
	* outName: Output file name
	* 
	* All three axes of the original boundary are [-bound,bound]=[-0.5,0.5], The center is [center]=[0]¡£
	* Cutting needs to ensure that the range of each axis [center-round, center+bound] is within [-0.5,0.5]
	*/
	void MhdClip_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]);

	/** DownSampling MHD files
	* filePath: Input .mhd file path
	* outputDir£ºOutput folder
	* outName: Output file name
	* 
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	void DownSamplingMhdWithInterval_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName, int Interval = 2);


	/** Generate my readable volume data format "Feimos" (binary uncompressed) from MHD files
	* filePath: Input. mhd file path
	* outputDir£ºOutput Folder
	* outName: Output file name
	*/
	void MhdGenerateFeimosData_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName, const GenerateFormat& generateFormat);

	/**Generate MHD files into PBRT volume data format (without compression)
	* filePath: The path to the input. mhd file
	* outputDir: Output folder
	* outName: The file name of the output
	*/
	void MhdGeneratePbrtVolume_OldFunc(const QString& filePath, const QString& outputDir, const QString& outName);

	// Obsolete functions
	void DCMTK_Test_OldFunc();
	void GDCM_Test_OldFunc();



};






#endif






