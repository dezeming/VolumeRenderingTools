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

#include "WidgetsCommon.hpp"
#include "Utility/DebugText.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>

void FileDirSet_Widget::updataDirFromPresetFile(QString filename) {
	QFile file_input(filename);
	if (!file_input.exists()) {
		return;
	}

	std::ifstream file(filename.toStdString());
	std::string name;

	QString InputFolderStr = "";
	QString InputFilePathStr = "";
	QString OutputFolderStr = "";
	QString OutputFileNameStr = "";

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

void FileDirSet_Widget::setupWidgets()
{
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

	mainLayout->addLayout(FolderLayout);
	
	updataDirFromPresetFile("./Icons/DirPreset.txt");

	openLayout = new QGridLayout;
	OpenInputDirButton = new QPushButton;
	OpenOutputDirButton = new QPushButton;
	OpenInputFileDirButton = new QPushButton;
	OpenIconsDirButton = new QPushButton;

	OpenInputDirButton->setText("Open InputDir");
	OpenInputFileDirButton->setText("Open InputFile Dir");
	OpenOutputDirButton->setText("Open OutputDir");
	OpenIconsDirButton->setText("Open IconsDir");

	openLayout->addWidget(OpenInputDirButton, 0, 0);
	openLayout->addWidget(OpenInputFileDirButton, 0, 1);
	openLayout->addWidget(OpenOutputDirButton, 0, 2);
	openLayout->addWidget(OpenIconsDirButton, 0, 3);

	mainLayout->addLayout(openLayout);


	connect(OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	connect(OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	connect(OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	connect(OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));
}

FileDirSet_Widget::~FileDirSet_Widget()
{
	disconnect(OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	disconnect(OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	disconnect(OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	disconnect(OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));
}

void FileDirSet_Widget::OpenInputDir() {
	QString inputDir = getInputFolder();

	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(inputDir);

	QDesktopServices::openUrl(absolutePath);
}

void FileDirSet_Widget::OpenInputFileDir() {
	QString inputFilePath = getInputFilePath();

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	QDesktopServices::openUrl(absoluteDirPath);
}

void FileDirSet_Widget::OpenOutputDir() {
	QString outputDir = getOutputFolder();
	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(outputDir);

	DebugTextPrintString(outputDir);

	QDesktopServices::openUrl(absolutePath);
}

void FileDirSet_Widget::OpenIconsDir() {
	QDesktopServices::openUrl(QUrl::fromLocalFile("./Icons"));
}
