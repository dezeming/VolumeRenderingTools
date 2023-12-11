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

// DisplayDockWidget

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
	framebuffer.clear();

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

	disconnect(m_QVolumeStatistics_Widget->getOriginInfo_processButton,
		SIGNAL(clicked()), this, SLOT(getStatistics()));

	disconnect(m_QVolumeDisplayRange_Widget,
		SIGNAL(dataRangeChanged()), this, SLOT(setCurrentDisplay()));
	disconnect(m_QVolumeDisplayRange_Widget->writeToDataButton,
		SIGNAL(clicked()), this, SLOT(saveChangeToFile()));
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

	m_QVolumeDisplayRange_Widget = new QVolumeDisplayRange_Widget;
	centerLayout->addWidget(m_QVolumeDisplayRange_Widget);
	connect(m_QVolumeDisplayRange_Widget,
		SIGNAL(dataRangeChanged()), this, SLOT(setCurrentDisplay()));
	connect(m_QVolumeDisplayRange_Widget,
		SIGNAL(dataDisplayChanged()), this, SLOT(setCurrentDisplay()));
	connect(m_QVolumeDisplayRange_Widget->writeToDataButton,
		SIGNAL(clicked()), this, SLOT(saveChangeToFile()));
	


}

void showMemoryInfo(void);
// read and write slots
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

void DisplayDockWidget::getStatistics() {
	DebugTextPrintLineBreak();
	bool flag = volumeData.getVolumeStatistics();
	if (!flag) {
		DebugTextPrintErrorString("Failed to generate statistical data");
		NoStatisticsDisableWidgets();
		return;
	}

	HaveStatisticsDataEnableWidgets(volumeData);

	displayVolumeStatistics();

	setCurrentDisplay();
}

#include "InfoPresent/Status.hpp"
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

inline float FloatDataClampped(const float&min, const float&max, const float& data) {
	return std::fmin(max, std::fmax(min, data));
}
inline float FloatDataClampped_Upper2Min(const float&min, const float&max, const float& data) {
		return (std::fmax(min, data) > max) ? min : std::fmax(min, data);
}
void DisplayDockWidget::setCurrentDisplay() {

	if (volumeData.format != Dez_Float) {
		DebugTextPrintErrorString("Only Float format data is supported to process.");
		return;
	}

	float min, max, range, rangeInv;
	m_QVolumeDisplayRange_Widget->getDataMinMax(min, max);
	range = max - min;
	if (0 == range) range = 1.0f;
	rangeInv = 1.0f / range;

	unsigned int width, height;
	unsigned int axis = m_QVolumeDisplayRange_Widget->getDisplayedAxis();
	unsigned int imageNum = m_QVolumeDisplayRange_Widget->getDisplayedImageNum();
	float *data = static_cast<float *>(volumeData.data);
	if (0 == axis) {
		width = volumeData.xResolution;
		height = volumeData.yResolution;

		framebuffer.resizeBuffer(width, height, 3);

		for (unsigned int i = 0;i < width; i++) {
			for (unsigned int j = 0;j < height; j++) {
				float voxel = FloatDataClampped_Upper2Min(min, max, volumeData.__getPixel(static_cast<float>(1), i, j, imageNum));
				unsigned char pixelValue = static_cast<unsigned char>((voxel - min) * rangeInv * 255.9f);

				framebuffer.fast_set_uc(i, j, 0, pixelValue);
				framebuffer.fast_set_uc(i, j, 1, pixelValue);
				framebuffer.fast_set_uc(i, j, 2, pixelValue);
			}
		}
	}
	else if (1 == axis) {
		width = volumeData.yResolution;
		height = volumeData.zResolution;
		framebuffer.resizeBuffer(width, height, 3);

		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				float voxel = FloatDataClampped_Upper2Min(min, max, volumeData.__getPixel(static_cast<float>(1), imageNum, i, j));
				unsigned char pixelValue = static_cast<unsigned char>((voxel - min) * rangeInv * 255.9f);

				framebuffer.fast_set_uc(i, j, 0, pixelValue);
				framebuffer.fast_set_uc(i, j, 1, pixelValue);
				framebuffer.fast_set_uc(i, j, 2, pixelValue);
			}
		}
	}
	else if (2 == axis) {
		width = volumeData.xResolution;
		height = volumeData.zResolution;
		framebuffer.resizeBuffer(width, height, 3);

		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				float voxel = FloatDataClampped_Upper2Min(min, max, volumeData.__getPixel(static_cast<float>(1), i, imageNum, j));
				unsigned char pixelValue = static_cast<unsigned char>((voxel - min) * rangeInv * 255.9f);

				framebuffer.fast_set_uc(i, j, 0, pixelValue);
				framebuffer.fast_set_uc(i, j, 1, pixelValue);
				framebuffer.fast_set_uc(i, j, 2, pixelValue);
			}
		}
	}

	if (!framebuffer.ubuffer) {
		DebugTextPrintErrorString("framebuffer has no data");
		return;
	}
	emit m_GuiStatus.PaintBuffer_signal(framebuffer.ubuffer, framebuffer.width, framebuffer.height, framebuffer.channals);
}
inline float FloatDataOuter2Aim(const float&min, const float&max, const float& aim, const float& data) {
	float data_t = data;
	if (data > max || data < min) data_t = aim;
	return data_t;
}
void DisplayDockWidget::saveChangeToFile() {

	if (volumeData.format != Dez_Float) {
		DebugTextPrintErrorString("Only Float format data is supported to process.");
		return;
	}

	float min, max, aim = volumeData.statistics.min;
	m_QVolumeDisplayRange_Widget->getDataMinMax(min, max);

	float *data = static_cast<float *>(volumeData.data);
	unsigned int width = volumeData.xResolution;
	unsigned int height = volumeData.yResolution;
	unsigned int imageNum = volumeData.zResolution;

	for (unsigned int k = 0; k < imageNum; k++) {
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				float voxel = FloatDataOuter2Aim(min, max, aim, volumeData.__getPixel(static_cast<float>(1), i, j, k));
				volumeData.__setPixel(i, j, k, voxel);
			}
		}
	}

	getStatistics();
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

