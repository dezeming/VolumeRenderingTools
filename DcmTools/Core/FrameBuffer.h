#pragma once

#ifndef __FrameBuffer_H__
#define __FrameBuffer_H__

#include "Fwd.h"

#include "MainGUI\DebugText.hpp"

#include <QObject>


class FrameBuffer : public QObject {
	Q_OBJECT

public:
	FrameBuffer() :
		width(width), 
		height(height),
		channals(channals),
		buffer(nullptr),
		ubuffer(nullptr) { }

	~FrameBuffer(){
		if (nullptr != buffer) delete[] buffer;
		if (nullptr != ubuffer) delete[] ubuffer;
		buffer = nullptr;
		ubuffer = nullptr;
		this->width = 0;
		this->height = 0;
		this->channals = 0;
	}

	void InitBuffer(const int width = 800, const int height = 600, const int channals = 4) {
		this->width = width;
		this->height = height;
		this->channals = channals;
		buffer = nullptr;
		ubuffer = nullptr;
		if (channals > 4) {
			TextDinodonS("Initialize Error: Channel is greater than 4!");
			return;
		}
		buffer = new DcmTools::Float[width * height * channals];
		ubuffer = new DcmTools::Uchar[width * height * channals];
		if (nullptr == buffer || nullptr == ubuffer) {
			TextDinodonS("Initialize Error: FrameBuffer has not applied for enough memory!");
			this->width = 0;
			this->height = 0;
			this->channals = 0;
		}
	}
	void FreeBuffer() {
		if (nullptr != buffer) delete[] buffer;
		if (nullptr != ubuffer) delete[] ubuffer;
		buffer = nullptr;
		ubuffer = nullptr;
		this->width = 0;
		this->height = 0;
		this->channals = 0;
	}
	bool bufferResize(const int width = 800, const int height = 600) {
		if (this->width == 0 || this->height == 0 || this->channals == 0) {
			TextDinodonS("Resize Error: The previous buffer size was 0 !");
			return false;
		}
		if (nullptr != buffer) delete[] buffer;
		this->width = width; this->height = width;
		buffer = new DcmTools::Float[width * height * channals];
		if (nullptr == buffer) {
			TextDinodonS("Resize Error: FrameBuffer has not applied for enough memory!");
			this->width = 0;
			this->height = 0;
			this->channals = 0;
			return false;
		}
		return true;
	}

	inline DcmTools::Float At_f(const int w, const int h, const int shifting) {
		if (nullptr == buffer) {
			TextDinodonS("Access Error: Buffer is empty and cannot be accessed!");
			return 0.0;
		}
		if (w >= width || h >= height || shifting >= channals ) {
			TextDinodonS("Access Error: Coordinates exceed array limit!");
			return 0.0;
		}
		int offset = (w + h * height) * channals + shifting;
		return buffer[offset];
	}

	inline bool set_f(const int w, const int h, const int shifting, const DcmTools::Float & dat) {
		if (nullptr == buffer) {
			TextDinodonS("Access Error: Buffer is empty and cannot be accessed!");
			return false;
		}
		if (w >= width || h >= height || w < 0 || h < 0) {
			TextDinodonS("Access Error: Coordinates exceed array limit!");
			return false;
		}
		int offset = (w + h * width) * channals + shifting;
		buffer[offset] = dat;
		return true;
	}
	inline bool set_uc(const int w, const int h, const int shifting, const DcmTools::Uchar & dat) {
		if (nullptr == buffer) {
			TextDinodonS("Access Error: Buffer is empty and cannot be accessed!");
			return false;
		}
		if (w >= width || h >= height || w < 0 || h < 0) {
			TextDinodonS("Access Error: Coordinates exceed array limit!");
			return false;
		}
		int offset = (w + h * width) * channals + shifting;
		ubuffer[offset] = dat;
		return true;
	}

	DcmTools::Uchar * getUCbuffer() { return ubuffer; }

private:
	DcmTools::Float * buffer;
	DcmTools::Uchar * ubuffer;
	int width;
	int height;
	int channals;

};





#endif





