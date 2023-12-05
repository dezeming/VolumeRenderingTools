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

#pragma once

#ifndef __FrameBuffer_H__
#define __FrameBuffer_H__

#include "Utility/Fwd.hpp"

#include "Utility/DebugText.hpp"

#include <QObject>


class FrameBuffer : public QObject {
	Q_OBJECT

public:
	FrameBuffer() :
		width(0), 
		height(0),
		channals(0),
		buffer(nullptr),
		ubuffer(nullptr) { }
	~FrameBuffer(){
		clear();
	}

	void clear() {
		if (buffer) delete[] buffer;
		if (ubuffer) delete[] ubuffer;
		buffer = nullptr;
		ubuffer = nullptr;
		this->width = 0;
		this->height = 0;
		this->channals = 0;
	}

	bool resizeBuffer(const int width = 800, const int height = 600, const int channals = 4) {

		clear();

		if (channals > 4) {
			DebugTextPrintErrorString("Channel is greater than 4!");
			return false;
		}

		this->width = width;
		this->height = height;
		this->channals = channals;

		buffer = new float[width * height * channals];
		ubuffer = new unsigned char[width * height * channals];
		if (nullptr == buffer || nullptr == ubuffer) {
			DebugTextPrintErrorString("FrameBuffer has not applied for enough memory!");
			clear();
			return false;
		}
		return true;
	}

	inline float at_f(const int w, const int h, const int shifting) {
		if (nullptr == buffer) {
			DebugTextPrintErrorString("Access Error: Buffer is empty and cannot be accessed!");
			return 0.0;
		}
		if (w >= width || h >= height || shifting >= channals ) {
			DebugTextPrintErrorString("Access Error: Coordinates exceed array limit!");
			return 0.0;
		}
		int offset = (w + h * height) * channals + shifting;
		return buffer[offset];
	}
	inline bool set_f(const int w, const int h, const int shifting, const float & dat) {
		if (nullptr == buffer) {
			DebugTextPrintErrorString("Access Error: Buffer is empty and cannot be accessed!");
			return false;
		}
		if (w >= width || h >= height || w < 0 || h < 0) {
			DebugTextPrintErrorString("Access Error: Coordinates exceed array limit!");
			return false;
		}
		int offset = (w + h * width) * channals + shifting;
		buffer[offset] = dat;
		return true;
	}
	inline unsigned char at_uc(const int w, const int h, const int shifting) {
		if (nullptr == ubuffer) {
			DebugTextPrintErrorString("Access Error: Buffer is empty and cannot be accessed!");
			return 0.0;
		}
		if (w >= width || h >= height || shifting >= channals) {
			DebugTextPrintErrorString("Access Error: Coordinates exceed array limit!");
			return 0.0;
		}
		int offset = (w + h * height) * channals + shifting;
		return ubuffer[offset];
	}
	inline bool set_uc(const int w, const int h, const int shifting, const unsigned char dat) {
		if (nullptr == ubuffer) {
			DebugTextPrintErrorString("Access Error: Buffer is empty and cannot be accessed!");
			return false;
		}
		if (w >= width || h >= height || w < 0 || h < 0) {
			DebugTextPrintErrorString("Access Error: Coordinates exceed array limit!");
			return false;
		}
		int offset = (w + h * width) * channals + shifting;
		ubuffer[offset] = dat;
		return true;
	}

	inline float fast_at_f(const int w, const int h, const int shifting) {
		return buffer[(w + h * height) * channals + shifting];
	}
	inline void fast_set_f(const int w, const int h, const int shifting, const float & dat) {
		buffer[(w + h * width) * channals + shifting] = dat;
	}
	inline unsigned char fast_at_uc(const int w, const int h, const int shifting) {
		return ubuffer[(w + h * height) * channals + shifting];
	}
	inline void fast_set_uc(const int w, const int h, const int shifting, const unsigned char dat) {
		ubuffer[(w + h * width) * channals + shifting] = dat;
	}

	float * buffer;
	unsigned char * ubuffer;
	int width;
	int height;
	int channals;
};





#endif





