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

#ifndef __DicomCommon_h__
#define __DicomCommon_h__

#include <QObject>

#include <QString>
#include <QStringList>

#include <vector>

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

inline QString getDataFormatString(DataFormat format) {
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

inline int getDataFormatOnePixelBytes(DataFormat format) {
	int bytes = -1;
	switch (format)
	{
	case Dez_Origin:
		bytes = -1;
		break;
	case Dez_UnsignedLong:
		bytes = sizeof(unsigned long);
		break;
	case Dez_SignedLong:
		bytes = sizeof(signed long);
		break;
	case Dez_UnsignedShort:
		bytes = sizeof(unsigned short);
		break;
	case Dez_SignedShort:
		bytes = sizeof(signed short);
		break;
	case Dez_UnsignedChar:
		bytes = sizeof(unsigned char);
		break;
	case Dez_SignedChar:
		bytes = sizeof(signed char);
		break;
	case Dez_Float:
		bytes = sizeof(float);
		break;
	case Dez_Double:
		bytes = sizeof(double);
		break;
	default:
		bytes = -1;
		break;
	}
	return bytes;
}

enum DcmParseLib {
	Dez_GDCM = 0,
	Dez_DCMTK = 1
};

struct GenerateFormat {
	DataFormat format = Dez_Origin;
	DcmParseLib parseLib = Dez_GDCM;
};

inline int getFormatOnePixelBytes(const DataFormat form) {
	int bytes = -1;
	switch (form)
	{
	case Dez_Origin:
		bytes = -1;
		break;
	case Dez_UnsignedLong:
		bytes = sizeof(unsigned long);
		break;
	case Dez_SignedLong:
		bytes = sizeof(signed long);
		break;
	case Dez_UnsignedShort:
		bytes = sizeof(unsigned short);
		break;
	case Dez_SignedShort:
		bytes = sizeof(signed short);
		break;
	case Dez_UnsignedChar:
		bytes = sizeof(unsigned char);
		break;
	case Dez_SignedChar:
		bytes = sizeof(signed char);
		break;
	case Dez_Float:
		bytes = sizeof(float);
		break;
	case Dez_Double:
		bytes = sizeof(double);
		break;
	default:
		bytes = -1;
		break;
	}
	return bytes;
}

inline int getFormatOnePixelBytes(const QString str) {
	int bytes = -1;
	if (str == "UnsignedLong") { bytes = sizeof(unsigned long); }
	else if (str == "SignedLong") { bytes = sizeof(signed long); }
	else if (str == "UnsignedShort") { bytes = sizeof(unsigned short); }
	else if (str == "SignedShort") { bytes = sizeof(signed short); }
	else if (str == "UnsignedChar") { bytes = sizeof(unsigned char); }
	else if (str == "SignedChar") { bytes = sizeof(signed char); }
	else if (str == "Float") { bytes = sizeof(float); }
	else if (str == "Double") { bytes = sizeof(double); }
	else {}
	return bytes;
}

struct DcmFilePixelData {
	char *pixData = nullptr;
	double position = 0.0f;
	double slope, intercept;
};

struct VolumeStatistics {
	float min, max;
	float gradientMin, gradientMax;

	float * data = nullptr;
};

struct VolumeData {

public:
	VolumeData() {
		data = nullptr;
		format = Dez_Origin;

		slope = 1.0;
		intercept = 0.0;
	}
	~VolumeData() {
		clear();
	}
public:
	unsigned int xResolution, yResolution, zResolution;
	float xPixelSpace, yPixelSpace, zPixelSpace;

	VolumeStatistics statistics;
	bool getVolumeStatistics();

	// The data for process
	void * data;
	DataFormat format;
	template <typename T>
	T __getPixel(T indicateSrc, unsigned int x, unsigned int y, unsigned int z) const {
		T* data_t = static_cast<T*>(data);
		size_t offset = x + y * xResolution + z * xResolution * yResolution;
		return data_t[offset];
	}
	template <typename T>
	void __setPixel(unsigned int x, unsigned int y, unsigned int z, T data_ta) {
		size_t offset = x + y * xResolution + z * xResolution * yResolution;
		(static_cast<T*>(data))[offset] = data_ta;
	}

	// Info used for .dcm image
	double slope, intercept;

	/****** functions ******/

	// clear buffers
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

		slope = 1.0;
		intercept = 0.0;

		statistics.data = nullptr;
	}

	// Format dependent function
	bool setFormatUsingString(const QString str) {
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
	int getFormatOnePixelBytes() {
		return getDataFormatOnePixelBytes(format);
	}
	QString getFormatString() {
		return getDataFormatString(format);
	}

	// To String
	QString toString() {
		QString s = "xResolution:[" + QString::number(xResolution) + "] "
			"yResolution:[" + QString::number(yResolution) + "] "
			"zResolution:[" + QString::number(zResolution) + "]; "
			"xPixelSpace:[" + QString::number(xPixelSpace) + "] "
			"yPixelSpace:[" + QString::number(yPixelSpace) + "] "
			"zPixelSpace:[" + QString::number(zPixelSpace) + "] "
			"format:[" + getFormatString() + "] ";
		return s;
	}
};

template <typename T>
inline void set_VolumeData(T* data, T dat, unsigned int dim[3], unsigned int pos[3]) {
	unsigned int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	data[offset] = dat;
}
template <typename T>
inline T at_VolumeData(T* data, unsigned int dim[3], unsigned int pos[3]) {
	unsigned int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	return data[offset];
}

struct VolumeDataToWrite {
public:
	VolumeDataToWrite() {
		data_toWrite = nullptr;
		generated = false;
	}
	~VolumeDataToWrite() {
		clearWriteBuffer();
	}


	// If the data format needs to be converted to write, use this pointer
	void * data_toWrite;
	DataFormat format_toWrite;
	bool generated;

	void clearWriteBuffer() {
		if (data_toWrite && generated) {
			switch (format_toWrite)
			{
			case Dez_Origin:
				break;
			case Dez_UnsignedLong:
				delete[] static_cast<unsigned long*>(data_toWrite);
				break;
			case Dez_SignedLong:
				delete[] static_cast<signed long*>(data_toWrite);
				break;
			case Dez_UnsignedShort:
				delete[] static_cast<unsigned short*>(data_toWrite);
				break;
			case Dez_SignedShort:
				delete[] static_cast<signed short*>(data_toWrite);
				break;
			case Dez_UnsignedChar:
				delete[] static_cast<unsigned char*>(data_toWrite);
				break;
			case Dez_SignedChar:
				delete[] static_cast<signed char*>(data_toWrite);
				break;
			case Dez_Float:
				delete[] static_cast<float*>(data_toWrite);
				break;
			case Dez_Double:
				delete[] static_cast<double*>(data_toWrite);
				break;
			default:
				break;
			}
		}
		data_toWrite = nullptr;
	}


};




#endif



