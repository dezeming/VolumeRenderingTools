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

#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(QGroupBox * parent) {

	renderFlag = false;
	rThread = nullptr;

	setLayout(&displayWidgetLayout);
	displayWidgetLayout.addWidget(&m_IMAGraphicsView);

	framebuffer.InitBuffer(800, 600, 4);
}

DisplayWidget::~DisplayWidget() { 
	killRenderThread();
	framebuffer.FreeBuffer();
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

		// Temporarily disable thread
		//rThread->start();
	}

	if (0 == index)
		rThread->process();
	if (1 == index) {
		rThread->DcmMakeMHDFile_DCMTK(InputFolder, OutputFolder, OutputFileName);
	}
	if (2 == index) {
		rThread->DcmMakeMHDFile_GDCM(InputFolder, OutputFolder, OutputFileName);
	}
	if (3 == index) {
		rThread->MHDGenerateFeimosData(InputFolder, OutputFolder, OutputFileName);
	}
	if (4 == index) {
		rThread->MHDGeneratePbrtVolume(InputFolder, OutputFolder, OutputFileName);
	}
	if (5 == index) {
		rThread->MHDRotateAxis(InputFilePath, OutputFolder, OutputFileName, permute);
	}
	if (6 == index) {
		rThread->MHDFlipAxis(InputFilePath, OutputFolder, OutputFileName, flip);
	}
	if (7 == index) {
		rThread->MHDClip(InputFilePath, OutputFolder, OutputFileName, clipCenter, clipBound);
	}
	if (8 == index) {
		rThread->ResizeMHD_IntervalSampling(InputFilePath, OutputFolder, OutputFileName, interval);
	}



	//renderFlag = true;
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
	TextDinodonS(s);
}
void DisplayWidget::PrintQString(const QString s) {
	TextDinodonS(s);
}
void DisplayWidget::PrintDataD(const char* s, const double data) {
	TextDinodonN(s, data);
}
/*void DisplayWidget::PrintDataF(char* s, float data) {
	TextDinodonN(s, data);
}
void DisplayWidget::PrintDataI(char* s, int data) {
	TextDinodonN(s, data);
}*/









