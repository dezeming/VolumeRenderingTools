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

	setFeatures(QDockWidget::DockWidgetMovable);
	
	dockCentralWidget = new QFrame;

	setWidget(dockCentralWidget);
	centerLayout = new QVBoxLayout;
	dockCentralWidget->setWindowFlags(Qt::FramelessWindowHint);
	dockCentralWidget->setLayout(centerLayout);

	setupWidget();
	setupFrames();
	setupProcessFunc();

	setMinimumWidth(200);
}

ProcessDockWidget::~ProcessDockWidget() {

}

void ProcessDockWidget::closeEvent(QCloseEvent * event) {

}

void ProcessDockWidget::setupWidget() {

	m_QParseDcmLibSet_Widget = new QParseDcmLibSet_Widget;
	m_QSetFormat_Widget = new QSetFormat_Widget;
	m_FileDirSet_Widget = new FileDirSet_Widget;

	centerLayout->addWidget(m_FileDirSet_Widget);
	centerLayout->addWidget(m_QSetFormat_Widget);
	centerLayout->addWidget(m_QParseDcmLibSet_Widget);

	processButton = new QPushButton;
	processButton->setText("Process examples");
	centerLayout->addWidget(processButton);
}

void ProcessDockWidget::setupFrames() {

	framesLayout = new QGridLayout;

	VolumeConvert_Frame = new QVolumeConvert_Frame;
	framesLayout->addWidget(VolumeConvert_Frame, 0, 0);

	VolumeDownSampling_Frame = new QVolumeDownSampling_Frame;
	framesLayout->addWidget(VolumeDownSampling_Frame, 0, 1);

	VolumeRotateAxis_Frame = new QVolumeRotateAxis_Frame;
	framesLayout->addWidget(VolumeRotateAxis_Frame, 1, 0);

	VolumeFlipAxis_Frame = new QVolumeFlipAxis_Frame;
	framesLayout->addWidget(VolumeFlipAxis_Frame, 1, 1);

	VolumeClip_Frame = new QVolumeClip_Frame;
	framesLayout->addWidget(VolumeClip_Frame, 2, 0);

	framesLayout->setColumnStretch(0, 1);
	framesLayout->setColumnStretch(1, 1);

	centerLayout->addLayout(framesLayout);
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
	connect(VolumeConvert_Frame->PbrtToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_PbrtToMhd()));
	

	connect(VolumeConvert_Frame->PngToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_PngsToFeimos()));
	connect(VolumeConvert_Frame->PngToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_PngsToMhd()));
	connect(VolumeConvert_Frame->JpgToFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(process_JpgsToFeimos()));
	connect(VolumeConvert_Frame->JpgToMhd_processButton,
		SIGNAL(clicked()), this, SLOT(process_JpgsToMhd()));

	// down sampling
	connect(VolumeDownSampling_Frame->MhdDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdDownSampling()));
	connect(VolumeDownSampling_Frame->FeimosDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosDownSampling()));
	connect(VolumeDownSampling_Frame->LargeFeimosDownSampling_processButton,
		SIGNAL(clicked()), this, SLOT(process_LargeFeimosDownSampling()));

	// volume process
	connect(VolumeRotateAxis_Frame->MhdRotateAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdRotateAxis()));
	connect(VolumeRotateAxis_Frame->FeimosRotateAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosRotateAxis()));


	connect(VolumeFlipAxis_Frame->MhdFlipAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdFlipAxis()));
	connect(VolumeFlipAxis_Frame->FeimosFlipAxis_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosFlipAxis()));


	connect(VolumeClip_Frame->MhdClip_processButton,
		SIGNAL(clicked()), this, SLOT(process_MhdClip()));
	connect(VolumeClip_Frame->FeimosClip_processButton,
		SIGNAL(clicked()), this, SLOT(process_FeimosClip()));

	// test
	connect(processButton, SIGNAL(clicked()), this, SLOT(setProcess()));

}

#include <string>
#include <iostream>
#include <fstream>

#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>

