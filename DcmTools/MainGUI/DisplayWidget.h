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
#ifndef __DisplayWidget_h__
#define __DisplayWidget_h__

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QCloseEvent>

#include "IMAGraphicsView.h"

#include "Core/FrameBuffer.h"
#include "RenderThread.h"

class DisplayWidget : public QGroupBox {
	Q_OBJECT


public:
	DisplayWidget(QGroupBox * parent = Q_NULLPTR);
	~DisplayWidget();

public:
	bool renderFlag;
	RenderThread * rThread;
	void startRenderThread(int index = 0);
	void killRenderThread();

	ProcessVolumeData processVolumeData;
	void Process(int index = 0);

	// process setting
	QString InputFolder;
	QString OutputFolder;
	QString OutputFileName;
	QString InputFilePath;

	GenerateFormat generateFormat;

	int permute[3];
	int flip[3];
	double clipCenter[3], clipBound[3];
	double clipLower[3], clipUpper[3];
	int interval;

	// display buffer
	FrameBuffer framebuffer;

private:

	QGridLayout displayWidgetLayout;

	IMAGraphicsView m_IMAGraphicsView;

	void closeEvent(QCloseEvent *event);

private slots:
	void PrintString(const char* s);
	void PrintQString(const QString s);
	void PrintDataD(const char* s, const double data);

	void PrintErrorString(const char* s);
	void PrintWarningString(const char* s);

};







#endif








