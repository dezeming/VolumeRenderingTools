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

	/*******************************************************/
	/* Rotate the volume data in MHD format according to the axis set by permute
	*
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name
	* permute[3]: Fill 0,1,2 into the permute array.
	*/

	template <typename T>
	bool __RotateAxis(T indicate, VolumeData& volumeData, int permute[3]);

	void RotateAxisMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, int permute[3]);
	void RotateAxisFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, int permute[3]);

	/*******************************************************/
	/* Flip up, down, left, and right based on the selected axis
	*
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name
	* flip[3]: flip the axis i if axis[i] does not equal to 0.
	*/

	template <typename T>
	bool __FlipAxis(T indicate, VolumeData& volumeData, int flip[3]);

	void FlipAxisMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, int flip[3]);
	void FlipAxisFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, int flip[3]);

	/*******************************************************/
	/** Clip the MHD file according to the boundary
	*
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name
	* begin[3]: within [0.0,1.0]
	* end[3]: within [0.0,1.0]
	* elements in 'begin' should less than 'end'
	*/
	template <typename T>
	bool __Clip(T indicate, VolumeData& volumeData, double begin[3], double end[3]);

	void ClipMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, double begin[3], double end[3]);
	void ClipFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, 
		const GenerateFormat& generateFormat, double begin[3], double end[3]);

	/*******************************************************/
	/* DownSampling functions
	*
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	/*******************************************************/

	template <typename T>
	bool __downSampling(T* data, VolumeData& volumeData, unsigned int dimM[3], unsigned int dim_Down[3], 
		unsigned int Interval_x, unsigned int Interval_y, unsigned int Interval_z);

	void DownSamplingMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
		const GenerateFormat& generateFormat, int Interval_x = 2, int Interval_y = 2, int Interval_z = 2);

	void DownSamplingFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName,
		const GenerateFormat& generateFormat, int Interval_x = 2, int Interval_y = 2, int Interval_z = 2);

	/**
	* Special functions: cannot be performed in steps
	* DownSampling large (.feimos,.raw) file
	*/
	template <typename T>
	bool __DownSamplingInOneImage(
		const T* data, const unsigned int width, const unsigned int height,
		T* data_aim, const unsigned int resizedWidth, const unsigned int resizedHeight,
		const unsigned int Interval);

	bool DownSamplingLargeFeimosData(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, unsigned int Interval = 5);


	/*******************************************************/
	/* Resize the volume data by scale
	*
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name
	* scale: the resize scale.
	*/
	/*******************************************************/
	bool Resize(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, float scale);


	/*******************************************************/
	/* Call type function
	*
	* inputDir: Input Folder
	* InputFilePath: Input File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output File Name */
	/*******************************************************/

	/**
	* Make (.mhd,.raw) file from (.dcm) files
	*/
	void DcmMakeMhdFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.dcm) files
	*/
	void DcmMakeFeimosFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.mhd,.raw) file
	*/
	void MhdMakeFeimosFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.mhd,.raw) file from (.feimos,.raw) file
	*/
	void FeimosMakeMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.png) files
	*/
	void PngsMakeFeimosFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.mhd,.raw) file from (.png) files
	*/
	void PngsMakeMhdFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.feimos,.raw) file from (.jpg) files
	*/
	void JpgsMakeFeimosFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.mhd,.raw) file from (.jpg) files
	*/
	void JpgsMakeMhdFile(const QString& inputDir, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**
	* Make (.mhd,.raw) file from (.pbrt) file
	*/
	void PbrtMakeMhdFile(const QString& InputFilePath, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);



	/*******************************************************/
	/******** Old functions *******/
	/*******************************************************/

	/** DCMTK version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* OutputDir£ºOutput Folder
	* OutputFileName: Output file name
	* 
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The free version does not support JPEG2000 compression format (paid version supports)
	*/
	void DcmMakeMhdFile_DCMTK_OldFunc(const QString& dirPath, const QString& OutputDir, const QString& OutputFileName);

	/** GDCM version: Generate MHD file from Dicom
	* dirPath: Path to store dicom files
	* OutputDir£ºOutput Folder
	* OutputFileName: Output file name
	*
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* Store the output results in the Output folder
	* The most comprehensive DICOM formats supported
	*/
	void DcmMakeMhdFile_GDCM_OldFunc(const QString& dirPath, const QString& OutputDir, const QString& OutputFileName);


	/** Rotate the volume data in MHD format according to the axis set by permute
	* filePath£ºmhd File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output file name
	* 
	* Fill 0,1,2 into the permute array
	* Store the output results in the OutputDir
	*/
	void MhdRotateAxis_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int permute[3]);

	/** MHD Flip up, down, left, and right based on the selected axis
	* filePath£ºmhd File Path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output file name
	*
	* Store the output results in the OutputDir
	*/
	void MhdFlipAxis_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int flip[3]);

	/** Clip the MHD file according to the boundary
	* filePath: Input .mhd file path
	* OutputDir£ºInput .mhd file path
	* OutputFileName: Output file name
	* 
	* All three axes of the original boundary are [-bound,bound]=[-0.5,0.5], The center is [center]=[0]¡£
	* Cutting needs to ensure that the range of each axis [center-round, center+bound] is within [-0.5,0.5]
	*/
	void MhdClip_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, double center[3], double bound[3]);

	/** DownSampling MHD files
	* filePath: Input .mhd file path
	* OutputDir£ºOutput folder
	* OutputFileName: Output file name
	* 
	* Interval: Reduce the sampling interval, which is equivalent to a resolution reduction of Interval times, and only supports integers.
	*/
	void DownSamplingMhdWithInterval_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, int Interval = 2);


	/** Generate my readable volume data format "Feimos" (binary uncompressed) from MHD files
	* filePath: Input. mhd file path
	* OutputDir£ºOutput Folder
	* OutputFileName: Output file name
	*/
	void MhdGenerateFeimosData_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName, const GenerateFormat& generateFormat);

	/**Generate MHD files into PBRT volume data format (without compression)
	* filePath: The path to the input. mhd file
	* OutputDir: Output folder
	* OutputFileName: The file name of the output
	*/
	void MhdGeneratePbrtVolume_OldFunc(const QString& filePath, const QString& OutputDir, const QString& OutputFileName);

	// Obsolete functions
	void DCMTK_Test_OldFunc();
	void GDCM_Test_OldFunc();



};






#endif






