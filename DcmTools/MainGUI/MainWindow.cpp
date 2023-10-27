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

#include "MainWindow.h"
#include "DebugText.hpp"


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
	
	connect(m_InteractionDockWidget->renderButton, SIGNAL(clicked()), this, SLOT(setRendering()));

	
	/*TextDinodonS("Let's start !");

	DcmTools::Vector4f vec(2.0, 1.0, 3.0, 2.0);
	TextDinVector("vec", vec);
	vec / 0;

	DcmTools::Point4f pot(2.0, 1.0, 3.0, 2.0);
	TextDinPoint("pot", pot);
	pot / 0;

	DcmTools::Normal n(1.0, 1.0, 1.0);
	n = DcmTools::normalize(n);
	TextDinNormal("n", n);

	TextDinodonS("Let's end !");*/
	
	
	/*TextDinodonS("Matrix Test start !");

	DcmTools::Vector4f vec1(1.0, 2.0, 3.0, 1.0);
	DcmTools::Vector4f vec2(2.0, 1.0, 1.0, 3.0);
	DcmTools::Vector4f vec3(1.0, 3.0, 1.0, 2.0);
	DcmTools::Vector4f vec4(4.0, 1.0, 2.0, 3.0);
	DcmTools::Matrix4x4 m(vec1, vec2, vec3, vec4);
	DcmTools::Matrix4x4 m_ivt;
	bool success = m.invert(m_ivt);
	if (!success) TextDinodonS("EError Unable to invert singular matrix");
	else TextDinMatrix("m_ivt", m_ivt);

	DcmTools::Matrix4x4 I = m * m_ivt;
	TextDinMatrix("I", I);

	TextDinodonS("Matrix Test end !");*/


}

void MainWindow::closeEvent(QCloseEvent *event) {
	// �ر���Ⱦ�߳�
	m_DisplayWidget->killRenderThread();

}

void MainWindow::setMenu(void) {


}

void MainWindow::setWidget(void) {
	
	m_DisplayWidget = new DisplayWidget;
	MainWindowLayout->addWidget(m_DisplayWidget);

}

void MainWindow::setDock(void) {
	m_InteractionDockWidget = new InteractionDockWidget;
	addDockWidget(Qt::LeftDockWidgetArea, m_InteractionDockWidget);

	connect(m_InteractionDockWidget->DcmToMhd_Frame_DCMTK_Frame->DcmToMhd_Frame_DCMTK_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToMhd_DCMTK()));
	connect(m_InteractionDockWidget->DcmToMhd_Frame_GDCM_Frame->DcmToMhd_Frame_GDCM_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToMhd_GDCM()));
	connect(m_InteractionDockWidget->MhdToFeimos_Frame->MhdToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdToFeimos()));
	connect(m_InteractionDockWidget->MhdToPbrt_Frame->MhdToPbrt_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdToPbrt()));

	connect(m_InteractionDockWidget->MhdRotateAxis_Frame->MhdRotateAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdRotateAxis()));
	connect(m_InteractionDockWidget->MhdFlipAxis_Frame->MhdFlipAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdFlipAxis()));
	connect(m_InteractionDockWidget->MhdClip_Frame->MhdClip_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdClip()));
	connect(m_InteractionDockWidget->MhdResize_Frame->MhdResize_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdResize()));

}

void MainWindow::setRendering() {

	if (!m_DisplayWidget->renderFlag) {
		// start rendering
		m_InteractionDockWidget->renderButton->setText("Processing");
		m_DisplayWidget->startRenderThread();
	}

}

#include <QDir>

void MainWindow::process_DcmToMhd_DCMTK() {

	TextDinodonS("Convert .dcm files into VTK's .mdh-.raw format using DCMTK lib.");

	QString inputDir = m_InteractionDockWidget->getInputFolder();
	TextDinodonS("inputDir: " + inputDir);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QDir dir;
	if (!dir.exists(inputDir)) {
		TextDinodonS("Error: Input Folder does not exist.");
		return;
	}
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);

	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}

	if (!m_DisplayWidget->renderFlag) {
		
		m_DisplayWidget->InputFolder = inputDir;
		m_DisplayWidget->OutputFolder = outputDir;
		m_DisplayWidget->OutputFileName = outputFileName;

		m_DisplayWidget->startRenderThread(1);
	}

}
void MainWindow::process_DcmToMhd_GDCM() {

	TextDinodonS("Convert .dcm files into VTK's .mdh-.raw format using GDCM lib.");

	QString inputDir = m_InteractionDockWidget->getInputFolder();
	TextDinodonS("inputDir: " + inputDir);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QDir dir;
	if (!dir.exists(inputDir)) {
		TextDinodonS("Error: Input Folder does not exist.");
		return;
	}
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);

	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}

	if (!m_DisplayWidget->renderFlag) {

		m_DisplayWidget->InputFolder = inputDir;
		m_DisplayWidget->OutputFolder = outputDir;
		m_DisplayWidget->OutputFileName = outputFileName;

		m_DisplayWidget->startRenderThread(2);
	}

}
void MainWindow::process_MhdToFeimos() {
	TextDinodonS("Convert .mhd-.raw file into Feimos file format.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}


}
void MainWindow::process_MhdToPbrt() {
	TextDinodonS("Convert .mhd-.raw file into Pbrt file format.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}


}
void MainWindow::process_MhdRotateAxis() {
	TextDinodonS("Permute axis of .mhd-.raw file.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}


}
void MainWindow::process_MhdFlipAxis() {
	TextDinodonS("Flip axis of .mhd-.raw file.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}



}
void MainWindow::process_MhdClip() {
	TextDinodonS("Clip.mhd - .raw file.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}



}
void MainWindow::process_MhdResize() {
	TextDinodonS("Resize .mhd-.raw file.");

	QString inputfile = m_InteractionDockWidget->getInputFilePath();
	TextDinodonS("inputfile: " + inputfile);
	QString outputDir = m_InteractionDockWidget->getOutputFolder();
	TextDinodonS("outputDir: " + outputDir);

	QFile file_input(inputfile);
	if (!file_input.exists()) {
		TextDinodonS("Error: The input file does not exist.");
		return;
	}

	QDir dir;
	if (!dir.exists(outputDir)) {
		if (dir.mkpath(outputDir)) {
			TextDinodonS("Folder created successfully.");
		}
		else {
			TextDinodonS("Error: Folder creation failed.");
			return;
		}
	}
	else {
		TextDinodonS("Folder already exists.");
	}

	QString outputFileName = m_InteractionDockWidget->getOutputFileName();
	TextDinodonS("outputFileName: " + outputFileName);
	QFile file_mhd(outputDir + "/" + outputFileName + ".mhd");
	if (file_mhd.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}
	QFile file_raw(outputDir + "/" + outputFileName + ".raw");
	if (file_raw.exists()) {
		TextDinodonS("Error: It is mandatory not to overwrite files with the same name.");
		return;
	}




}