// QVolumeReadWrite_Widget

QVolumeReadWrite_Widget::QVolumeReadWrite_Widget(QWidget * parent) {
	setTitle("Basic function");
	setMinimumWidth(200);

	centerLayout = new QVBoxLayout;

	readDataLayout = new QGridLayout;
	setup_readwrite_Widgets();

	setLayout(centerLayout);
	centerLayout->addLayout(readDataLayout);

	NoDataDisableWidgets();
}
QVolumeReadWrite_Widget::~QVolumeReadWrite_Widget() { }
void QVolumeReadWrite_Widget::setup_readwrite_Widgets() {
	readDcms_processButton = new QPushButton;
	readDcms_processButton->setText("Read Dcm files");
	readMhd_processButton = new QPushButton;
	readMhd_processButton->setText("Read Mhd file");
	readFeimos_processButton = new QPushButton;
	readFeimos_processButton->setText("Read Feimos file");
	readDataLayout->addWidget(readDcms_processButton, 0, 0);
	readDataLayout->addWidget(readMhd_processButton, 0, 1);
	readDataLayout->addWidget(readFeimos_processButton, 0, 2);
	writeMhd_processButton = new QPushButton;
	writeMhd_processButton->setText("Write Mhd files");
	writeFeimos_processButton = new QPushButton;
	writeFeimos_processButton->setText("Write Feimos file");
	readDataLayout->addWidget(writeMhd_processButton, 2, 0);
	readDataLayout->addWidget(writeFeimos_processButton, 2, 1);

	readPngs_processButton = new QPushButton;
	readJpgs_processButton = new QPushButton;
	readDataLayout->addWidget(readPngs_processButton, 1, 0);
	readDataLayout->addWidget(readJpgs_processButton, 1, 1);
	readPngs_processButton->setText("Read Png files");
	readJpgs_processButton->setText("Read JPG files");
}

