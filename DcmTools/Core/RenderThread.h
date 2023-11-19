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

#include "Core/FrameBuffer.h"
#include "MainGUI/IMAGraphicsView.h"

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



#endif






