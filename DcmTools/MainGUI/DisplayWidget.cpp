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

#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(QGroupBox * parent) {

	renderFlag = false;
	rThread = nullptr;

	setLayout(&displayWidgetLayout);
	displayWidgetLayout.addWidget(&m_IMAGraphicsView);

	framebuffer.InitBuffer(800, 600, 4);

	{
		connect(&processVolumeData, SIGNAL(PrintString(const char*)), this, SLOT(PrintString(const char*)));
		connect(&processVolumeData, SIGNAL(PrintDataD(const char*, const double)), this, SLOT(PrintDataD(const char*, const double)));
		connect(&processVolumeData, SIGNAL(PrintError(const char*)), this, SLOT(PrintErrorString(const char*)));
		connect(&processVolumeData, SIGNAL(PrintWarning(const char*)), this, SLOT(PrintWarningString(const char*)));
	}
}

DisplayWidget::~DisplayWidget() { 
	killRenderThread();
	framebuffer.FreeBuffer();

	{
		disconnect(&processVolumeData, SIGNAL(PrintString(const char*)), this, SLOT(PrintString(const char*)));
		disconnect(&processVolumeData, SIGNAL(PrintDataD(const char*, const double)), this, SLOT(PrintDataD(const char*, const double)));
		disconnect(&processVolumeData, SIGNAL(PrintError(const char*)), this, SLOT(PrintErrorString(const char*)));
		disconnect(&processVolumeData, SIGNAL(PrintWarning(const char*)), this, SLOT(PrintWarningString(const char*)));
	}
}

void DisplayWidget::closeEvent(QCloseEvent *event) {
	killRenderThread();
}

void DisplayWidget::startRenderThread(int index) {
	if (!rThread) {
		rThread = new RenderThread;
		//renderFlag = true;
		rThread->renderFlag = true;
		rThread->p_framebuffer = &framebuffer;

		connect(rThread, SIGNAL(PrintString(const char*)), this, SLOT(PrintString(const char*)));
		connect(rThread, SIGNAL(PrintDataD(const char*, const double)), this, SLOT(PrintDataD(const char*, const double)));
		//connect(rThread, SIGNAL(PrintDataI(char* s, int data)), this, SLOT(PrintDataI(char* s, int data)));
		connect(rThread, SIGNAL(PaintBuffer(DcmTools::Uchar*, int, int, int)), &m_IMAGraphicsView, SLOT(PaintBuffer(DcmTools::Uchar*, int, int, int)));

		connect(rThread, SIGNAL(PrintError(const char*)), this, SLOT(PrintErrorString(const char*)));
		connect(rThread, SIGNAL(PrintWarning(const char*)), this, SLOT(PrintWarningString(const char*)));

		// Temporarily disable thread
		rThread->start();
	}

	renderFlag = true;
}

void DisplayWidget::killRenderThread() {
	if (!rThread)
		return;
	renderFlag = false;
	rThread->renderFlag = false;
	// Kill the render thread
	rThread->quit();
	// Wait for thread to end
	rThread->wait();
	// Remove the render thread
	delete rThread;

	rThread = nullptr;
}

void DisplayWidget::PrintString(const char* s) {
	DebugTextPrintString(s);
}
void DisplayWidget::PrintQString(const QString s) {
	DebugTextPrintString(s);
}
void DisplayWidget::PrintDataD(const char* s, const double data) {
	DebugTextPrintNum(s, data);
}

void DisplayWidget::PrintErrorString(const char* s) {
	DebugTextPrintErrorString(s);
}
void DisplayWidget::PrintWarningString(const char* s) {
	DebugTextPrintWarningString(s);
}


void DisplayWidget::Process(int index) {
	if (0 == index)
		processVolumeData.process();
	// convert
	if (1 == index) {
		processVolumeData.DcmMakeMhdFile(InputFolder, OutputFolder, OutputFileName, generateFormat);
	}
	if (2 == index) {
		processVolumeData.DcmMakeFeimosFile(InputFolder, OutputFolder, OutputFileName, generateFormat);
	}
	if (3 == index) {
		processVolumeData.MhdMakeFeimosFile(InputFilePath, OutputFolder, OutputFileName, generateFormat);
	}
	if (4 == index) {
		processVolumeData.FeimosMakeMhdFile(InputFilePath, OutputFolder, OutputFileName, generateFormat);
	}
	// downsampling
	if (5 == index) {
		processVolumeData.DownSamplingMhdFile(InputFolder, OutputFolder, OutputFileName, interval);
	}
	if (6 == index) {
		processVolumeData.DownSamplingFeimosFile(InputFolder, OutputFolder, OutputFileName, interval);
	}
	if (7 == index) {
		processVolumeData.DownSamplingLargeFeimosData(InputFilePath, OutputFolder, OutputFileName, interval);
	}

	// process
	if (105 == index) {
		processVolumeData.MhdRotateAxis(InputFilePath, OutputFolder, OutputFileName, permute);
	}
	if (106 == index) {
		processVolumeData.MhdFlipAxis(InputFilePath, OutputFolder, OutputFileName, flip);
	}
	if (107 == index) {
		processVolumeData.MhdClip(InputFilePath, OutputFolder, OutputFileName, clipCenter, clipBound);
	}
	if (108 == index) {
		processVolumeData.DownSamplingMhdWithInterval(InputFilePath, OutputFolder, OutputFileName, interval);
	}
}







