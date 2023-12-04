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

#ifndef __DisplayDockWidget_h__
#define __DisplayDockWidget_h__

#include <QDockWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>

#include "Process/DataReaderAndWriter.hpp"

#include "Utility/WidgetsCommon.hpp"
#include "Utility/DicomCommon.hpp"
#include "Utility/QSliderDoubleRange.hpp"

#include "Utility/QDoubleSlider.hpp"
#include "Utility/QDoubleSliderDoubleRange.hpp"

#include <QSlider>
#include <QDoubleSpinBox>

class QVolumeReadWrite_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeReadWrite_Widget(QWidget * parent = Q_NULLPTR) {
		setTitle("Basic function");
		setMinimumWidth(200);

		centerLayout = new QVBoxLayout;

		readDataLayout = new QGridLayout;
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

		setLayout(centerLayout);
		centerLayout->addLayout(readDataLayout);
		
		NoDataDisableWidgets();
	}
	~QVolumeReadWrite_Widget() { }

	QVBoxLayout *centerLayout;

	QGridLayout *readDataLayout;
	QPushButton *readDcms_processButton;
	QPushButton *readMhd_processButton;
	QPushButton *readFeimos_processButton;

	QPushButton *readPngs_processButton;
	QPushButton *readJpgs_processButton;

	QPushButton *writeMhd_processButton;
	QPushButton *writeFeimos_processButton;

	void NoDataDisableWidgets() {
		writeMhd_processButton->setDisabled(true);
		writeFeimos_processButton->setDisabled(true);
		NoStatisticsDisableWidgets();
	}
	void NoStatisticsDisableWidgets() {
		;
	}

	void HaveDataEnableWidgets() {
		writeMhd_processButton->setEnabled(true);
		writeFeimos_processButton->setEnabled(true);
	}
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
		;
	}
};

class QVolumeStatistics_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeStatistics_Widget(QWidget * parent = Q_NULLPTR) {
		setTitle("Statistics");
		setMinimumWidth(200);

		centerLayout = new QVBoxLayout;

		GetStatisticsLayout = new QGridLayout;
		getOriginInfo_processButton = new QPushButton;
		getOriginInfo_processButton->setText("Generate Data Info");
		
		GetStatisticsLayout->addWidget(getOriginInfo_processButton, 0, 0);

		ScaleTheDataValue_Layout = new QGridLayout;
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

		Clamp_Layout = new QGridLayout;
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


		setLayout(centerLayout);
		centerLayout->addLayout(GetStatisticsLayout);
		centerLayout->addLayout(ScaleTheDataValue_Layout);
		centerLayout->addLayout(Clamp_Layout);

		NoDataDisableWidgets();
	}
	~QVolumeStatistics_Widget() { 
		disconnect(Scale_Slider, SIGNAL(valueChanged(double)), Scale_SpinBox, SLOT(setValue(double)));
		disconnect(Scale_SpinBox, SIGNAL(valueChanged(double)), Scale_Slider, SLOT(setValue(double)));

		disconnect(ClampRangeMin_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setLowerValue(double)));
		disconnect(ClampRangeMax_SpinBox, SIGNAL(valueChanged(double)), ClampRangeSlider, SLOT(setUpperValue(double)));
		disconnect(ClampRangeSlider, SIGNAL(lowerValueChanged(double)), ClampRangeMin_SpinBox, SLOT(setValue(double)));
		disconnect(ClampRangeSlider, SIGNAL(upperValueChanged(double)), ClampRangeMax_SpinBox, SLOT(setValue(double)));
	}

	QVBoxLayout *centerLayout;

	QGridLayout *GetStatisticsLayout;
	QPushButton *getOriginInfo_processButton;

	QGridLayout *ScaleTheDataValue_Layout;
	QPushButton *Scale_processButton;
	QDoubleSlider * Scale_Slider;
	QDoubleSpinBox * Scale_SpinBox;

	QGridLayout *Clamp_Layout;
	QPushButton *Clamp_Button;
	QDoubleSliderDoubleRange *ClampRangeSlider;
	QDoubleSpinBox * ClampRangeMin_SpinBox;
	QDoubleSpinBox * ClampRangeMax_SpinBox;

	void NoDataDisableWidgets() {
		getOriginInfo_processButton->setDisabled(true);
		NoStatisticsDisableWidgets();
	}
	void NoStatisticsDisableWidgets() {
		Scale_processButton->setDisabled(true);
		Scale_Slider->setDisabled(true);
		Scale_SpinBox->setDisabled(true);

		Clamp_Button->setDisabled(true);
		ClampRangeMin_SpinBox->setDisabled(true);
		ClampRangeSlider->setDisabled(true);
		ClampRangeMax_SpinBox->setDisabled(true);
	}

	void HaveDataEnableWidgets() {
		getOriginInfo_processButton->setEnabled(true);
	}
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
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
};

class QVolumeWWWL_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeWWWL_Widget(QWidget * parent = Q_NULLPTR) {
		setTitle("Data Display Range");
		setMinimumWidth(200);

		centerLayout = new QVBoxLayout;

		WWWL_Layout = new QGridLayout;
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

		DataRange_Layout = new QGridLayout;
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

		setLayout(centerLayout);
		centerLayout->addLayout(WWWL_Layout);
		centerLayout->addLayout(DataRange_Layout);

