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
	QVolumeConvert_Frame(QWidget * parent = Q_NULLPTR);
	~QVolumeConvert_Frame();

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
	QVolumeDownSampling_Frame(QWidget * parent = Q_NULLPTR);
	~QVolumeDownSampling_Frame();

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

class QVolumeRotateAxis_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeRotateAxis_Frame(QWidget * parent = Q_NULLPTR);
	~QVolumeRotateAxis_Frame();

	bool getRotateAxis(int permute[3]);

	QLabel * permuteLabel;
	QLabel * permuteExampleLabel;
	QLineEdit * permuteEdit;
	QGridLayout * VolumeRotateAxis_Edit_Layout;

	QPushButton *MhdRotateAxis_processButton;
	QPushButton *FeimosRotateAxis_processButton;
	QGridLayout * process_Layout;

	QVBoxLayout *VolumeRotateAxis_Layoput;
};

class QVolumeFlipAxis_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeFlipAxis_Frame(QWidget * parent = Q_NULLPTR);
	~QVolumeFlipAxis_Frame();

	bool getFlipAxis(int flip[3]);

	QRadioButton *VolumeFlipAxis_Radio_X, *VolumeFlipAxis_Radio_Y, *VolumeFlipAxis_Radio_Z;
	QLabel *VolumeFlipAxis_Label_X, *VolumeFlipAxis_Label_Y, *VolumeFlipAxis_Label_Z;
	QGridLayout * VolumeFlipAxis_EditLayout;
	QButtonGroup* VolumeFlipAxis_ButtonGroup;

	QPushButton *FeimosFlipAxis_processButton;
	QPushButton *MhdFlipAxis_processButton;
	QGridLayout * process_Layout;
	
	QVBoxLayout *VolumeFlipAxis_Layoput;
};

class QVolumeClip_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeClip_Frame(QWidget * parent = Q_NULLPTR);
	~QVolumeClip_Frame();

	QLabel *VolumeClip_Label_X, *VolumeClip_Label_Y, *VolumeClip_Label_Z;
	QLineEdit * VolumeClip_Lower_X, *VolumeClip_Lower_Y, *VolumeClip_Lower_Z;
	QLineEdit * VolumeClip_Upper_X, *VolumeClip_Upper_Y, *VolumeClip_Upper_Z;
	QGridLayout * VolumeClip_EditLayout;

	QPushButton *MhdClip_processButton;
	QPushButton *FeimosClip_processButton;
	QGridLayout * process_Layout;
	
	QVBoxLayout *VolumeClip_Layoput;
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

	QVolumeRotateAxis_Frame * VolumeRotateAxis_Frame;
	QVolumeFlipAxis_Frame *VolumeFlipAxis_Frame;
	QVolumeClip_Frame *VolumeClip_Frame;

	QGridLayout * framesLayout;

	// data

	// display buffer
	FrameBuffer framebuffer;

	
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

	void process_PngsToFeimos();
	void process_PngsToMhd();
	void process_JpgsToFeimos();
	void process_JpgsToMhd();

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








