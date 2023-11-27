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

#ifndef __DataReaderAndWriter_h__
#define __DataReaderAndWriter_h__

#include "Utility/DicomCommon.hpp"

class DataReaderAndWriter : public QObject {
	Q_OBJECT
public:
	DataReaderAndWriter();
	~DataReaderAndWriter();

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
	bool isInputFileExist(const QString& inputFilePath, const QString& suffix = "");

	/**
	* Check if the input (.mhd-.raw) file path exists.
	*/
	bool isInputMhdFileExist(const QString& inputFilePath);

	/**
	* Check if the input (.feimos-.raw) file path exists.
	*/
	bool isInputFeimosFileExist(const QString& inputFilePath);

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


};






#endif