		NoDataDisableWidgets();
	}
	~QVolumeWWWL_Widget() {
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
	}

	QVBoxLayout *centerLayout;

	QGridLayout *WWWL_Layout;

	void setDataRange(double ww, double wl, double min, double max) {
		WW_Slider->setRange(min, max);
		WW_SpinBox->setRange(min, max);
		WW_Slider->setValue(max);
		WL_Slider->setRange(min, max);
		WL_SpinBox->setRange(min, max);
		WL_Slider->setValue((0.5 * (max - min)));
	}

	float WindowWidth, WindowLevel;
	float originDataMin, originDataMax;
	float dataMin, dataMax;

	QPushButton *WW_Button;
	QDoubleSlider * WW_Slider;
	QDoubleSpinBox * WW_SpinBox;
	QPushButton *WL_Button;
	QDoubleSlider * WL_Slider;
	QDoubleSpinBox * WL_SpinBox;

	QGridLayout *DataRange_Layout;
	QPushButton *dataRange_Button;
	QDoubleSliderDoubleRange *dataRangeSlider;
	QDoubleSpinBox * dataRangeMin_SpinBox;
	QDoubleSpinBox * dataRangeMax_SpinBox;

	void NoDataDisableWidgets() {
		NoStatisticsDisableWidgets();
	}
	void NoStatisticsDisableWidgets() {
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
	}
	void HaveDataEnableWidgets() {
		;
	}
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
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
	}

signals:
	void dataRangeChanged();
private slots:
	void setNewRange_WW(double ww) {
		WindowWidth = ww;

		if ((WindowLevel - 0.5 * WindowWidth) < originDataMin) {
			dataMin = originDataMin;
		}
		if ((WindowLevel + 0.5 * WindowWidth) > originDataMax) {
			dataMax = originDataMax;
		}
		dataRangeSlider->SetLowerValue(dataMin);
		dataRangeSlider->SetUpperValue(dataMax);
		emit dataRangeChanged();
	}
	void setNewRange_WL(double wl) {
		WindowLevel = wl;

		if ((WindowLevel - 0.5 * WindowWidth) < originDataMin) {
			dataMin = originDataMin;
		}
		if ((WindowLevel + 0.5 * WindowWidth) > originDataMax) {
			dataMax = originDataMax;
		}
		dataRangeSlider->SetLowerValue(dataMin);
		dataRangeSlider->SetUpperValue(dataMax);

		emit dataRangeChanged();
	}
	void setNewRange_DataMin(double datamin) {
		dataMin = datamin;
		WindowWidth = dataMax - dataMin;
		WW_Slider->setValue(dataMax - dataMin);
		WL_Slider->setValue((dataMax + dataMin) * 0.5);

		emit dataRangeChanged();
	}
	void setNewRange_DataMax(double datamax) {
		dataMax = datamax;
		WindowWidth = dataMax - dataMin;
		WW_Slider->setValue(dataMax - dataMin);
		WL_Slider->setValue((dataMax + dataMin) * 0.5);

		emit dataRangeChanged();
	}
};


class DisplayDockWidget : public QDockWidget {
	Q_OBJECT

public:

	DisplayDockWidget(QWidget * parent = Q_NULLPTR);
	~DisplayDockWidget();

public:
	void getPredefinedInfo();

private:
	QVBoxLayout *centerLayout;

	QFrame *dockCentralWidget;

	FileDirSet_Widget *m_FileDirSet_Widget;
	QSetFormat_Widget * m_QSetFormat_Widget;

	QVolumeReadWrite_Widget * m_QVolumeReadWrite_Widget;
	QVolumeStatistics_Widget * m_QVolumeStatistics_Widget;
	QVolumeWWWL_Widget * m_QVolumeWWWL_Widget;

	void setpuWidgets();

	void displayVolumeInfo();
	void displayVolumeStatistics();

	void clearVolumeInfo();
	void clearVolumeStatistics();

private slots:
	void readDcmsData();
	void readMhdData();
	void readFeimosData();
	void readPNGsData();
	void readJPGsData();

	void writeMhdData();
	void writeFeimosData();

	void getStatistics();

private:
	VolumeData volumeData;

	DataReaderAndWriter m_DataRW;
	GenerateFormat generateFormat;

	// process setting
	QString InputFolder;
	QString OutputFolder;
	QString OutputFileName;
	QString InputFilePath;
	

private:
	void NoDataDisableWidgets() {
		m_QVolumeReadWrite_Widget->NoDataDisableWidgets();
		m_QVolumeStatistics_Widget->NoDataDisableWidgets();
		m_QVolumeWWWL_Widget->NoDataDisableWidgets();

		m_QVolumeReadWrite_Widget->NoStatisticsDisableWidgets();
		m_QVolumeStatistics_Widget->NoStatisticsDisableWidgets();
		m_QVolumeWWWL_Widget->NoStatisticsDisableWidgets();
	}
	void NoStatisticsDisableWidgets() {
		m_QVolumeReadWrite_Widget->NoStatisticsDisableWidgets();
		m_QVolumeStatistics_Widget->NoStatisticsDisableWidgets();
		m_QVolumeWWWL_Widget->NoStatisticsDisableWidgets();
	}

	void HaveDataEnableWidgets() {
		m_QVolumeReadWrite_Widget->HaveDataEnableWidgets();
		m_QVolumeStatistics_Widget->HaveDataEnableWidgets();
		m_QVolumeWWWL_Widget->HaveDataEnableWidgets();
	}
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
		m_QVolumeReadWrite_Widget->HaveStatisticsDataEnableWidgets(volumeData);
		m_QVolumeStatistics_Widget->HaveStatisticsDataEnableWidgets(volumeData);
		m_QVolumeWWWL_Widget->HaveStatisticsDataEnableWidgets(volumeData);
	}
};


#endif



