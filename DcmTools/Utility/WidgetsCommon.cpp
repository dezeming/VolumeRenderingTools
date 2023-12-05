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

// FileDirSet_Widget

FileDirSet_Widget::FileDirSet_Widget(QWidget * parent) {
	setTitle("Data File Path");
	setMinimumWidth(200);

	mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	setupWidgets();
}
FileDirSet_Widget::~FileDirSet_Widget()
{
	disconnect(OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	disconnect(OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	disconnect(OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	disconnect(OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));
}

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

// QSetFormat_Widget

QSetFormat_Widget::QSetFormat_Widget(QWidget * parent) {
	setTitle("Generate Format");
	setMinimumWidth(200);

	GenerateFormatLayout = new QGridLayout;
	setLayout(GenerateFormatLayout);

	Label_Origin = new QLabel;
	Label_UnsignedInt = new QLabel;
	Label_SignedInt = new QLabel;
	Label_UnsignedShort = new QLabel;
	Label_SignedShort = new QLabel;
	Label_UnsignedChar = new QLabel;
	Label_SignedChar = new QLabel;
	Label_Float = new QLabel;
	Label_Double = new QLabel;

	Label_Origin->setText("Origin");
	Label_UnsignedInt->setText("Unsigned Int");
	Label_SignedInt->setText("Signed Int");
	Label_UnsignedShort->setText("Unsigned Short");
	Label_SignedShort->setText("Signed Short");
	Label_UnsignedChar->setText("Unsigned Char");
	Label_SignedChar->setText("Signed Char");
	Label_Float->setText("Float");
	Label_Double->setText("Double");

	RadioButton_Origin = new QRadioButton;
	RadioButton_UnsignedInt = new QRadioButton;
	RadioButton_SignedInt = new QRadioButton;
	RadioButton_UnsignedShort = new QRadioButton;
	RadioButton_SignedShort = new QRadioButton;
	RadioButton_UnsignedChar = new QRadioButton;
	RadioButton_SignedChar = new QRadioButton;
	RadioButton_Float = new QRadioButton;
	RadioButton_Double = new QRadioButton;

	ButtonGroup = new QButtonGroup;
	ButtonGroup->addButton(RadioButton_Origin);
	ButtonGroup->addButton(RadioButton_UnsignedInt);
	ButtonGroup->addButton(RadioButton_SignedInt);
	ButtonGroup->addButton(RadioButton_UnsignedShort);
	ButtonGroup->addButton(RadioButton_SignedShort);
	ButtonGroup->addButton(RadioButton_UnsignedChar);
	ButtonGroup->addButton(RadioButton_SignedChar);
	ButtonGroup->addButton(RadioButton_Float);
	ButtonGroup->addButton(RadioButton_Double);

	RadioButton_Origin->setChecked(true);

	QFrame* hLine[6];
	for (int fi = 0; fi < 6; fi++) {
		hLine[fi] = new QFrame;
		hLine[fi]->setFrameShape(QFrame::VLine);
		hLine[fi]->setFrameShadow(QFrame::Sunken);
	}

	GenerateFormatLayout->addWidget(Label_UnsignedInt, 0, 0);
	GenerateFormatLayout->addWidget(RadioButton_UnsignedInt, 0, 1);
	GenerateFormatLayout->addWidget(hLine[0], 0, 2);

	GenerateFormatLayout->addWidget(Label_SignedInt, 0, 3);
	GenerateFormatLayout->addWidget(RadioButton_SignedInt, 0, 4);
	GenerateFormatLayout->addWidget(hLine[1], 0, 5);

	GenerateFormatLayout->addWidget(Label_UnsignedShort, 0, 6);
	GenerateFormatLayout->addWidget(RadioButton_UnsignedShort, 0, 7);
	GenerateFormatLayout->addWidget(hLine[2], 0, 8);

	GenerateFormatLayout->addWidget(Label_SignedShort, 0, 9);
	GenerateFormatLayout->addWidget(RadioButton_SignedShort, 0, 10);


	GenerateFormatLayout->addWidget(Label_UnsignedChar, 1, 0);
	GenerateFormatLayout->addWidget(RadioButton_UnsignedChar, 1, 1);
	GenerateFormatLayout->addWidget(hLine[3], 1, 2);

	GenerateFormatLayout->addWidget(Label_SignedChar, 1, 3);
	GenerateFormatLayout->addWidget(RadioButton_SignedChar, 1, 4);
	GenerateFormatLayout->addWidget(hLine[4], 1, 5);

	GenerateFormatLayout->addWidget(Label_Float, 1, 6);
	GenerateFormatLayout->addWidget(RadioButton_Float, 1, 7);
	GenerateFormatLayout->addWidget(hLine[5], 1, 8);

	GenerateFormatLayout->addWidget(Label_Double, 1, 9);
	GenerateFormatLayout->addWidget(RadioButton_Double, 1, 10);

	GenerateFormatLayout->addWidget(Label_Origin, 3, 0);
	GenerateFormatLayout->addWidget(RadioButton_Origin, 3, 1);

	GenerateFormatLayout->setColumnStretch(0, 3);
	GenerateFormatLayout->setColumnStretch(1, 1);
	GenerateFormatLayout->setColumnStretch(2, 2);
	GenerateFormatLayout->setColumnStretch(3, 3);
	GenerateFormatLayout->setColumnStretch(4, 1);
	GenerateFormatLayout->setColumnStretch(5, 2);
	GenerateFormatLayout->setColumnStretch(6, 3);
	GenerateFormatLayout->setColumnStretch(7, 1);
	GenerateFormatLayout->setColumnStretch(8, 2);
	GenerateFormatLayout->setColumnStretch(9, 3);
	GenerateFormatLayout->setColumnStretch(10, 1);
}
QSetFormat_Widget::~QSetFormat_Widget() { }

DataFormat QSetFormat_Widget::getDataFormatSet() {
	if (RadioButton_Origin->isChecked()) return Dez_Origin;
	if (RadioButton_UnsignedInt->isChecked()) return Dez_UnsignedLong;
	if (RadioButton_SignedInt->isChecked()) return Dez_SignedLong;
	if (RadioButton_UnsignedShort->isChecked()) return Dez_UnsignedShort;
	if (RadioButton_SignedShort->isChecked()) return Dez_SignedShort;
	if (RadioButton_UnsignedChar->isChecked()) return Dez_UnsignedChar;
	if (RadioButton_SignedChar->isChecked()) return Dez_SignedChar;
	if (RadioButton_Float->isChecked()) return Dez_Float;
	if (RadioButton_Double->isChecked()) return Dez_Double;
	return Dez_Origin;
}

// QParseDcmLibSet_Widget

QParseDcmLibSet_Widget::QParseDcmLibSet_Widget(QWidget * parent) {
	setTitle("Parse Lib");
	setMinimumWidth(200);

	ParseDcmLibLayout = new QGridLayout;
	setLayout(ParseDcmLibLayout);

	ParseDcmLib_Label_GDCM = new QLabel;
	ParseDcmLib_Label_GDCM->setText("GDCM");
	ParseDcmLib_Label_DCMTK = new QLabel;
	ParseDcmLib_Label_DCMTK->setText("DCMTK");

	Label_Null1 = new QLabel;

	Label_Null2 = new QLabel;

	Label_Null3 = new QLabel;

	Label_Null4 = new QLabel;


	ParseDcmLib_Radio_GDCM = new QRadioButton;
	ParseDcmLib_Radio_DCMTK = new QRadioButton;
	ParseDcmLib_ButtonGroup = new QButtonGroup;
	ParseDcmLib_ButtonGroup->addButton(ParseDcmLib_Radio_GDCM);
	ParseDcmLib_ButtonGroup->addButton(ParseDcmLib_Radio_DCMTK);

	ParseDcmLibLayout->addWidget(ParseDcmLib_Label_GDCM, 0, 0);
	ParseDcmLibLayout->addWidget(ParseDcmLib_Radio_GDCM, 0, 1);
	ParseDcmLibLayout->addWidget(ParseDcmLib_Label_DCMTK, 0, 2);
	ParseDcmLibLayout->addWidget(ParseDcmLib_Radio_DCMTK, 0, 3);

	ParseDcmLib_Radio_GDCM->setChecked(true);

	ParseDcmLibLayout->addWidget(Label_Null1, 0, 4);
	ParseDcmLibLayout->addWidget(Label_Null2, 0, 5);
	ParseDcmLibLayout->addWidget(Label_Null3, 0, 6);
	ParseDcmLibLayout->addWidget(Label_Null4, 0, 7);
}
QParseDcmLibSet_Widget::~QParseDcmLibSet_Widget() { }

DcmParseLib QParseDcmLibSet_Widget::getParseSet() {
	if (ParseDcmLib_Radio_GDCM->isChecked()) return Dez_GDCM;
	if (ParseDcmLib_Radio_DCMTK->isChecked()) return Dez_DCMTK;
	return Dez_GDCM;
}






