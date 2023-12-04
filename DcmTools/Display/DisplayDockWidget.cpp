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
#include <QString>
#include <QTextStream>

DisplayDockWidget::DisplayDockWidget(QWidget * parent) {

	setWindowTitle("Display Interaction");

	setFeatures(QDockWidget::DockWidgetMovable);

	QFile styleFile("./Icons/DisplayDock.qss");
	if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&styleFile);
		// 应用样式表
		setStyleSheet(stream.readAll());
		styleFile.close();
	}
	else {
		DebugTextPrintErrorString("Failed to open stylesheet file for DisplayDockWidget");
	}

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

	disconnect(m_QVolumeReadWrite_Widget->readDcms_processButton,
		SIGNAL(clicked()), this, SLOT(readDcmsData()));
	disconnect(m_QVolumeReadWrite_Widget->readPngs_processButton,
		SIGNAL(clicked()), this, SLOT(readPNGsData()));
	disconnect(m_QVolumeReadWrite_Widget->readJpgs_processButton,
		SIGNAL(clicked()), this, SLOT(readJPGsData()));

	disconnect(m_QVolumeReadWrite_Widget->readMhd_processButton,
		SIGNAL(clicked()), this, SLOT(readMhdData()));
	disconnect(m_QVolumeReadWrite_Widget->readFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(readFeimosData()));

	disconnect(m_QVolumeReadWrite_Widget->writeMhd_processButton,
		SIGNAL(clicked()), this, SLOT(writeMhdData()));
	disconnect(m_QVolumeReadWrite_Widget->writeFeimos_processButton,
		SIGNAL(clicked()), this, SLOT(writeFeimosData()));
}

void DisplayDockWidget::setpuWidgets()
{
	m_FileDirSet_Widget = new FileDirSet_Widget;
	centerLayout->addWidget(m_FileDirSet_Widget);

	m_QSetFormat_Widget = new QSetFormat_Widget;
	centerLayout->addWidget(m_QSetFormat_Widget);

	m_QVolumeReadWrite_Widget = new QVolumeReadWrite_Widget;
	centerLayout->addWidget(m_QVolumeReadWrite_Widget);
	connect(m_QVolumeReadWrite_Widget->readDcms_processButton,
		SIGNAL(clicked()), this, SLOT(readDcmsData()));
	connect(m_QVolumeReadWrite_Widget->readPngs_processButton,
		SIGNAL(clicked()), this, SLOT(readPNGsData()));
	connect(m_QVolumeReadWrite_Widget->readJpgs_processButton,
		SIGNAL(clicked()), this, SLOT(readJPGsData()));

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
	connect(m_QVolumeStatistics_Widget->getOriginInfo_processButton,
		SIGNAL(clicked()), this, SLOT(getStatistics()));

	m_QVolumeWWWL_Widget = new QVolumeWWWL_Widget;
	centerLayout->addWidget(m_QVolumeWWWL_Widget);

}

#include "InfoPresent/Status.hpp"
void showMemoryInfo(void);

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
	
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Float;
	parseFlag = m_DataRW.DataFormatConvertToInteract(generateFormat, volumeData);

	if (parseFlag) {
		displayVolumeInfo();

		HaveDataEnableWidgets();
		NoStatisticsDisableWidgets();
	}
	else {
		NoDataDisableWidgets();
	}
	showMemoryInfo();
}

void DisplayDockWidget::readPNGsData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputDirExist(InputFolder)) {
		showMemoryInfo(); return;
	}

	volumeData.clear();

	std::vector<QString> fileList;
	bool parseFlag = m_DataRW.getInputPngsFileList(InputFolder, fileList);
	if (!parseFlag) {
		clearVolumeInfo();
		showMemoryInfo();
		return;
	}
	parseFlag = m_DataRW.GenerateInput_PNGs(fileList, volumeData);
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Float;
	parseFlag = m_DataRW.DataFormatConvertToInteract(generateFormat, volumeData);

	if (parseFlag) {
		displayVolumeInfo();

		HaveDataEnableWidgets();
		NoStatisticsDisableWidgets();
	}
	else {
		NoDataDisableWidgets();
	}
	showMemoryInfo();
}

void DisplayDockWidget::readJPGsData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputDirExist(InputFolder)) { 
		showMemoryInfo();
		return; 
	}

	volumeData.clear();

	std::vector<QString> fileList;
	bool parseFlag = m_DataRW.getInputJpgsFileList(InputFolder, fileList);
	if (!parseFlag) {
		clearVolumeInfo();
		showMemoryInfo();
		return;
	}
	parseFlag = m_DataRW.GenerateInput_JPGs(fileList, volumeData);
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Float;
	parseFlag = m_DataRW.DataFormatConvertToInteract(generateFormat, volumeData);

	if (parseFlag) {
		displayVolumeInfo();

		HaveDataEnableWidgets();
		NoStatisticsDisableWidgets();
	}
	else {

		NoDataDisableWidgets();
	}
	showMemoryInfo();
}

