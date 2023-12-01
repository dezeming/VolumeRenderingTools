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

#ifndef __ImageToolsDockWidget_h__
#define __ImageToolsDockWidget_h__

#include <QDockWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>

#include <QGroupBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

#include <QRadioButton>
#include <QButtonGroup>
#include <QSlider>
#include <QSpinBox>

#include "Utility/QSliderDoubleRange.hpp"
#include "Utility/DebugText.hpp"
#include "Utility/WidgetsCommon.hpp"

#include "Display/RenderThread.hpp"
#include "Process/DataProcess.hpp"

// convert

class QVolumeConvert_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeConvert_Frame(QWidget * parent = Q_NULLPTR) {
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

		VolumeConvert_Layoput->addWidget(DcmToMhd_processButton, 0, 0);
		VolumeConvert_Layoput->addWidget(DcmToFeimos_processButton, 0, 1);
		VolumeConvert_Layoput->addWidget(MhdToFeimos_processButton, 1, 0);
		VolumeConvert_Layoput->addWidget(FeimosToMhd_processButton, 1, 1);
		VolumeConvert_Layoput->addWidget(PngToMhd_processButton, 2, 0);
		VolumeConvert_Layoput->addWidget(PngToFeimos_processButton, 2, 1);
		VolumeConvert_Layoput->addWidget(JpgToMhd_processButton, 3, 0);
		VolumeConvert_Layoput->addWidget(JpgToFeimos_processButton, 3, 1);
	}
	~QVolumeConvert_Frame(){ }

	QPushButton *DcmToMhd_processButton;
	QPushButton *DcmToFeimos_processButton;
	QPushButton *MhdToFeimos_processButton;
	QPushButton *FeimosToMhd_processButton;

	QPushButton * PngToMhd_processButton;
	QPushButton * PngToFeimos_processButton;
	QPushButton * JpgToMhd_processButton;
	QPushButton * JpgToFeimos_processButton;

	QGridLayout *VolumeConvert_Layoput;
};

// down sampling

class QVolumeDownSampling_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeDownSampling_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("DownSampling Volume");
		setMinimumWidth(200);

		DownSampling_Layoput = new QVBoxLayout;
		setLayout(DownSampling_Layoput);

		Interval_Label = new QLabel;
		Interval_Label->setText("Interval:");
		Interval_Slider = new QSlider(Qt::Horizontal);
		Interval_Slider->setRange(1,100);
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
	~QVolumeDownSampling_Frame() { 
		disconnect(Interval_Slider, SIGNAL(valueChanged(int)), Interval_SpinBox, SLOT(setValue(int)));
		disconnect(Interval_SpinBox, SIGNAL(valueChanged(int)), Interval_Slider, SLOT(setValue(int)));
	}

	int getIntervalValue() {
		return Interval_Slider->value();
	}


	QLabel * Interval_Label;
	QSlider * Interval_Slider;
	QSpinBox * Interval_SpinBox;
	QGridLayout * Interval_Layout;

	QPushButton *MhdDownSampling_processButton;
	QPushButton *FeimosDownSampling_processButton;
	QPushButton *LargeFeimosDownSampling_processButton;

	QVBoxLayout *DownSampling_Layoput;
};

// Process

class QMhdRotateAxis_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdRotateAxis_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("MHD Rotate Axis");
		setMinimumWidth(200);

		MhdRotateAxis_Layoput = new QVBoxLayout;
		setLayout(MhdRotateAxis_Layoput);

		permuteLabel = new QLabel;
		permuteLabel->setText("Rotate Axis(Example: 2,0,1)");
		permuteEdit = new QLineEdit;
		permuteEdit->setText("0,1,2");
		MhdRotateAxis_Edit_Layout = new QGridLayout;
		MhdRotateAxis_Edit_Layout->addWidget(permuteLabel, 0, 0);
		MhdRotateAxis_Edit_Layout->addWidget(permuteEdit, 0, 1);

		MhdRotateAxis_processButton = new QPushButton;
		MhdRotateAxis_processButton->setText("Process");

		MhdRotateAxis_Layoput->addLayout(MhdRotateAxis_Edit_Layout);
		MhdRotateAxis_Layoput->addWidget(MhdRotateAxis_processButton);
	}
	~QMhdRotateAxis_Frame() { }

	bool getRotateAxis(int permute[3]) {

		QString permu = permuteEdit->text();
		QStringList splitList = permu.split(",");
		if (splitList.size() != 3) {
			DebugTextPrintString("Incorrect input format.");
			return false;
		}

		bool isOk = true;
		for (int i = 0; i < splitList.size(); ++i) {
			bool ok;
			permute[i] = splitList[i].toInt(&ok);
			isOk = isOk && ok;
		}

		return isOk;
	}

	QLabel * permuteLabel;
	QLabel * permuteExampleLabel;
	QLineEdit * permuteEdit;
	QGridLayout * MhdRotateAxis_Edit_Layout;

	QPushButton *MhdRotateAxis_processButton;

	QVBoxLayout *MhdRotateAxis_Layoput;
};

