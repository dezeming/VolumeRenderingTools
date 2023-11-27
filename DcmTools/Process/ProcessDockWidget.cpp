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

#include "ProcessDockWidget.hpp"

ProcessDockWidget::ProcessDockWidget(QWidget * parent) {
	setWindowTitle("Process");
	
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

	processButton = new QPushButton;
	processButton->setText("Process examples");
	centerLayout->addWidget(processButton);	

	setupInputOutput();

	setupFrames();

	setupProcessFunc();

	setMinimumWidth(200);
}

ProcessDockWidget::~ProcessDockWidget() {

	disconnect(m_OpenPresetPath_Frame->OpenInputDirButton, SIGNAL(clicked()), this, SLOT(OpenInputDir()));
	disconnect(m_OpenPresetPath_Frame->OpenInputFileDirButton, SIGNAL(clicked()), this, SLOT(OpenInputFileDir()));
	disconnect(m_OpenPresetPath_Frame->OpenOutputDirButton, SIGNAL(clicked()), this, SLOT(OpenOutputDir()));
	disconnect(m_OpenPresetPath_Frame->OpenIconsDirButton, SIGNAL(clicked()), this, SLOT(OpenIconsDir()));

}

void ProcessDockWidget::closeEvent(QCloseEvent * event) {

}

void ProcessDockWidget::setupProcessFunc() {
	// volume file type convert
	connect(VolumeConvert_Frame->DcmToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToMhd()));
	connect(VolumeConvert_Frame->DcmToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_DcmToFeimos()));
	connect(VolumeConvert_Frame->MhdToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdToFeimos()));
	connect(VolumeConvert_Frame->FeimosToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosToMhd()));

	// down sampling
	connect(VolumeDownSampling_Frame->MhdDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdDownSampling()));
	connect(VolumeDownSampling_Frame->FeimosDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosDownSampling()));
	connect(VolumeDownSampling_Frame->LargeFeimosDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_LargeFeimosDownSampling()));

	// volume process
	connect(MhdRotateAxis_Frame->MhdRotateAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdRotateAxis()));
	connect(MhdFlipAxis_Frame->MhdFlipAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdFlipAxis()));
	connect(MhdClip_Frame->MhdClip_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdClip()));

	// test
	connect(processButton, SIGNAL(clicked()), this, SLOT(setProcess()));

}

