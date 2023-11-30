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

#include "DisplayDockWidget.hpp"
#include "Utility/DebugText.hpp"

#include <QFile>
#include <fstream>
#include <string>

DisplayDockWidget::DisplayDockWidget(QWidget * parent) {

	setWindowTitle("Display Interaction");

	setFeatures(QDockWidget::DockWidgetMovable);

	dockCentralWidget = new QFrame;

	setWidget(dockCentralWidget);
	centerLayout = new QVBoxLayout;
	dockCentralWidget->setWindowFlags(Qt::FramelessWindowHint);
	dockCentralWidget->setLayout(centerLayout);

	setpuWidgets();

	setMinimumWidth(200);
}

DisplayDockWidget::~DisplayDockWidget() {

	volumeData.clear();

}

void DisplayDockWidget::setpuWidgets()
{
	m_FileDirSet_Widget = new FileDirSet_Widget;
	centerLayout->addWidget(m_FileDirSet_Widget);

	m_QVolumeReadWrite_Widget = new QVolumeReadWrite_Widget;
	centerLayout->addWidget(m_QVolumeReadWrite_Widget);
	connect(m_QVolumeReadWrite_Widget->readDcms_processButton,
		SIGNAL(clicked()), this, SLOT(readDcmsData()));
	connect(m_QVolumeReadWrite_Widget->readMhd_processButton,
		SIGNAL(clicked()), this, SLOT(readMhdData()));
	connect(m_QVolumeReadWrite_Widget->readFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(readFeimosData()));
	connect(m_QVolumeReadWrite_Widget->writeMhd_processButton,
		SIGNAL(clicked()), this, SLOT(writeMhdData()));
	connect(m_QVolumeReadWrite_Widget->writeFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(writeFeimosData()));

	m_QVolumeStatistics_Widget = new QVolumeStatistics_Widget;
	centerLayout->addWidget(m_QVolumeStatistics_Widget);
}

#include "InfoPresent/Status.hpp"

void DisplayDockWidget::readDcmsData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputDirExist(InputFolder)) return;

	volumeData.clear();

	std::vector<QString> fileList;
	bool parseFlag = m_DataRW.getInputDcmFileList(InputFolder, fileList);
	if (!parseFlag) {
		clearVolumeInfo();
		return;
	}
	parseFlag = m_DataRW.GenerateInput_GDCM(fileList, volumeData);
	if (parseFlag) {
		displayVolumeInfo();
	}
}

void DisplayDockWidget::readMhdData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputMhdFileExist(InputFilePath)) return;

	volumeData.clear();
	bool parseFlag = m_DataRW.GenerateInput_Mhd(InputFilePath, volumeData);
	if (!parseFlag) {
		clearVolumeInfo();
	}
	else {
		displayVolumeInfo();
	}
}

void DisplayDockWidget::readFeimosData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputFeimosFileExist(InputFilePath)) return;

	volumeData.clear();
	bool parseFlag = m_DataRW.GenerateInput_Feimos(InputFilePath, volumeData);
	if (!parseFlag) {
		clearVolumeInfo();
	}
	else {
		displayVolumeInfo();
	}
}

void DisplayDockWidget::writeMhdData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();
	if (!m_DataRW.checkOutputDir_Mhd(OutputFolder, OutputFileName)) return;
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Origin;
	bool parseFlag = m_DataRW.GenerateOutput_Mhd(OutputFolder, OutputFileName, generateFormat, volumeData);
}

void DisplayDockWidget::writeFeimosData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();
	if (!m_DataRW.checkOutputDir_Feimos(OutputFolder, OutputFileName)) return;
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Origin;
	bool parseFlag = m_DataRW.GenerateOutput_Feimos(OutputFolder, OutputFileName, generateFormat, volumeData);
}

void DisplayDockWidget::clearVolumeInfo() {
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-x", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-y", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-z", "", "");

	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-x", "", "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-y", "", "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-z", "", "mm");

	m_GuiStatus.setDataChanged("Volume Data", "Data Format", "", "");
}

void DisplayDockWidget::displayVolumeInfo() {
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-x", QString::number(volumeData.xResolution), "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-y", QString::number(volumeData.yResolution), "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-z", QString::number(volumeData.zResolution), "");

	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-x", QString::number(volumeData.xPixelSpace), "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-y", QString::number(volumeData.yPixelSpace), "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-z", QString::number(volumeData.zPixelSpace), "mm");

	m_GuiStatus.setDataChanged("Volume Data", "Data Format", (volumeData.getFormatString()), "");
}

#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>

void DisplayDockWidget::getPredefinedInfo() {
	// Obtain predefined information

	InputFilePath = m_FileDirSet_Widget->getInputFilePath();
	//DebugTextPrintString("inputfilePath: " + inputfilePath);
	InputFolder = m_FileDirSet_Widget->getInputFolder();
	//DebugTextPrintString("inputDir: " + inputDir);
	OutputFolder = m_FileDirSet_Widget->getOutputFolder();
	//DebugTextPrintString("outputDir: " + outputDir);
	OutputFileName = m_FileDirSet_Widget->getOutputFileName();
	//DebugTextPrintString("outputFileName: " + outputFileName);

}