class QMhdFlipAxis_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdFlipAxis_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("MHD Flip Axis");
		setMinimumWidth(200);

		MhdFlipAxis_Layoput = new QVBoxLayout;
		setLayout(MhdFlipAxis_Layoput);

		MhdFlipAxis_Label_X = new QLabel;
		MhdFlipAxis_Label_Y = new QLabel;
		MhdFlipAxis_Label_Z = new QLabel;
		MhdFlipAxis_Radio_X = new QRadioButton;
		MhdFlipAxis_Radio_Y = new QRadioButton;
		MhdFlipAxis_Radio_Z = new QRadioButton;
		MhdFlipAxis_ButtonGroup = new QButtonGroup(this);
		MhdFlipAxis_ButtonGroup->addButton(MhdFlipAxis_Radio_X);
		MhdFlipAxis_ButtonGroup->addButton(MhdFlipAxis_Radio_Y);
		MhdFlipAxis_ButtonGroup->addButton(MhdFlipAxis_Radio_Z);
		MhdFlipAxis_EditLayout = new QGridLayout;
		MhdFlipAxis_Label_X->setText("X:");
		MhdFlipAxis_Label_Y->setText("Y:");
		MhdFlipAxis_Label_Z->setText("Z:");
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Label_X, 0, 0);
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Label_Y, 0, 2);
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Label_Z, 0, 4);
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Radio_X, 0, 1);
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Radio_Y, 0, 3);
		MhdFlipAxis_EditLayout->addWidget(MhdFlipAxis_Radio_Z, 0, 5);

		MhdFlipAxis_processButton = new QPushButton;
		MhdFlipAxis_processButton->setText("Process");

		MhdFlipAxis_Layoput->addLayout(MhdFlipAxis_EditLayout);
		MhdFlipAxis_Layoput->addWidget(MhdFlipAxis_processButton);
	}
	~QMhdFlipAxis_Frame() { }

	bool getFlipAxis(int flip[3]) {

		if (MhdFlipAxis_Radio_X->isChecked()) flip[0] = 1;
		else  flip[0] = 0;
		if (MhdFlipAxis_Radio_Y->isChecked()) flip[1] = 1;
		else  flip[1] = 0;
		if (MhdFlipAxis_Radio_Z->isChecked()) flip[2] = 1;
		else  flip[2] = 0;

		return true;
	}

	QRadioButton *MhdFlipAxis_Radio_X, *MhdFlipAxis_Radio_Y, *MhdFlipAxis_Radio_Z;
	QLabel *MhdFlipAxis_Label_X, *MhdFlipAxis_Label_Y, *MhdFlipAxis_Label_Z;
	QGridLayout * MhdFlipAxis_EditLayout;
	QButtonGroup* MhdFlipAxis_ButtonGroup;

	QPushButton *MhdFlipAxis_processButton;
	
	QVBoxLayout *MhdFlipAxis_Layoput;
};