void ProcessDockWidget::getPredefinedInfo() {
	// Obtain predefined information

	InputFilePath = m_FileDirSet_Widget->getInputFilePath();
	//DebugTextPrintString("inputfilePath: " + inputfilePath);
	InputFolder = m_FileDirSet_Widget->getInputFolder();
	//DebugTextPrintString("inputDir: " + inputDir);
	OutputFolder = m_FileDirSet_Widget->getOutputFolder();
	//DebugTextPrintString("outputDir: " + outputDir);
	OutputFileName = m_FileDirSet_Widget->getOutputFileName();
	//DebugTextPrintString("outputFileName: " + outputFileName);

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
void ProcessDockWidget::process_PbrtToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert (.pbrt) file into (.mhd-.raw) file format.");

	getPredefinedInfo();

	processVolumeData.PbrtMakeMhdFile(
		InputFilePath, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

void ProcessDockWidget::process_PngsToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .png files into Feimos' uncompressed (.feimos,.raw) format.");

	getPredefinedInfo();

	processVolumeData.PngsMakeFeimosFile(
		InputFolder, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_PngsToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .png files into VTK's (.mdh-.raw) format.");

	getPredefinedInfo();

	processVolumeData.PngsMakeMhdFile(
		InputFolder, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_JpgsToFeimos() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .jpg files into Feimos' uncompressed (.feimos,.raw) format.");

	getPredefinedInfo();

	processVolumeData.JpgsMakeFeimosFile(
		InputFolder, OutputFolder, OutputFileName,
		generateFormat);

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_JpgsToMhd() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Convert .jpg files into VTK's (.mdh-.raw) format.");

	getPredefinedInfo();

	processVolumeData.JpgsMakeMhdFile(
		InputFolder, OutputFolder, OutputFileName,
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

	bool ok = VolumeRotateAxis_Frame->getRotateAxis(permute);
	if (ok) {
		processVolumeData.RotateAxisMhdFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, permute);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_FeimosRotateAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Permute axis of .feimos-.raw file.");
	getPredefinedInfo();

	bool ok = VolumeRotateAxis_Frame->getRotateAxis(permute);
	if (ok) {
		processVolumeData.RotateAxisFeimosFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, permute);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

void ProcessDockWidget::process_MhdFlipAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Flip axis of .mhd-.raw file.");
	getPredefinedInfo();

	bool ok = VolumeFlipAxis_Frame->getFlipAxis(flip);
	if (ok) {
		processVolumeData.FlipAxisMhdFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, flip);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_FeimosFlipAxis() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Flip axis of .feimos-.raw file.");
	getPredefinedInfo();

	bool ok = VolumeFlipAxis_Frame->getFlipAxis(flip);
	if (ok) {
		processVolumeData.FlipAxisFeimosFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, flip);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

void ProcessDockWidget::process_MhdClip() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Clip .mhd-.raw file.");
	getPredefinedInfo();

	bool isOk = VolumeClip_Frame->getClipRange(clipBegin, clipEnd);
	if (isOk) {
		processVolumeData.ClipMhdFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, clipBegin, clipEnd);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}
void ProcessDockWidget::process_FeimosClip() {
	DebugTextPrintLineBreak();
	DebugTextPrintString("........................  New Task   ................................");

	DebugTextPrintString("Clip .feimos-.raw file.");
	getPredefinedInfo();
	
	bool isOk = VolumeClip_Frame->getClipRange(clipBegin, clipEnd);
	if (isOk) {
		processVolumeData.ClipFeimosFile(
			InputFilePath, OutputFolder, OutputFileName,
			generateFormat, clipBegin, clipEnd);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}

// QVolumeConvert_Frame

QVolumeConvert_Frame::QVolumeConvert_Frame(QWidget * parent) {
	setTitle("DCM Sequence to Volume");
	setMinimumWidth(200);

	VolumeConvert_Layoput = new QGridLayout;
	setLayout(VolumeConvert_Layoput);

	DcmToMhd_processButton = new QPushButton;
	DcmToMhd_processButton->setText("Dcms To Mhd format");

	DcmToFeimos_processButton = new QPushButton;
	DcmToFeimos_processButton->setText("Dcms To Feimos format");

	MhdToFeimos_processButton = new QPushButton;
	MhdToFeimos_processButton->setText("Mhd To Feimos format");

	FeimosToMhd_processButton = new QPushButton;
	FeimosToMhd_processButton->setText("Feimos To Mhd format");

	PngToMhd_processButton = new QPushButton;
	PngToMhd_processButton->setText("PNGs To Mhd format");
	PngToFeimos_processButton = new QPushButton;
	PngToFeimos_processButton->setText("PNGs To Feimos format");

	JpgToMhd_processButton = new QPushButton;
	JpgToMhd_processButton->setText("JPGs To Mhd format");
	JpgToFeimos_processButton = new QPushButton;
	JpgToFeimos_processButton->setText("JPGs To Feimos format");

	PbrtToMhd_processButton = new QPushButton;
	PbrtToMhd_processButton->setText("Pbrt To Mhd format");

	VolumeConvert_Layoput->addWidget(DcmToMhd_processButton, 0, 0);
	VolumeConvert_Layoput->addWidget(DcmToFeimos_processButton, 0, 1);
	VolumeConvert_Layoput->addWidget(MhdToFeimos_processButton, 1, 0);
	VolumeConvert_Layoput->addWidget(FeimosToMhd_processButton, 1, 1);
	VolumeConvert_Layoput->addWidget(PngToMhd_processButton, 2, 0);
	VolumeConvert_Layoput->addWidget(PngToFeimos_processButton, 2, 1);
	VolumeConvert_Layoput->addWidget(JpgToMhd_processButton, 3, 0);
	VolumeConvert_Layoput->addWidget(JpgToFeimos_processButton, 3, 1);
	VolumeConvert_Layoput->addWidget(PbrtToMhd_processButton, 4, 0);
}
QVolumeConvert_Frame::~QVolumeConvert_Frame() { }

// QVolumeDownSampling_Frame

QVolumeDownSampling_Frame::QVolumeDownSampling_Frame(QWidget * parent) {
	setTitle("DownSampling Volume");
	setMinimumWidth(200);

	DownSampling_Layoput = new QVBoxLayout;
	setLayout(DownSampling_Layoput);

	Interval_Label = new QLabel;
	Interval_Label->setText("Interval:");
	Interval_Slider = new QSlider(Qt::Horizontal);
	Interval_Slider->setRange(1, 100);
	Interval_SpinBox = new QSpinBox;
	Interval_SpinBox->setRange(1, 100);
	Interval_Layout = new QGridLayout;
	Interval_Layout->addWidget(Interval_Label, 0, 0);
	Interval_Layout->addWidget(Interval_Slider, 0, 1);
	Interval_Layout->addWidget(Interval_SpinBox, 0, 2);

	MhdDownSampling_processButton = new QPushButton;
	MhdDownSampling_processButton->setText("DownSampling .mhd");

	FeimosDownSampling_processButton = new QPushButton;
	FeimosDownSampling_processButton->setText("DownSampling .feimos");

	LargeFeimosDownSampling_processButton = new QPushButton;
	LargeFeimosDownSampling_processButton->setText("DownSampling large .feimos");

	DownSampling_Layoput->addLayout(Interval_Layout);
	DownSampling_Layoput->addWidget(MhdDownSampling_processButton);
	DownSampling_Layoput->addWidget(FeimosDownSampling_processButton);
	DownSampling_Layoput->addWidget(LargeFeimosDownSampling_processButton);

	connect(Interval_Slider, SIGNAL(valueChanged(int)), Interval_SpinBox, SLOT(setValue(int)));
	connect(Interval_SpinBox, SIGNAL(valueChanged(int)), Interval_Slider, SLOT(setValue(int)));
	Interval_Slider->setValue(2);
}
QVolumeDownSampling_Frame::~QVolumeDownSampling_Frame() {
	disconnect(Interval_Slider, SIGNAL(valueChanged(int)), Interval_SpinBox, SLOT(setValue(int)));
	disconnect(Interval_SpinBox, SIGNAL(valueChanged(int)), Interval_Slider, SLOT(setValue(int)));
}

// QVolumeRotateAxis_Frame

QVolumeRotateAxis_Frame::QVolumeRotateAxis_Frame(QWidget * parent) {
	setTitle("MHD Rotate Axis");
	setMinimumWidth(200);

	VolumeRotateAxis_Layoput = new QVBoxLayout;
	setLayout(VolumeRotateAxis_Layoput);

	permuteLabel = new QLabel;
	permuteLabel->setText("Rotate Axis(Example: 2,0,1)");
	permuteEdit = new QLineEdit;
	permuteEdit->setText("0,1,2");
	VolumeRotateAxis_Edit_Layout = new QGridLayout;
	VolumeRotateAxis_Edit_Layout->addWidget(permuteLabel, 0, 0);
	VolumeRotateAxis_Edit_Layout->addWidget(permuteEdit, 0, 1);

	MhdRotateAxis_processButton = new QPushButton;
	MhdRotateAxis_processButton->setText("Rotate Mhd");
	FeimosRotateAxis_processButton = new QPushButton;
	FeimosRotateAxis_processButton->setText("Rotate Feimos");
	process_Layout = new QGridLayout;
	process_Layout->addWidget(MhdRotateAxis_processButton, 0, 0);
	process_Layout->addWidget(FeimosRotateAxis_processButton, 0, 1);

	VolumeRotateAxis_Layoput->addLayout(VolumeRotateAxis_Edit_Layout);
	VolumeRotateAxis_Layoput->addLayout(process_Layout);
}
QVolumeRotateAxis_Frame::~QVolumeRotateAxis_Frame() { }

bool QVolumeRotateAxis_Frame::getRotateAxis(int permute[3]) {

	QString permu = permuteEdit->text();
	QStringList splitList = permu.split(",");
	if (splitList.size() != 3) {
		DebugTextPrintString("Incorrect input axis.");
		return false;
	}

	bool isOk = true;
	for (int i = 0; i < splitList.size(); ++i) {
		bool ok;
		permute[i] = splitList[i].toInt(&ok);
		isOk = isOk && ok;
	}
	if (isOk) {
		if (permute[0] < 0 || permute[0] > 2) isOk = false;
		if (permute[1] < 0 || permute[1] > 2) isOk = false;
		if (permute[2] < 0 || permute[2] > 2) isOk = false;

		if (permute[0] == permute[1]) isOk = false;
		if (permute[0] == permute[2]) isOk = false;
		if (permute[1] == permute[2]) isOk = false;
	}

	if (!isOk) {
		DebugTextPrintString("Incorrect input axis.");
	}

	return isOk;
}

// QVolumeFlipAxis_Frame

QVolumeFlipAxis_Frame::QVolumeFlipAxis_Frame(QWidget * parent) {
	setTitle("MHD Flip Axis");
	setMinimumWidth(200);

	VolumeFlipAxis_Layoput = new QVBoxLayout;
	setLayout(VolumeFlipAxis_Layoput);

	VolumeFlipAxis_Label_X = new QLabel;
	VolumeFlipAxis_Label_Y = new QLabel;
	VolumeFlipAxis_Label_Z = new QLabel;
	VolumeFlipAxis_Radio_X = new QRadioButton;
	VolumeFlipAxis_Radio_Y = new QRadioButton;
	VolumeFlipAxis_Radio_Z = new QRadioButton;
	VolumeFlipAxis_ButtonGroup = new QButtonGroup(this);
	VolumeFlipAxis_ButtonGroup->addButton(VolumeFlipAxis_Radio_X);
	VolumeFlipAxis_ButtonGroup->addButton(VolumeFlipAxis_Radio_Y);
	VolumeFlipAxis_ButtonGroup->addButton(VolumeFlipAxis_Radio_Z);
	VolumeFlipAxis_EditLayout = new QGridLayout;
	VolumeFlipAxis_Label_X->setText("X:");
	VolumeFlipAxis_Label_Y->setText("Y:");
	VolumeFlipAxis_Label_Z->setText("Z:");
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Label_X, 0, 0);
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Label_Y, 0, 2);
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Label_Z, 0, 4);
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Radio_X, 0, 1);
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Radio_Y, 0, 3);
	VolumeFlipAxis_EditLayout->addWidget(VolumeFlipAxis_Radio_Z, 0, 5);

	MhdFlipAxis_processButton = new QPushButton;
	MhdFlipAxis_processButton->setText("Flip Mhd");
	FeimosFlipAxis_processButton = new QPushButton;
	FeimosFlipAxis_processButton->setText("Flip Feimos");
	process_Layout = new QGridLayout;
	process_Layout->addWidget(MhdFlipAxis_processButton, 0, 0);
	process_Layout->addWidget(FeimosFlipAxis_processButton, 0, 1);

	VolumeFlipAxis_Layoput->addLayout(VolumeFlipAxis_EditLayout);
	VolumeFlipAxis_Layoput->addLayout(process_Layout);
}
QVolumeFlipAxis_Frame::~QVolumeFlipAxis_Frame() { }