void QVolumeReadWrite_Widget::NoDataDisableWidgets() {
	writeMhd_processButton->setDisabled(true);
	writeFeimos_processButton->setDisabled(true);
	NoStatisticsDisableWidgets();
}
void QVolumeReadWrite_Widget::NoStatisticsDisableWidgets() {
	;
}
void QVolumeReadWrite_Widget::HaveDataEnableWidgets() {
	writeMhd_processButton->setEnabled(true);
	writeFeimos_processButton->setEnabled(true);
}
void QVolumeReadWrite_Widget::HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
	;
}


// QVolumeStatistics_Widget

QVolumeStatistics_Widget::QVolumeStatistics_Widget(QWidget * parent) {
	setTitle("Statistics");
	setMinimumWidth(200);

	centerLayout = new QVBoxLayout;

	GetStatisticsLayout = new QGridLayout;
	getOriginInfo_processButton = new QPushButton;
	getOriginInfo_processButton->setText("Generate Data Info");

	GetStatisticsLayout->addWidget(getOriginInfo_processButton, 0, 0);

	ScaleTheDataValue_Layout = new QGridLayout;
	setupScale_Widgets();

	Clamp_Layout = new QGridLayout;
	setupClamp_Widgets();

	setLayout(centerLayout);
	centerLayout->addLayout(GetStatisticsLayout);
	centerLayout->addLayout(ScaleTheDataValue_Layout);
	centerLayout->addLayout(Clamp_Layout);

	NoDataDisableWidgets();
}
QVolumeStatistics_Widget::~QVolumeStatistics_Widget() {
	disconnect(Scale_Slider, SIGNAL(valueChanged(double)), Scale_SpinBox, SLOT(setValue(double)));
	disconnect(Scale_SpinBox, SIGNAL(valueChanged(double)), Scale_Slider, SLOT(setValue(double)));

	disconnect(ClampRangeMin_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setLowerValue(double)));
	disconnect(ClampRangeMax_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setUpperValue(double)));
	disconnect(ClampRangeSlider, SIGNAL(lowerValueChanged(double)), ClampRangeMin_SpinBox, SLOT(setValue(double)));
	disconnect(ClampRangeSlider, SIGNAL(upperValueChanged(double)), ClampRangeMax_SpinBox, SLOT(setValue(double)));
}
void QVolumeStatistics_Widget::setupScale_Widgets() {
	Scale_processButton = new QPushButton;
	Scale_Slider = new QDoubleSlider;
	Scale_SpinBox = new QDoubleSpinBox;
	ScaleTheDataValue_Layout->addWidget(Scale_processButton, 0, 0);
	Scale_processButton->setText("Scale the value");
	ScaleTheDataValue_Layout->addWidget(Scale_Slider, 0, 1);
	ScaleTheDataValue_Layout->addWidget(Scale_SpinBox, 0, 2);
	Scale_Slider->setRange(0.1, 10.0f);
	Scale_SpinBox->setRange(0.1, 10.0f);
	connect(Scale_Slider, SIGNAL(valueChanged(double)), Scale_SpinBox, SLOT(setValue(double)));
	connect(Scale_SpinBox, SIGNAL(valueChanged(double)), Scale_Slider, SLOT(setValue(double)));
	Scale_Slider->setValue(1.0f);
	Scale_SpinBox->setSingleStep(0.1);
}
void QVolumeStatistics_Widget::setupClamp_Widgets() {
	Clamp_Button = new QPushButton;
	Clamp_Button->setText("Clamp");
	ClampRangeSlider = new QDoubleSliderDoubleRange(Qt::Horizontal, QDoubleSliderDoubleRange::Option::DoubleHandles, nullptr);
	ClampRangeMin_SpinBox = new QDoubleSpinBox;
	ClampRangeMax_SpinBox = new QDoubleSpinBox;
	Clamp_Layout->addWidget(Clamp_Button, 0, 0);
	Clamp_Layout->addWidget(ClampRangeMin_SpinBox, 0, 1);
	Clamp_Layout->addWidget(ClampRangeSlider, 0, 2);
	Clamp_Layout->addWidget(ClampRangeMax_SpinBox, 0, 3);
	connect(ClampRangeMin_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setLowerValue(double)));
	connect(ClampRangeMax_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setUpperValue(double)));
	connect(ClampRangeSlider, SIGNAL(lowerValueChanged(double)), ClampRangeMin_SpinBox, SLOT(setValue(double)));
	connect(ClampRangeSlider, SIGNAL(upperValueChanged(double)), ClampRangeMax_SpinBox, SLOT(setValue(double)));
	Clamp_Layout->setColumnStretch(0, 2);
	Clamp_Layout->setColumnStretch(1, 1);
	Clamp_Layout->setColumnStretch(2, 5);
	Clamp_Layout->setColumnStretch(3, 1);
}

