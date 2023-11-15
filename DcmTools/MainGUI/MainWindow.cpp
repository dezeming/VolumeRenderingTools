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

#include "MainWindow.h"
#include "DebugText.hpp"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
	setMinimumSize(800, 800);

	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	MainWindowLayout = new QVBoxLayout;
	MainWindowLayout->setAlignment(Qt::AlignCenter);

	centralWidget->setLayout(MainWindowLayout);

	setMenu();

	setWidget();

	setDock();

	/*DebugTextPrintString("Let's start !");

	DcmTools::Vector4f vec(2.0, 1.0, 3.0, 2.0);
	TextDinVector("vec", vec);
	vec / 0;

	DcmTools::Point4f pot(2.0, 1.0, 3.0, 2.0);
	TextDinPoint("pot", pot);
	pot / 0;

	DcmTools::Normal n(1.0, 1.0, 1.0);
	n = DcmTools::normalize(n);
	TextDinNormal("n", n);

	DebugTextPrintString("Let's end !");*/
	
	
	/*DebugTextPrintString("Matrix Test start !");

	DcmTools::Vector4f vec1(1.0, 2.0, 3.0, 1.0);
	DcmTools::Vector4f vec2(2.0, 1.0, 1.0, 3.0);
	DcmTools::Vector4f vec3(1.0, 3.0, 1.0, 2.0);
	DcmTools::Vector4f vec4(4.0, 1.0, 2.0, 3.0);
	DcmTools::Matrix4x4 m(vec1, vec2, vec3, vec4);
	DcmTools::Matrix4x4 m_ivt;
	bool success = m.invert(m_ivt);
	if (!success) DebugTextPrintString("EError Unable to invert singular matrix");
	else TextDinMatrix("m_ivt", m_ivt);

	DcmTools::Matrix4x4 I = m * m_ivt;
	TextDinMatrix("I", I);

	DebugTextPrintString("Matrix Test end !");*/


}

void MainWindow::closeEvent(QCloseEvent *event) {
	// ¹Ø±ÕäÖÈ¾Ïß³Ì
	m_DisplayWidget->killRenderThread();

}

void MainWindow::setMenu(void) {


}

void MainWindow::setWidget(void) {
	
	m_DisplayWidget = new DisplayWidget;
	MainWindowLayout->addWidget(m_DisplayWidget);

}

void showMemoryInfo(void);
void MainWindow::setDock(void) {
	m_InteractionDockWidget = new InteractionDockWidget;
	addDockWidget(Qt::LeftDockWidgetArea, m_InteractionDockWidget);

	// volume file type convert
	connect(m_InteractionDockWidget->VolumeConvert_Frame->DcmToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToMhd()));
	connect(m_InteractionDockWidget->VolumeConvert_Frame->DcmToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToFeimos()));
	connect(m_InteractionDockWidget->VolumeConvert_Frame->MhdToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdToFeimos()));
	connect(m_InteractionDockWidget->VolumeConvert_Frame->FeimosToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosToMhd()));

	// volume process
	connect(m_InteractionDockWidget->MhdRotateAxis_Frame->MhdRotateAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdRotateAxis()));
	connect(m_InteractionDockWidget->MhdFlipAxis_Frame->MhdFlipAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdFlipAxis()));
	connect(m_InteractionDockWidget->MhdClip_Frame->MhdClip_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdClip()));
	connect(m_InteractionDockWidget->MhdResize_Frame->MhdResize_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdResize()));

	// test
	connect(m_InteractionDockWidget->processButton, 
		SIGNAL(clicked()), this, SLOT(setProcess()));

	m_DataPresentDockWidget = new DataPresentDockWidget;
	addDockWidget(Qt::RightDockWidgetArea, m_DataPresentDockWidget);
	showMemoryInfo();
}

void MainWindow::setRendering() {
	if (!m_DisplayWidget->renderFlag) {
		// start rendering
		m_InteractionDockWidget->processButton->setText("Processing");
		m_DisplayWidget->startRenderThread();
	}
}



void MainWindow::getPredefinedInfo() {
	// Obtain predefined information
	
	QString inputfilePath = m_InteractionDockWidget->getInputFilePath();
	//DebugTextPrintString("inputfilePath: " + inputfilePath);
	QString inputDir = m_InteractionDockWidget->getInputFolder();
	//DebugTextPrintString("inputDir: " + inputDir);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	//DebugTextPrintString("outputDir: " + outputDir);
	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	//DebugTextPrintString("outputFileName: " + outputFileName);

	m_DisplayWidget->InputFolder = inputDir;
	m_DisplayWidget->OutputFolder = outputDir;
	m_DisplayWidget->OutputFileName = outputFileName;
	m_DisplayWidget->InputFilePath = inputfilePath;

	m_DisplayWidget->generateFormat.parseLib = m_InteractionDockWidget->getDcmParseLib();
	m_DisplayWidget->generateFormat.format = m_InteractionDockWidget->getGenerateDataFormat();
	
}

// volume file type convert
void MainWindow::process_DcmToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .dcm files into VTK's .mdh-.raw format.");
	getPredefinedInfo();

	m_DisplayWidget->Process(1);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_DcmToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .dcm files into Feimos' uncompressed .raw format.");
	getPredefinedInfo();

	m_DisplayWidget->Process(2);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_MhdToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .mhd-.raw file into Feimos file format.");
	getPredefinedInfo();


	m_DisplayWidget->Process(3);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_FeimosToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .mhd-.raw file into Pbrt file format.");

	getPredefinedInfo();

	m_DisplayWidget->Process(4);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// volume process
void MainWindow::process_MhdRotateAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Permute axis of .mhd-.raw file.");
	getPredefinedInfo();

	bool ok = m_InteractionDockWidget->MhdRotateAxis_Frame->getRotateAxis(m_DisplayWidget->permute);
	if (!ok) return;

	m_DisplayWidget->Process(5);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_MhdFlipAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Flip axis of .mhd-.raw file.");
	getPredefinedInfo();

	bool ok = m_InteractionDockWidget->MhdFlipAxis_Frame->getFlipAxis(m_DisplayWidget->flip);
	if (!ok) return;

	m_DisplayWidget->Process(6);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_MhdClip() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Clip.mhd - .raw file.");
	getPredefinedInfo();

	m_DisplayWidget->Process(4);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void MainWindow::process_MhdResize() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Resize .mhd-.raw file.");
	getPredefinedInfo();

	m_DisplayWidget->Process(4);
	
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// test
void MainWindow::setProcess() {
	DebugTextPrintString("........................  New Task   ................................");
	m_DisplayWidget->Process(0);
	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}