bool QVolumeFlipAxis_Frame::getFlipAxis(int flip[3]) {

	if (VolumeFlipAxis_Radio_X->isChecked()) flip[0] = 1;
	else  flip[0] = 0;
	if (VolumeFlipAxis_Radio_Y->isChecked()) flip[1] = 1;
	else  flip[1] = 0;
	if (VolumeFlipAxis_Radio_Z->isChecked()) flip[2] = 1;
	else  flip[2] = 0;

	return true;
}

// QVolumeClip_Frame

QVolumeClip_Frame::QVolumeClip_Frame(QWidget * parent) {
	setTitle("MHD Clip");
	setMinimumWidth(200);

	VolumeClip_Layoput = new QVBoxLayout;
	setLayout(VolumeClip_Layoput);

	VolumeClip_Label_X = new QLabel;
	VolumeClip_Label_X->setText("X:");
	VolumeClip_Lower_X = new QLineEdit;
	VolumeClip_Lower_X->setText("0.0");
	VolumeClip_Upper_X = new QLineEdit;
	VolumeClip_Upper_X->setText("1.0");
	VolumeClip_Label_Y = new QLabel;
	VolumeClip_Label_Y->setText("Y:");
	VolumeClip_Lower_Y = new QLineEdit;
	VolumeClip_Lower_Y->setText("0.0");
	VolumeClip_Upper_Y = new QLineEdit;
	VolumeClip_Upper_Y->setText("1.0");
	VolumeClip_Label_Z = new QLabel;
	VolumeClip_Label_Z->setText("Z:");
	VolumeClip_Lower_Z = new QLineEdit;
	VolumeClip_Lower_Z->setText("0.0");
	VolumeClip_Upper_Z = new QLineEdit;
	VolumeClip_Upper_Z->setText("1.0");

	xbegin = new QLabel; xbegin->setText("xbegin");
	xend = new QLabel; xend->setText("xbegin");
	ybegin = new QLabel; ybegin->setText("ybegin");
	yend = new QLabel; yend->setText("yend");
	zbegin = new QLabel; zbegin->setText("zbegin");
	zend = new QLabel; zend->setText("zend");

	VolumeClip_EditLayout = new QGridLayout;
	VolumeClip_EditLayout->addWidget(VolumeClip_Label_X, 0, 0);
	VolumeClip_EditLayout->addWidget(xbegin, 0, 1);
	VolumeClip_EditLayout->addWidget(VolumeClip_Lower_X, 0, 2);
	VolumeClip_EditLayout->addWidget(xend, 0, 3);
	VolumeClip_EditLayout->addWidget(VolumeClip_Upper_X, 0, 4);

	VolumeClip_EditLayout->addWidget(VolumeClip_Label_Y, 1, 0);
	VolumeClip_EditLayout->addWidget(ybegin, 1, 1);
	VolumeClip_EditLayout->addWidget(VolumeClip_Lower_Y, 1, 2);
	VolumeClip_EditLayout->addWidget(yend, 1, 3);
	VolumeClip_EditLayout->addWidget(VolumeClip_Upper_Y, 1, 4);

	VolumeClip_EditLayout->addWidget(VolumeClip_Label_Z, 2, 0);
	VolumeClip_EditLayout->addWidget(zbegin, 2, 1);
	VolumeClip_EditLayout->addWidget(VolumeClip_Lower_Z, 2, 2);
	VolumeClip_EditLayout->addWidget(zend, 2, 3);
	VolumeClip_EditLayout->addWidget(VolumeClip_Upper_Z, 2, 4);

	VolumeClip_EditLayout->setColumnMinimumWidth(0, 30);
	VolumeClip_EditLayout->setColumnMinimumWidth(1, 50);
	VolumeClip_EditLayout->setColumnMinimumWidth(2, 50);
	VolumeClip_EditLayout->setColumnStretch(0, 1);
	VolumeClip_EditLayout->setColumnStretch(1, 1);
	VolumeClip_EditLayout->setColumnStretch(2, 3);
	VolumeClip_EditLayout->setColumnStretch(3, 1);
	VolumeClip_EditLayout->setColumnStretch(4, 3);

	MhdClip_processButton = new QPushButton;
	MhdClip_processButton->setText("Clip Mhd");
	FeimosClip_processButton = new QPushButton;
	FeimosClip_processButton->setText("Clip Feimos");
	process_Layout = new QGridLayout;
	process_Layout->addWidget(MhdClip_processButton, 0, 0);
	process_Layout->addWidget(FeimosClip_processButton, 0, 1);

	VolumeClip_Layoput->addLayout(VolumeClip_EditLayout);
	VolumeClip_Layoput->addLayout(process_Layout);
}
QVolumeClip_Frame::~QVolumeClip_Frame() { }