void QVolumeStatistics_Widget::NoDataDisableWidgets() {
	getOriginInfo_processButton->setDisabled(true);
	NoStatisticsDisableWidgets();
}
void QVolumeStatistics_Widget::NoStatisticsDisableWidgets() {
	Scale_processButton->setDisabled(true);
	Scale_Slider->setDisabled(true);
	Scale_SpinBox->setDisabled(true);

	Clamp_Button->setDisabled(true);
	ClampRangeMin_SpinBox->setDisabled(true);
	ClampRangeSlider->setDisabled(true);
	ClampRangeMax_SpinBox->setDisabled(true);
}
void QVolumeStatistics_Widget::HaveDataEnableWidgets() {
	getOriginInfo_processButton->setEnabled(true);
}
void QVolumeStatistics_Widget::HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
	getOriginInfo_processButton->setEnabled(true);

	Scale_Slider->setRange(0.1, 10.0f);
	Scale_SpinBox->setRange(0.1, 10.0f);
	Scale_Slider->setValue(1.0f);
	Scale_processButton->setEnabled(true);
	Scale_Slider->setEnabled(true);
	Scale_SpinBox->setEnabled(true);

	ClampRangeSlider->SetRange(volumeData.statistics.min, volumeData.statistics.max);
	ClampRangeSlider->SetLowerValue(volumeData.statistics.min);
	ClampRangeSlider->SetUpperValue(volumeData.statistics.max);
	ClampRangeMin_SpinBox->setRange(volumeData.statistics.min, volumeData.statistics.max);
	ClampRangeMin_SpinBox->setValue(volumeData.statistics.min);
	ClampRangeMax_SpinBox->setRange(volumeData.statistics.min, volumeData.statistics.max);
	ClampRangeMax_SpinBox->setValue(volumeData.statistics.max);
	Clamp_Button->setEnabled(true);
	ClampRangeMin_SpinBox->setEnabled(true);
	ClampRangeSlider->setEnabled(true);
	ClampRangeMax_SpinBox->setEnabled(true);
}

// QVolumeDisplayRange_Widget