class QMhdClip_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdClip_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("MHD Clip");
		setMinimumWidth(200);

		MhdClip_Layoput = new QVBoxLayout;
		setLayout(MhdClip_Layoput);

		MhdClip_Label_X = new QLabel;
		MhdClip_Label_X->setText("X:");
		MhdClip_Lower_X = new QLineEdit;
		MhdClip_Lower_X->setText("0.0");
		MhdClip_Upper_X = new QLineEdit;
		MhdClip_Upper_X->setText("1.0");
		MhdClip_Label_Y = new QLabel;
		MhdClip_Label_Y->setText("Y:");
		MhdClip_Lower_Y = new QLineEdit;
		MhdClip_Lower_Y->setText("0.0");
		MhdClip_Upper_Y = new QLineEdit;
		MhdClip_Upper_Y->setText("1.0");
		MhdClip_Label_Z = new QLabel;
		MhdClip_Label_Z->setText("Z:");
		MhdClip_Lower_Z = new QLineEdit;
		MhdClip_Lower_Z->setText("0.0");
		MhdClip_Upper_Z = new QLineEdit;
		MhdClip_Upper_Z->setText("1.0");
		MhdClip_EditLayout = new QGridLayout;
		MhdClip_EditLayout->addWidget(MhdClip_Label_X, 0, 0);
		MhdClip_EditLayout->addWidget(MhdClip_Lower_X, 0, 1);
		MhdClip_EditLayout->addWidget(MhdClip_Upper_X, 0, 2);
		MhdClip_EditLayout->addWidget(MhdClip_Label_Y, 1, 0);
		MhdClip_EditLayout->addWidget(MhdClip_Lower_Y, 1, 1);
		MhdClip_EditLayout->addWidget(MhdClip_Upper_Y, 1, 2);
		MhdClip_EditLayout->addWidget(MhdClip_Label_Z, 2, 0);
		MhdClip_EditLayout->addWidget(MhdClip_Lower_Z, 2, 1);
		MhdClip_EditLayout->addWidget(MhdClip_Upper_Z, 2, 2);
		MhdClip_EditLayout->setColumnMinimumWidth(0, 30);
		MhdClip_EditLayout->setColumnMinimumWidth(1, 50);
		MhdClip_EditLayout->setColumnMinimumWidth(2, 50);
		MhdClip_EditLayout->setColumnStretch(0, 1);       
		MhdClip_EditLayout->setColumnStretch(1, 2);
		MhdClip_EditLayout->setColumnStretch(2, 2);

		MhdClip_processButton = new QPushButton;
		MhdClip_processButton->setText("Process");

		MhdClip_Layoput->addLayout(MhdClip_EditLayout);
		MhdClip_Layoput->addWidget(MhdClip_processButton);
	}
	~QMhdClip_Frame() { }

	QLabel *MhdClip_Label_X, *MhdClip_Label_Y, *MhdClip_Label_Z;
	QLineEdit * MhdClip_Lower_X, *MhdClip_Lower_Y, *MhdClip_Lower_Z;
	QLineEdit * MhdClip_Upper_X, *MhdClip_Upper_Y, *MhdClip_Upper_Z;
	QGridLayout * MhdClip_EditLayout;

	QPushButton *MhdClip_processButton;
	
	QVBoxLayout *MhdClip_Layoput;
};


class ProcessDockWidget : public QDockWidget {
	Q_OBJECT

public:
	ProcessDockWidget(QWidget * parent = Q_NULLPTR);
	~ProcessDockWidget();

public:
	void getPredefinedInfo();

	DataFormat getGenerateDataFormat() {
		return m_QSetFormat_Widget->getDataFormatSet();
	}
	DcmParseLib getDcmParseLib() {
		return m_QParseDcmLibSet_Widget->getParseSet();
	}

public:
	ProcessVolumeData processVolumeData;

	// process setting
	QString InputFolder;
	QString OutputFolder;
	QString OutputFileName;
	QString InputFilePath;

	GenerateFormat generateFormat;

	int permute[3];
	int flip[3];
	double clipCenter[3], clipBound[3];
	double clipLower[3], clipUpper[3];
	int interval;

private:
	void setupWidget();
	void setupFrames();
	void setupProcessFunc();

private:
	QVBoxLayout *centerLayout;
	QFrame *dockCentralWidget;
	QPushButton *processButton;

	FileDirSet_Widget * m_FileDirSet_Widget;
	QSetFormat_Widget * m_QSetFormat_Widget;
	QParseDcmLibSet_Widget * m_QParseDcmLibSet_Widget;
	
	QVolumeConvert_Frame *VolumeConvert_Frame;
	QVolumeDownSampling_Frame *VolumeDownSampling_Frame;

	QMhdRotateAxis_Frame * MhdRotateAxis_Frame;
	QMhdFlipAxis_Frame *MhdFlipAxis_Frame;
	QMhdClip_Frame *MhdClip_Frame;

	QGridLayout * framesLayout;
	
protected:
	void closeEvent(QCloseEvent *event);
	
private slots:

	// test
	void setProcess();

	// volume file type convert
	void process_DcmToMhd();
	void process_DcmToFeimos();
	void process_MhdToFeimos();
	void process_FeimosToMhd();

	// down sampling
	void process_MhdDownSampling();
	void process_FeimosDownSampling();
	void process_LargeFeimosDownSampling();

	// volume process
	void process_MhdRotateAxis();
	void process_MhdFlipAxis();
	void process_MhdClip();


};





	
#endif