void DisplayDockWidget::readMhdData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputMhdFileExist(InputFilePath)) { showMemoryInfo(); return; }

	volumeData.clear();
	bool parseFlag = m_DataRW.GenerateInput_Mhd(InputFilePath, volumeData);
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Float;
	parseFlag = m_DataRW.DataFormatConvertToInteract(generateFormat, volumeData);

	if (!parseFlag) {
		clearVolumeInfo();

		NoDataDisableWidgets();
	}
	else {
		displayVolumeInfo();

		HaveDataEnableWidgets();
		NoStatisticsDisableWidgets();
	}
	showMemoryInfo();
}

void DisplayDockWidget::readFeimosData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();

	// check input and output
	if (!m_DataRW.isInputFeimosFileExist(InputFilePath)) { showMemoryInfo(); return; }

	volumeData.clear();
	bool parseFlag = m_DataRW.GenerateInput_Feimos(InputFilePath, volumeData);
	GenerateFormat generateFormat;
	generateFormat.format = Dez_Float;
	parseFlag = m_DataRW.DataFormatConvertToInteract(generateFormat, volumeData);

	if (!parseFlag) {
		clearVolumeInfo();

		NoDataDisableWidgets();
	}
	else {
		displayVolumeInfo();

		HaveDataEnableWidgets();
		NoStatisticsDisableWidgets();
	}
	showMemoryInfo();
}

void DisplayDockWidget::writeMhdData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();
	if (!m_DataRW.checkOutputDir_Mhd(OutputFolder, OutputFileName)) { showMemoryInfo(); return; }
	
	if (generateFormat.format == Dez_Origin) {
		DebugTextPrintWarningString("Due to being converted to Float type before processing, Float type data is generated with format Origin");
	}

	bool parseFlag = m_DataRW.GenerateOutput_Mhd(OutputFolder, OutputFileName, generateFormat, volumeData);
	showMemoryInfo();
}

void DisplayDockWidget::writeFeimosData() {
	DebugTextPrintLineBreak();
	getPredefinedInfo();
	if (!m_DataRW.checkOutputDir_Feimos(OutputFolder, OutputFileName)) {
		showMemoryInfo(); return;
	}

	if (generateFormat.format == Dez_Origin) {
		DebugTextPrintWarningString("Due to being converted to Float type before processing, Float type data is generated with format Origin");
	}
	
	bool parseFlag = m_DataRW.GenerateOutput_Feimos(OutputFolder, OutputFileName, generateFormat, volumeData);
	showMemoryInfo();
}

void DisplayDockWidget::displayVolumeInfo() {
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-x", QString::number(volumeData.xResolution), "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-y", QString::number(volumeData.yResolution), "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-z", QString::number(volumeData.zResolution), "");

	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-x", QString::number(volumeData.xPixelSpace), "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-y", QString::number(volumeData.yPixelSpace), "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-z", QString::number(volumeData.zPixelSpace), "mm");

	m_GuiStatus.setDataChanged("Volume Data", "Data Format", (volumeData.getFormatString()), "");

	m_GuiStatus.setDataChanged("Dicom Orignal Info", "Slope", QString::number(volumeData.slope), "");
	m_GuiStatus.setDataChanged("Dicom Orignal Info", "Intercept", QString::number(volumeData.intercept), "");

	clearVolumeStatistics();
}
void DisplayDockWidget::clearVolumeInfo() {
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-x", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-y", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Resolution-z", "", "");

	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-x", "", "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-y", "", "mm");
	m_GuiStatus.setDataChanged("Volume Data", "VoxelSpace-z", "", "mm");

	m_GuiStatus.setDataChanged("Volume Data", "Data Format", "", "");

	m_GuiStatus.setDataChanged("Dicom Orignal Info", "Slope", "", "");
	m_GuiStatus.setDataChanged("Dicom Orignal Info", "Intercept", "", "");

	clearVolumeStatistics();
}

void DisplayDockWidget::displayVolumeStatistics() {
	m_GuiStatus.setDataChanged("Volume Data", "Data-min", QString::number(volumeData.statistics.min), "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-max", QString::number(volumeData.statistics.max), "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-graient-min", QString::number(volumeData.statistics.gradientMin), "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-graient-max", QString::number(volumeData.statistics.gradientMax), "");
}
void DisplayDockWidget::clearVolumeStatistics() {
	m_GuiStatus.setDataChanged("Volume Data", "Data-min", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-max", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-graient-min", "", "");
	m_GuiStatus.setDataChanged("Volume Data", "Data-graient-max", "", "");
}

void DisplayDockWidget::getStatistics() {
	DebugTextPrintLineBreak();
	bool flag = volumeData.getVolumeStatistics();
	if (!flag) {
		DebugTextPrintErrorString("Failed to generate statistical data");
		return;
	}

	HaveStatisticsDataEnableWidgets(volumeData);

	displayVolumeStatistics();
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

	generateFormat.format = m_QSetFormat_Widget->getDataFormatSet();

}







