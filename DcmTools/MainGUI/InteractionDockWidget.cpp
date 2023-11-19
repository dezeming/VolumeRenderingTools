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

#include "InteractionDockWidget.h"

InteractionDockWidget::InteractionDockWidget(QWidget * parent) {
	setWindowTitle("Interaction");
	
	InputFolderStr = "";
	InputFilePathStr = "";
	OutputFolderStr = "";
	OutputFileNameStr = "";

	setFeatures(QDockWidget::DockWidgetMovable);
	
	dockCentralWidget = new QFrame;

	setWidget(dockCentralWidget);
	centerLayout = new QVBoxLayout;
	dockCentralWidget->setWindowFlags(Qt::FramelessWindowHint);
	dockCentralWidget->setLayout(centerLayout);
	
	setupInputOutput();

	setupFrames();

	setupDock();



	setMinimumWidth(200);
}

InteractionDockWidget::~InteractionDockWidget() {

	disconnect(m_OpenPresetPath_Frame->OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	disconnect(m_OpenPresetPath_Frame->OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	disconnect(m_OpenPresetPath_Frame->OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	disconnect(m_OpenPresetPath_Frame->OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));

}

void InteractionDockWidget::closeEvent(QCloseEvent * event) {

}

void InteractionDockWidget::setupDock() {

	processButton = new QPushButton;
	processButton->setText("Process examples");

	centerLayout->addWidget(processButton);
}

void InteractionDockWidget::setupInputOutput() {

	FolderLayout = new QGridLayout;
	InputFolderButton = new QPushButton;
	InputFolderEdit = new QLineEdit;
	InputFilePathButton = new QPushButton;
	InputFilePathEdit = new QLineEdit;
	OutputFolderButton = new QPushButton;
	OutputFolderEdit = new QLineEdit;
	OutputFileNameButton = new QPushButton;
	OutputFileNameEdit = new QLineEdit;

	FolderLayout->addWidget(InputFolderButton, 0, 0);
	InputFolderButton->setText("Input Folder");
	FolderLayout->addWidget(InputFolderEdit, 0, 1);
	InputFolderEdit->setText("E:/Datasets/DcmToolsTest/");

	FolderLayout->addWidget(InputFilePathButton, 1, 0);
	InputFilePathButton->setText("Input File Path");
	FolderLayout->addWidget(InputFilePathEdit, 1, 1);
	InputFilePathEdit->setText("E:/Datasets/DcmToolsTest/Sample.mhd");

	FolderLayout->addWidget(OutputFolderButton, 2, 0);
	OutputFolderButton->setText("Output Folder");
	FolderLayout->addWidget(OutputFolderEdit, 2, 1);
	OutputFolderEdit->setText("E:/Datasets/DcmToolsTest/Output/");

	FolderLayout->addWidget(OutputFileNameButton, 3, 0);
	OutputFileNameButton->setText("Output File Name");
	FolderLayout->addWidget(OutputFileNameEdit, 3, 1);
	OutputFileNameEdit->setText("Sample");

	centerLayout->addLayout(FolderLayout);

	m_QParseDcmLibFormat_Frame = new QParseDcmLibFormat_Frame;

	m_QGenerateFormat_Frame = new QGenerateFormat_Frame;

	centerLayout->addWidget(m_QParseDcmLibFormat_Frame);

	centerLayout->addWidget(m_QGenerateFormat_Frame);

	updataDirFromPresetFile("./Icons/DirPreset.txt");


	m_OpenPresetPath_Frame = new OpenPresetPath_Frame;
	connect(m_OpenPresetPath_Frame->OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	connect(m_OpenPresetPath_Frame->OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	connect(m_OpenPresetPath_Frame->OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	connect(m_OpenPresetPath_Frame->OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));

	centerLayout->addWidget(m_OpenPresetPath_Frame);
}

void InteractionDockWidget::setupFrames() {

	framesLayout = new QGridLayout;

	VolumeConvert_Frame = new QVolumeConvert_Frame;
	framesLayout->addWidget(VolumeConvert_Frame, 0, 0);

	VolumeDownSampling_Frame = new QVolumeDownSampling_Frame;
	framesLayout->addWidget(VolumeDownSampling_Frame, 0, 1);

	MhdRotateAxis_Frame = new QMhdRotateAxis_Frame;
	framesLayout->addWidget(MhdRotateAxis_Frame, 1, 0);

	MhdFlipAxis_Frame = new QMhdFlipAxis_Frame;
	framesLayout->addWidget(MhdFlipAxis_Frame, 1, 1);

	MhdClip_Frame = new QMhdClip_Frame;
	framesLayout->addWidget(MhdClip_Frame, 2, 0);


	centerLayout->addLayout(framesLayout);
}


#include <string>
#include <iostream>
#include <fstream>

#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>

void InteractionDockWidget::updataDirFromPresetFile(QString filename) {

	QFile file_input(filename);
	if (!file_input.exists()) {
		return;
	}

	std::ifstream file(filename.toStdString());
	std::string name;

	while (file >> name) {
		if (name == "InputFolder") {
			//file >> name;
			std::getline(file, name);
			name.erase(name.begin());
			if (InputFolderStr == "")
				InputFolderStr = QString(name.c_str());
		}
		else if (name == "InputFilePath") {
			//file >> name;
			std::getline(file, name);
			name.erase(name.begin());
			if (InputFilePathStr == "")
				InputFilePathStr = QString(name.c_str());
		}
		else if (name == "OutputFolder") {
			//file >> name;
			std::getline(file, name);
			name.erase(name.begin());
			if (OutputFolderStr == "")
				OutputFolderStr = QString(name.c_str());
		}
		else if (name == "OutputFileName") {
			//file >> name;
			std::getline(file, name);
			name.erase(name.begin());
			if (OutputFileNameStr == "")
				OutputFileNameStr = QString(name.c_str());
		}
	}
	file.close();

	if (InputFolderStr != "") {
		InputFolderEdit->setText(InputFolderStr);
	}
	if (InputFilePathStr != "") {
		InputFilePathEdit->setText(InputFilePathStr);
	}
	if (OutputFolderStr != "") {
		OutputFolderEdit->setText(OutputFolderStr);
	}
	if (OutputFileNameStr != "") {
		OutputFileNameEdit->setText(OutputFileNameStr);
	}
}

void InteractionDockWidget::OpenInputDir() {
	QString inputDir = getInputFolder();

	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(inputDir);

	QDesktopServices::openUrl(absolutePath);
}

void InteractionDockWidget::OpenInputFileDir() {
	QString inputFilePath = getInputFilePath();

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	QDesktopServices::openUrl(absoluteDirPath);
}

void InteractionDockWidget::OpenOutputDir() {
	QString outputDir = getOutputFolder();
	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(outputDir);

	DebugTextPrintString(outputDir);

	QDesktopServices::openUrl(absolutePath);
}

void InteractionDockWidget::OpenIconsDir() {
	QDesktopServices::openUrl(QUrl::fromLocalFile("./Icons"));
}