QVolumeDisplayRange_Widget::QVolumeDisplayRange_Widget(QWidget * parent) {
	setTitle("Data Display Range");
	setMinimumWidth(200);

	centerLayout = new QVBoxLayout;

	WWWL_Layout = new QGridLayout;
	DataRange_Layout = new QGridLayout;
	setupWWWL_Widgets();

	axis_Layout = new QGridLayout;
	setupAxis_Widgets();

	changedataLayout = new QGridLayout;
	setupChangeDataWidgets();

	setLayout(centerLayout);
	centerLayout->addLayout(WWWL_Layout);
	centerLayout->addLayout(DataRange_Layout);
	centerLayout->addLayout(axis_Layout);
	centerLayout->addLayout(changedataLayout);

	NoDataDisableWidgets();
}
QVolumeDisplayRange_Widget::~QVolumeDisplayRange_Widget() {
	disconnect(WW_Slider, SIGNAL(valueChanged(double)), WW_SpinBox, SLOT(setValue(double)));
	disconnect(WW_SpinBox, SIGNAL(valueChanged(double)), WW_Slider, SLOT(setValue(double)));
	disconnect(WL_Slider, SIGNAL(valueChanged(double)), WL_SpinBox, SLOT(setValue(double)));
	disconnect(WL_SpinBox, SIGNAL(valueChanged(double)), WL_Slider, SLOT(setValue(double)));

	disconnect(dataRangeMin_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setLowerValue(double)));
	disconnect(dataRangeMax_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setUpperValue(double)));
	disconnect(dataRangeSlider, SIGNAL(lowerValueChanged(double)), dataRangeMin_SpinBox, SLOT(setValue(double)));
	disconnect(dataRangeSlider, SIGNAL(upperValueChanged(double)), dataRangeMax_SpinBox, SLOT(setValue(double)));

	disconnect(WW_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_WW(double)));
	disconnect(WL_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_WL(double)));
	disconnect(dataRangeMin_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_DataMin(double)));
	disconnect(dataRangeMax_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_DataMax(double)));

	disconnect(display_axis_xy_slider, SIGNAL(valueChanged(int)), display_axis_xy_spinbox, SLOT(setValue(int)));
	disconnect(display_axis_xy_spinbox, SIGNAL(valueChanged(int)), display_axis_xy_slider, SLOT(setValue(int)));

	disconnect(display_axis_yz_slider, SIGNAL(valueChanged(int)), display_axis_yz_spinbox, SLOT(setValue(int)));
	disconnect(display_axis_yz_spinbox, SIGNAL(valueChanged(int)), display_axis_yz_slider, SLOT(setValue(int)));

	disconnect(display_axis_xz_slider, SIGNAL(valueChanged(int)), display_axis_xz_spinbox, SLOT(setValue(int)));
	disconnect(display_axis_xz_spinbox, SIGNAL(valueChanged(int)), display_axis_xz_slider, SLOT(setValue(int)));

	disconnect(display_axis_xy_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	disconnect(display_axis_yz_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	disconnect(display_axis_xz_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	disconnect(display_axis_xy_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));
	disconnect(display_axis_xz_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));
	disconnect(display_axis_yz_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));
}
void QVolumeDisplayRange_Widget::setupWWWL_Widgets() {

	WW_Button = new QPushButton;
	WW_Button->setText("Window Width");
	WL_Button = new QPushButton;
	WL_Button->setText("Window Level");

	WWWL_Layout->addWidget(WW_Button, 0, 0);
	WWWL_Layout->addWidget(WL_Button, 1, 0);

	WW_Slider = new QDoubleSlider;
	WW_SpinBox = new QDoubleSpinBox;
	WL_Slider = new QDoubleSlider;
	WL_SpinBox = new QDoubleSpinBox;

	WWWL_Layout->addWidget(WW_Slider, 0, 1);
	WWWL_Layout->addWidget(WW_SpinBox, 0, 2);
	WWWL_Layout->addWidget(WL_Slider, 1, 1);
	WWWL_Layout->addWidget(WL_SpinBox, 1, 2);

	connect(WW_Slider, SIGNAL(valueChanged(double)), WW_SpinBox, SLOT(setValue(double)));
	connect(WW_SpinBox, SIGNAL(valueChanged(double)), WW_Slider, SLOT(setValue(double)));
	connect(WL_Slider, SIGNAL(valueChanged(double)), WL_SpinBox, SLOT(setValue(double)));
	connect(WL_SpinBox, SIGNAL(valueChanged(double)), WL_Slider, SLOT(setValue(double)));

	connect(WW_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_WW(double)));
	connect(WL_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_WL(double)));


	dataRange_Button = new QPushButton;
	dataRangeSlider = new QDoubleSliderDoubleRange(Qt::Horizontal, QDoubleSliderDoubleRange::Option::DoubleHandles, nullptr);
	dataRange_Button->setText("Data Range");
	dataRangeMin_SpinBox = new QDoubleSpinBox;
	dataRangeMax_SpinBox = new QDoubleSpinBox;
	DataRange_Layout->addWidget(dataRange_Button, 0, 0);
	DataRange_Layout->addWidget(dataRangeMin_SpinBox, 0, 1);
	DataRange_Layout->addWidget(dataRangeSlider, 0, 2);
	DataRange_Layout->addWidget(dataRangeMax_SpinBox, 0, 3);
	DataRange_Layout->setColumnStretch(0, 2);
	DataRange_Layout->setColumnStretch(1, 1);
	DataRange_Layout->setColumnStretch(2, 5);
	DataRange_Layout->setColumnStretch(3, 1);

	connect(dataRangeMin_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setLowerValue(double)));
	connect(dataRangeMax_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setUpperValue(double)));
	connect(dataRangeSlider, SIGNAL(lowerValueChanged(double)), dataRangeMin_SpinBox, SLOT(setValue(double)));
	connect(dataRangeSlider, SIGNAL(upperValueChanged(double)), dataRangeMax_SpinBox, SLOT(setValue(double)));

	connect(dataRangeMin_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_DataMin(double)));
	connect(dataRangeMax_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(setNewRange_DataMax(double)));
}
void QVolumeDisplayRange_Widget::setupAxis_Widgets() {
	display_axis_xy_label = new QLabel;
	display_axis_xy_label->setText(" X-Y axis");
	display_axis_xy_radio = new QRadioButton;
	display_axis_yz_label = new QLabel;
	display_axis_yz_label->setText(" Y-Z axis");
	display_axis_yz_radio = new QRadioButton;
	display_axis_xz_label = new QLabel;
	display_axis_xz_label->setText(" X-Z axis");
	display_axis_xz_radio = new QRadioButton;
	display_axis_xy_slider = new QSlider(Qt::Horizontal);
	display_axis_xy_spinbox = new QSpinBox;
	display_axis_yz_slider = new QSlider(Qt::Horizontal);
	display_axis_yz_spinbox = new QSpinBox;
	display_axis_xz_slider = new QSlider(Qt::Horizontal);
	display_axis_xz_spinbox = new QSpinBox;

	display_axis_ButtonGroup = new QButtonGroup;
	display_axis_ButtonGroup->addButton(display_axis_xy_radio);
	display_axis_ButtonGroup->addButton(display_axis_yz_radio);
	display_axis_ButtonGroup->addButton(display_axis_xz_radio);


	axis_Layout->addWidget(display_axis_xy_label, 0, 0);
	axis_Layout->addWidget(display_axis_xy_radio, 0, 1);
	axis_Layout->addWidget(display_axis_xy_slider, 0, 2);
	axis_Layout->addWidget(display_axis_xy_spinbox, 0, 3);

	axis_Layout->addWidget(display_axis_yz_label, 1, 0);
	axis_Layout->addWidget(display_axis_yz_radio, 1, 1);
	axis_Layout->addWidget(display_axis_yz_slider, 1, 2);
	axis_Layout->addWidget(display_axis_yz_spinbox, 1, 3);

	axis_Layout->addWidget(display_axis_xz_label, 2, 0);
	axis_Layout->addWidget(display_axis_xz_radio, 2, 1);
	axis_Layout->addWidget(display_axis_xz_slider, 2, 2);
	axis_Layout->addWidget(display_axis_xz_spinbox, 2, 3);

	axis_Layout->setColumnStretch(0, 2);
	axis_Layout->setColumnStretch(1, 1);
	axis_Layout->setColumnStretch(2, 10);
	axis_Layout->setColumnStretch(3, 2);

	connect(display_axis_xy_slider, SIGNAL(valueChanged(int)), display_axis_xy_spinbox, SLOT(setValue(int)));
	connect(display_axis_xy_spinbox, SIGNAL(valueChanged(int)), display_axis_xy_slider, SLOT(setValue(int)));

	connect(display_axis_yz_slider, SIGNAL(valueChanged(int)), display_axis_yz_spinbox, SLOT(setValue(int)));
	connect(display_axis_yz_spinbox, SIGNAL(valueChanged(int)), display_axis_yz_slider, SLOT(setValue(int)));

	connect(display_axis_xz_slider, SIGNAL(valueChanged(int)), display_axis_xz_spinbox, SLOT(setValue(int)));
	connect(display_axis_xz_spinbox, SIGNAL(valueChanged(int)), display_axis_xz_slider, SLOT(setValue(int)));


	connect(display_axis_xy_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	connect(display_axis_yz_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	connect(display_axis_xz_slider, SIGNAL(valueChanged(int)), this, SLOT(setDataDisplayChanged()));
	connect(display_axis_xy_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));
	connect(display_axis_xz_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));
	connect(display_axis_yz_radio, SIGNAL(clicked()), this, SLOT(setDataDisplayChanged()));

}
void QVolumeDisplayRange_Widget::setupChangeDataWidgets() {
	
	writeToDataButton = new QPushButton;
	writeToDataButton->setText("save to source");
	changedataLayout->addWidget(writeToDataButton, 0, 0);

}