bool QVolumeClip_Frame::getClipRange(double begin[3], double end[3]) {
	bool isOk = true;
	QString xbeginValue = VolumeClip_Lower_X->text();
	{
		bool ok;
		begin[0] = xbeginValue.toDouble(&ok);
		isOk = isOk && ok;
	}
	QString ybeginValue = VolumeClip_Lower_Y->text();
	{
		bool ok;
		begin[1] = ybeginValue.toDouble(&ok);
		isOk = isOk && ok;
	}
	QString zbeginValue = VolumeClip_Lower_Z->text();
	{
		bool ok;
		begin[2] = zbeginValue.toDouble(&ok);
		isOk = isOk && ok;
	}

	QString xendValue = VolumeClip_Upper_X->text();
	{
		bool ok;
		end[0] = xendValue.toDouble(&ok);
		isOk = isOk && ok;
	}
	QString yendValue = VolumeClip_Upper_Y->text();
	{
		bool ok;
		end[1] = yendValue.toDouble(&ok);
		isOk = isOk && ok;
	}
	QString zendValue = VolumeClip_Upper_Z->text();
	{
		bool ok;
		end[2] = zendValue.toDouble(&ok);
		isOk = isOk && ok;
	}

	if (isOk) {
		for (int i = 0; i < 3; i++) {
			if (begin[i] < 0.0) begin[i] = 0.0;
			if (begin[i] > 1.0) begin[i] = 1.0;
			if (end[i] < 0.0) end[i] = 0.0;
			if (end[i] > 1.0) end[i] = 1.0;

			if (begin[i] >= end[i]) {
				isOk = false;
			}
		}
	}

	if (!isOk) {
		DebugTextPrintString("Incorrect input axis.");
	}
	return isOk;
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
	
		//processVolumeData.MhdClip_OldFunc(InputFilePath, OutputFolder, OutputFileName, , );
	
		processVolumeData.DownSamplingMhdWithInterval_OldFunc(InputFilePath, OutputFolder, OutputFileName, interval);
	}

	DebugTextPrintString(".................  Process finished   ....................");
	showMemoryInfo();
}













