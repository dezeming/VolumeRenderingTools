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
		readDataLayout->addWidget(writeMhd_processButton, 1, 0);
		readDataLayout->addWidget(writeFeimos_processButton, 1, 1);

		setLayout(centerLayout);
		centerLayout->addLayout(readDataLayout);
	}
	~QVolumeReadWrite_Widget() { }

	QVBoxLayout *centerLayout;

	QGridLayout *readDataLayout;
	QPushButton *readDcms_processButton;
	QPushButton *readMhd_processButton;
	QPushButton *readFeimos_processButton;

	QPushButton *writeMhd_processButton;
	QPushButton *writeFeimos_processButton;
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
		getGragientInfo_processButton = new QPushButton;
		getGragientInfo_processButton->setText("Generate Gradient Info");
		
		GetStatisticsLayout->addWidget(getOriginInfo_processButton, 0, 0);
		GetStatisticsLayout->addWidget(getGragientInfo_processButton, 0, 1);

		ScaleTheDataValue_Layout = new QGridLayout;
		Scale_processButton = new QPushButton;
		Scale_Slider = new QDoubleSlider;
		Scale_SpinBox = new QDoubleSpinBox;
		ScaleTheDataValue_Layout->addWidget(Scale_processButton, 0, 0);
		Scale_processButton->setText("Scale the value");
		ScaleTheDataValue_Layout->addWidget(Scale_Slider, 0, 1);
		ScaleTheDataValue_Layout->addWidget(Scale_SpinBox, 0, 2);

		setLayout(centerLayout);
		centerLayout->addLayout(GetStatisticsLayout);
		centerLayout->addLayout(ScaleTheDataValue_Layout);
	}
	~QVolumeStatistics_Widget() { }

	QVBoxLayout *centerLayout;

	QGridLayout *GetStatisticsLayout;
	QPushButton *getOriginInfo_processButton;
	QPushButton *getGragientInfo_processButton;

	QGridLayout *ScaleTheDataValue_Layout;
	QPushButton *Scale_processButton;
	QDoubleSlider * Scale_Slider;
	QDoubleSpinBox * Scale_SpinBox;

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

		WW_Slider->setRange(100.0f, 3000.0f);
		WW_SpinBox->setRange(100.0f, 3000.0f);
		WW_Slider->setValue(1000.0f);
		WL_Slider->setRange(-1000.0f, 1000.0f);
		WL_SpinBox->setRange(-1000.0f, 1000.0f);
		WL_Slider->setValue(0.0f);

		DataRange_Layout = new QGridLayout;
		dataRange_Button = new QPushButton;
		dataRangeSlider = new QDoubleSliderDoubleRange(Qt::Horizontal, QDoubleSliderDoubleRange::Option::DoubleHandles, nullptr);
		dataRange_Button->setText("Data Range");
		dataRangeSlider->SetRange(-1000.0,3000.0);
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
		dataRangeMin_SpinBox->setRange(-1000.0, 3000.0);
		dataRangeMax_SpinBox->setRange(-1000.0, 3000.0);
		connect(dataRangeMin_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setLowerValue(double)));
		connect(dataRangeMax_SpinBox, SIGNAL(valueChanged(double)), dataRangeSlider, SLOT(setUpperValue(double)));
		connect(dataRangeSlider, SIGNAL(lowerValueChanged(double)), dataRangeMin_SpinBox, SLOT(setValue(double)));
		connect(dataRangeSlider, SIGNAL(upperValueChanged(double)), dataRangeMax_SpinBox, SLOT(setValue(double)));

		setLayout(centerLayout);
		centerLayout->addLayout(WWWL_Layout);
		centerLayout->addLayout(DataRange_Layout);
	}
	~QVolumeWWWL_Widget() {
		disconnect(WW_Slider, SIGNAL(valueChanged(double)), WW_SpinBox, SLOT(setValue(double)));
		disconnect(WW_SpinBox, SIGNAL(valueChanged(double)), WW_Slider, SLOT(setValue(double)));
		disconnect(WL_Slider, SIGNAL(valueChanged(double)), WL_SpinBox, SLOT(setValue(double)));
		disconnect(WL_SpinBox, SIGNAL(valueChanged(double)), WL_Slider, SLOT(setValue(double)));
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

	int WindowWidth, WindowLevel;
	int dataMin, dataMax;

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
	QVolumeReadWrite_Widget * m_QVolumeReadWrite_Widget;
	QVolumeStatistics_Widget * m_QVolumeStatistics_Widget;
	QVolumeWWWL_Widget * m_QVolumeWWWL_Widget;

	void setpuWidgets();
	void displayVolumeInfo();
	void clearVolumeInfo();

private slots:
	void readDcmsData();
	void readMhdData();
	void readFeimosData();
	void writeMhdData();
	void writeFeimosData();
private:
	VolumeData volumeData;

	DataReaderAndWriter m_DataRW;

	// process setting
	QString InputFolder;
	QString OutputFolder;
	QString OutputFileName;
	QString InputFilePath;
	

private:

};


#endif