void ProcessDockWidget::setupInputOutput() {

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

void ProcessDockWidget::setupFrames() {

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

void ProcessDockWidget::updataDirFromPresetFile(QString filename) {

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

void ProcessDockWidget::OpenInputDir() {
	QString inputDir = getInputFolder();

	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(inputDir);

	QDesktopServices::openUrl(absolutePath);
}

void ProcessDockWidget::OpenInputFileDir() {
	QString inputFilePath = getInputFilePath();

	QFileInfo fileInfo(inputFilePath);
	QString absoluteFilePath = fileInfo.absoluteFilePath();
	QString absoluteDirPath = fileInfo.absolutePath();

	QDesktopServices::openUrl(absoluteDirPath);
}

void ProcessDockWidget::OpenOutputDir() {
	QString outputDir = getOutputFolder();
	QDir absoluteDir(QCoreApplication::applicationDirPath());
	QString absolutePath = absoluteDir.absoluteFilePath(outputDir);

	DebugTextPrintString(outputDir);

	QDesktopServices::openUrl(absolutePath);
}

void ProcessDockWidget::OpenIconsDir() {
	QDesktopServices::openUrl(QUrl::fromLocalFile("./Icons"));
}

void ProcessDockWidget::getPredefinedInfo() {
	// Obtain predefined information

	QString inputfilePath = getInputFilePath();
	//DebugTextPrintString("inputfilePath: " + inputfilePath);
	QString inputDir = getInputFolder();
	//DebugTextPrintString("inputDir: " + inputDir);
	QString outputDir = getOutputFolder();
	//DebugTextPrintString("outputDir: " + outputDir);
	QString outputFileName = getOutputFileName();
	//DebugTextPrintString("outputFileName: " + outputFileName);

	// input/output
	InputFolder = inputDir;
	OutputFolder = outputDir;
	OutputFileName = outputFileName;
	InputFilePath = inputfilePath;

	generateFormat.parseLib = getDcmParseLib();
	generateFormat.format = getGenerateDataFormat();

	// down sampling
	interval = VolumeDownSampling_Frame->getIntervalValue();
}

void showMemoryInfo(void);

// volume file type convert
void ProcessDockWidget::process_DcmToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .dcm files into VTK's (.mdh-.raw) format.");
	getPredefinedInfo();

	processVolumeData.DcmMakeMhdFile(
		InputFolder, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_DcmToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .dcm files into Feimos' uncompressed (.feimos,.raw) format.");
	getPredefinedInfo();

	processVolumeData.DcmMakeFeimosFile(
		InputFolder, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_MhdToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert (.mhd-.raw) file into (.feimos,.raw) file format.");
	getPredefinedInfo();

	processVolumeData.MhdMakeFeimosFile(
		InputFilePath, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_FeimosToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert (.feimos,.raw) file into (.mhd-.raw) file format.");

	getPredefinedInfo();

	processVolumeData.FeimosMakeMhdFile(
		InputFilePath, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// down sampling
void ProcessDockWidget::process_MhdDownSampling() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("DownSampling .mhd-.raw file.");
	getPredefinedInfo();

	processVolumeData.DownSamplingMhdFile(
		InputFilePath, OutputFolder, OutputFileName,
		generateFormat, interval);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_FeimosDownSampling() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("DownSampling .feimos-.raw file.");
	getPredefinedInfo();

	processVolumeData.DownSamplingFeimosFile(
		InputFilePath, OutputFolder, OutputFileName,
		generateFormat, interval);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_LargeFeimosDownSampling() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("DownSampling large .feimos-.raw file.");
	getPredefinedInfo();

	processVolumeData.DownSamplingLargeFeimosData(
		InputFilePath, OutputFolder, OutputFileName, 
		interval);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// volume process
void ProcessDockWidget::process_MhdRotateAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Permute axis of .mhd-.raw file.");
	getPredefinedInfo();

	bool ok = MhdRotateAxis_Frame->getRotateAxis(permute);
	if (!ok) return;

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_MhdFlipAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Flip axis of .mhd-.raw file.");
	getPredefinedInfo();

	bool ok = MhdFlipAxis_Frame->getFlipAxis(flip);
	if (!ok) return;

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_MhdClip() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Clip.mhd - .raw file.");
	getPredefinedInfo();


	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// test
void ProcessDockWidget::setProcess() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");
	
	DebugTextPrintString("This feature has been removed");

	



	if (0) {

		// DCMTK_Test();
		// GDCM_Test();

		if (0) {
			processVolumeData.DcmMakeMhdFile_DCMTK_OldFunc(
				"D:/Datasets/DicomImages/Heart/",
				"./Output",
				"sample");
		}
		if (0) {
			processVolumeData.DcmMakeMhdFile_GDCM_OldFunc(
				"E:/Datasets/MaxinData/3D_AX+C_Series0010/",
				"./Output",
				"PT096");
		}

		// some test
		if (0) {
			for (int i = 1; i < 486; i++) {
				QString inputfolderPath = "S:/CT-DicomDatasets/Cerebral_aneurysm/AIneurysm/R"
					+ QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10) + "/";

				QString outputfolderPath = "S:/CT-DicomDatasets/GeneratedMhdData/R"
					+ QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10) + "/";

				QDir dir;
				if (!dir.exists(outputfolderPath)) {
					if (dir.mkpath(outputfolderPath)) {
						DebugTextPrintString("Folder created successfully.");
					}
					else {
						DebugTextPrintString("Folder creation failed.");
						continue;
					}
				}
				else {
					DebugTextPrintString("Folder already exists.");
				}

				processVolumeData.DcmMakeMhdFile_GDCM_OldFunc(
					inputfolderPath,
					outputfolderPath,
					"R" + QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10));

			}

		}


		// down sampling
		if (0) {
			processVolumeData.DownSamplingMhdWithInterval_OldFunc(
				"D:/DataSets/OpenScientificVisData-Origin/synthetic_truss_with_five_defects/synthetic_truss.mhd",
				"./Output",
				"synthetic_truss_small", 2);
		}

		// MHD clipping
		if (0) {
			double center[3] = { 0.0, 0, 0.0 };
			double bound[3] = { 0.212, 0.5, 0.5 };
			processVolumeData.MhdClip_OldFunc(
				"D:/DataSets/OpenScientificVisData/Chameleon/chameleon.mhd",
				"Chameleon_small",
				"./Output",
				center, bound);
		}

		// MHD Rotate according to axis
		if (0) {
			int permute[3] = { 0,2,1 };
			processVolumeData.MhdRotateAxis_OldFunc(
				"E:/DataSets/DcmToolsTest/ssas/mhd_abdomen.mhd",
				"E:/DataSets/DcmToolsTest/dddd/",
				"bunny_r",
				permute);
		}

		// MHD flips up, down, left, and right based on the selected axis
		if (0) {
			int flip[3] = { 0,1,0 };
			processVolumeData.MhdFlipAxis_OldFunc(
				"E:/DataSets/DcmToolsTest/mhd_abdomen.mhd",
				"E:/DataSets/DcmToolsTest/dddd/",
				"sample",
				flip);
		}

		// MHD to Feimos format
		if (0) {
			GenerateFormat generateFormat;
			processVolumeData.MhdGenerateFeimosData_OldFunc(
				"sample.mhd",
				"./Output",
				"sample", generateFormat);
		}

		processVolumeData.MhdRotateAxis_OldFunc(InputFilePath, OutputFolder, OutputFileName, permute);
	
		processVolumeData.MhdFlipAxis_OldFunc(InputFilePath, OutputFolder, OutputFileName, flip);
	
		processVolumeData.MhdClip_OldFunc(InputFilePath, OutputFolder, OutputFileName, clipCenter, clipBound);
	
		processVolumeData.DownSamplingMhdWithInterval_OldFunc(InputFilePath, OutputFolder, OutputFileName, interval);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}