void QVolumeDisplayRange_Widget::setDataDisplayChanged() {
	emit dataDisplayChanged();
}

void QVolumeDisplayRange_Widget::NoDataDisableWidgets() {
	NoStatisticsDisableWidgets();
}
void QVolumeDisplayRange_Widget::NoStatisticsDisableWidgets() {
	WW_Button->setDisabled(true);
	WW_Slider->setDisabled(true);
	WW_SpinBox->setDisabled(true);

	WL_Button->setDisabled(true);
	WL_Slider->setDisabled(true);
	WL_SpinBox->setDisabled(true);

	dataRange_Button->setDisabled(true);
	dataRangeSlider->setDisabled(true);
	dataRangeMin_SpinBox->setDisabled(true);
	dataRangeMax_SpinBox->setDisabled(true);

	display_axis_xy_radio->setDisabled(true);
	display_axis_xy_slider->setDisabled(true);
	display_axis_xy_spinbox->setDisabled(true);

	display_axis_yz_radio->setDisabled(true);
	display_axis_yz_slider->setDisabled(true);
	display_axis_yz_spinbox->setDisabled(true);

	display_axis_xz_radio->setDisabled(true);
	display_axis_xz_slider->setDisabled(true);
	display_axis_xz_spinbox->setDisabled(true);
}
void QVolumeDisplayRange_Widget::HaveDataEnableWidgets() {
	;
}
void QVolumeDisplayRange_Widget::HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {

	// data range

	float dataInterval = volumeData.statistics.max - volumeData.statistics.min;

	originDataMin = volumeData.statistics.min;
	originDataMax = volumeData.statistics.max;

	dataRangeSlider->SetRange(volumeData.statistics.min, volumeData.statistics.max);
	dataRangeSlider->SetLowerValue(volumeData.statistics.min);
	dataRangeSlider->SetUpperValue(volumeData.statistics.max);
	dataRangeMin_SpinBox->setRange(volumeData.statistics.min, volumeData.statistics.max);
	dataRangeMin_SpinBox->setValue(volumeData.statistics.min);
	dataRangeMin_SpinBox->setSingleStep(dataInterval * 0.01);
	dataRangeMax_SpinBox->setRange(volumeData.statistics.min, volumeData.statistics.max);
	dataRangeMax_SpinBox->setValue(volumeData.statistics.max);
	dataRangeMax_SpinBox->setSingleStep(dataInterval * 0.01);
	dataRange_Button->setEnabled(true);
	dataRangeSlider->setEnabled(true);
	dataRangeMin_SpinBox->setEnabled(true);
	dataRangeMax_SpinBox->setEnabled(true);

	WW_Slider->setRange(dataInterval * 0.01, dataInterval);
	WW_SpinBox->setRange(dataInterval * 0.01, dataInterval);
	WW_SpinBox->setSingleStep(dataInterval * 0.01);
	WindowWidth = dataInterval;
	WW_SpinBox->setValue(dataInterval);
	WW_Button->setEnabled(true);
	WW_Slider->setEnabled(true);
	WW_SpinBox->setEnabled(true);

	WL_Slider->setRange(volumeData.statistics.min, volumeData.statistics.max);
	WL_SpinBox->setRange(volumeData.statistics.min, volumeData.statistics.max);
	WL_SpinBox->setSingleStep(dataInterval * 0.01);
	WindowLevel = (volumeData.statistics.max + volumeData.statistics.min) * 0.5;
	WL_SpinBox->setValue(WindowLevel);
	WL_Button->setEnabled(true);
	WL_Slider->setEnabled(true);
	WL_SpinBox->setEnabled(true);

	dataMin = volumeData.statistics.min;
	dataMax = volumeData.statistics.max;

	// display axis
	display_axis_xy_radio->setEnabled(true);
	display_axis_xy_slider->setEnabled(true);
	display_axis_xy_spinbox->setEnabled(true);
	display_axis_xy_slider->setRange(0, volumeData.zResolution-1);
	display_axis_xy_spinbox->setRange(0, volumeData.zResolution-1);
	display_axis_xy_slider->setValue(0);

	display_axis_yz_radio->setEnabled(true);
	display_axis_yz_slider->setEnabled(true);
	display_axis_yz_spinbox->setEnabled(true);
	display_axis_yz_slider->setRange(0, volumeData.xResolution-1);
	display_axis_yz_spinbox->setRange(0, volumeData.xResolution-1);
	display_axis_yz_slider->setValue(0);

	display_axis_xz_radio->setEnabled(true);
	display_axis_xz_slider->setEnabled(true);
	display_axis_xz_spinbox->setEnabled(true);
	display_axis_xz_slider->setRange(0, volumeData.yResolution-1);
	display_axis_xz_spinbox->setRange(0, volumeData.yResolution-1);
	display_axis_xz_slider->setValue(0);

	display_axis_xy_radio->setChecked(true);
}

