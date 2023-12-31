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

#include "DisplayWidget.hpp"

DisplayWidget::DisplayWidget(QGroupBox * parent) {

	renderFlag = false;
	rThread = nullptr;

	setLayout(&displayWidgetLayout);
	displayWidgetLayout.addWidget(&m_IMAGraphicsView);
}

DisplayWidget::~DisplayWidget() { 
	killRenderThread();
}

void DisplayWidget::closeEvent(QCloseEvent *event) {
	killRenderThread();
}

void DisplayWidget::startRenderThread(int index) {
	if (!rThread) {
		rThread = new RenderThread;
		//renderFlag = true;
		rThread->renderFlag = true;

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







