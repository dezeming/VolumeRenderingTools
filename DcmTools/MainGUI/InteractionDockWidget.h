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

	Github site: <https://github.com/dezeming/Crystal>
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

#include "QSliderDoubleRange.hpp"


class QDcmToMhd_DCMTK_Frame : public QGroupBox {
	Q_OBJECT
public:
	QDcmToMhd_DCMTK_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("DCM To MHD-RAW(DCMTK)");
		setMinimumWidth(200);

		DcmToMhd_DCMTK_Layoput = new QVBoxLayout;
		setLayout(DcmToMhd_DCMTK_Layoput);

		DcmToMhd_Frame_DCMTK_processButton = new QPushButton;
		DcmToMhd_Frame_DCMTK_processButton->setText("Process");

		DcmToMhd_DCMTK_Layoput->addWidget(DcmToMhd_Frame_DCMTK_processButton);
	}
	~QDcmToMhd_DCMTK_Frame(){ }

	QPushButton *DcmToMhd_Frame_DCMTK_processButton;
	QVBoxLayout *DcmToMhd_DCMTK_Layoput;
};

class QDcmToMhd_GDCM_Frame : public QGroupBox {
	Q_OBJECT
public:
	QDcmToMhd_GDCM_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("DCM To MHD-RAW(GDCM)");
		setMinimumWidth(200);

		DcmToMhd_GDCM_Layoput = new QVBoxLayout;
		setLayout(DcmToMhd_GDCM_Layoput);

		DcmToMhd_Frame_GDCM_processButton = new QPushButton;
		DcmToMhd_Frame_GDCM_processButton->setText("Process");

		DcmToMhd_GDCM_Layoput->addWidget(DcmToMhd_Frame_GDCM_processButton);
	}
	~QDcmToMhd_GDCM_Frame() { }

	QPushButton *DcmToMhd_Frame_GDCM_processButton;
	QVBoxLayout *DcmToMhd_GDCM_Layoput;
};

class QMhdToFeimos_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdToFeimos_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("MHD To Feimos Format");
		setMinimumWidth(200);

		MhdToFeimos_Layoput = new QVBoxLayout;
		setLayout(MhdToFeimos_Layoput);

		MhdToFeimos_processButton = new QPushButton;
		MhdToFeimos_processButton->setText("Process");

		MhdToFeimos_Layoput->addWidget(MhdToFeimos_processButton);
	}
	~QMhdToFeimos_Frame() { }

	QPushButton *MhdToFeimos_processButton;
	QVBoxLayout *MhdToFeimos_Layoput;
};

class QMhdToPbrt_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdToPbrt_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("MHD To Pbrt Format");
		setMinimumWidth(200);

		MhdToPbrt_Layoput = new QVBoxLayout;
		setLayout(MhdToPbrt_Layoput);

		MhdToPbrt_processButton = new QPushButton;
		MhdToPbrt_processButton->setText("Process");

		MhdToPbrt_Layoput->addWidget(MhdToPbrt_processButton);
	}
	~QMhdToPbrt_Frame() { }

	QPushButton *MhdToPbrt_processButton;
	QVBoxLayout *MhdToPbrt_Layoput;
};


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

class QMhdResize_Frame : public QGroupBox {
	Q_OBJECT
public:
	QMhdResize_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("Resize MHD");
		setMinimumWidth(200);

		MhdResize_Layoput = new QVBoxLayout;
		setLayout(MhdResize_Layoput);

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

		MhdResize_processButton = new QPushButton;
		MhdResize_processButton->setText("Process");

		MhdResize_Layoput->addLayout(Interval_Layout);
		MhdResize_Layoput->addWidget(MhdResize_processButton);
	}
	~QMhdResize_Frame() { }

	QLabel * Interval_Label;
	QSlider * Interval_Slider;
	QSpinBox * Interval_SpinBox;
	QGridLayout * Interval_Layout;

	QPushButton *MhdResize_processButton;

	QVBoxLayout *MhdResize_Layoput;
};



class InteractionDockWidget : public QDockWidget {
	Q_OBJECT

public:
	InteractionDockWidget(QWidget * parent = Q_NULLPTR);
	~InteractionDockWidget();

	QDcmToMhd_DCMTK_Frame *DcmToMhd_Frame_DCMTK_Frame;
	QDcmToMhd_GDCM_Frame *DcmToMhd_Frame_GDCM_Frame;
	QMhdToFeimos_Frame *MhdToFeimos_Frame;
	QMhdToPbrt_Frame *MhdToPbrt_Frame;

	QMhdRotateAxis_Frame * MhdRotateAxis_Frame;
	QMhdFlipAxis_Frame *MhdFlipAxis_Frame;
	QMhdClip_Frame *MhdClip_Frame;
	QMhdResize_Frame *MhdResize_Frame;

	QString getInputFolder() {
		return InputFolderEdit->text();
	}
	QString getInputFilePath() {
		return InputFilePathEdit->text();
	}
	QString getOutputFolder() {
		return OutputFolderEdit->text();
	}
	QString getOutputFileName() {
		return OutputFileNameEdit->text();
	}

private:
	void setupDock();

private:
	QVBoxLayout *centerLayout;
	QFrame *dockCentralWidget;

	QGridLayout * FolderLayout;
	QPushButton * InputFolderButton;
	QLineEdit * InputFolderEdit;
	QPushButton * InputFilePathButton;
	QLineEdit * InputFilePathEdit;
	QPushButton * OutputFolderButton;
	QLineEdit * OutputFolderEdit;
	QPushButton * OutputFileNameButton;
	QLineEdit * OutputFileNameEdit;

	void setupInputOutput();

	QGridLayout * framesLayout;

	void setupFrames();

public:
	QPushButton *renderButton;

protected:
	void closeEvent(QCloseEvent *event);
	
private slots :
	
	
};




	






	
#endif