void QVolumeDisplayRange_Widget::setNewRange_WW(double ww) {
	WindowWidth = ww;

	if ((WindowLevel - 0.5 * WindowWidth) < originDataMin) {
		dataMin = originDataMin;
		return;
	}
	else {
		dataMin = WindowLevel - 0.5 * WindowWidth;
	}
	if ((WindowLevel + 0.5 * WindowWidth) > originDataMax) {
		dataMax = originDataMax;
		return;
	}
	else {
		dataMax = WindowLevel + 0.5 * WindowWidth;
	}
	dataRangeSlider->SetLowerValue(dataMin);
	dataRangeSlider->SetUpperValue(dataMax);
	emit dataRangeChanged();
}
void QVolumeDisplayRange_Widget::setNewRange_WL(double wl) {
	WindowLevel = wl;

	if ((WindowLevel - 0.5 * WindowWidth) < originDataMin) {
		dataMin = originDataMin;
		return;
	}
	else {
		dataMin = WindowLevel - 0.5 * WindowWidth;
	}
	if ((WindowLevel + 0.5 * WindowWidth) > originDataMax) {
		dataMax = originDataMax;
		return;
	}
	else {
		dataMax = WindowLevel + 0.5 * WindowWidth;
	}
	dataRangeSlider->SetLowerValue(dataMin);
	dataRangeSlider->SetUpperValue(dataMax);

	emit dataRangeChanged();
}
void QVolumeDisplayRange_Widget::setNewRange_DataMin(double datamin) {
	dataMin = datamin;
	WindowWidth = dataMax - dataMin;
	WL_Slider->setValue((dataMax + dataMin) * 0.5);
	WW_Slider->setValue(dataMax - dataMin);

	emit dataRangeChanged();
}
void QVolumeDisplayRange_Widget::setNewRange_DataMax(double datamax) {
	dataMax = datamax;
	WindowWidth = dataMax - dataMin;
	WL_Slider->setValue((dataMax + dataMin) * 0.5);
	WW_Slider->setValue(dataMax - dataMin);

	emit dataRangeChanged();
}

void QVolumeDisplayRange_Widget::getDataMinMax(float& min, float& max) {
	min = dataMin;
	max = dataMax;
}
unsigned int QVolumeDisplayRange_Widget::getDisplayedAxis() {
	if (display_axis_xy_radio->isChecked()) return 0;
	if (display_axis_yz_radio->isChecked()) return 1;
	if (display_axis_xz_radio->isChecked()) return 2;
	return 0;
}
unsigned int QVolumeDisplayRange_Widget::getDisplayedImageNum() {
	if (display_axis_xy_radio->isChecked()) 
		return static_cast<unsigned int>(display_axis_xy_slider->value());
	if (display_axis_yz_radio->isChecked()) 
		return static_cast<unsigned int>(display_axis_yz_slider->value());
	if (display_axis_xz_radio->isChecked()) 
		return static_cast<unsigned int>(display_axis_xz_slider->value());
	return 0;
}






