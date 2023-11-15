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

#include "QSliderDoubleRange.hpp"
#include "DebugText.hpp"

#include "RenderThread.h"

class QVolumeConvert_Frame : public QGroupBox {
	Q_OBJECT
public:
	QVolumeConvert_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("DCM To MHD-RAW");
		setMinimumWidth(200);

		VolumeConvert_Layoput = new QVBoxLayout;
		setLayout(VolumeConvert_Layoput);

		DcmToMhd_processButton = new QPushButton;
		DcmToMhd_processButton->setText("Dcm To Mhd format");

		DcmToFeimos_processButton = new QPushButton;
		DcmToFeimos_processButton->setText("Dcm To Feimos format");

		MhdToFeimos_processButton = new QPushButton;
		MhdToFeimos_processButton->setText("Mhd To Feimos format");

		FeimosToMhd_processButton = new QPushButton;
		FeimosToMhd_processButton->setText("Feimos To Mhd format");

		VolumeConvert_Layoput->addWidget(DcmToMhd_processButton);
		VolumeConvert_Layoput->addWidget(DcmToFeimos_processButton);
		VolumeConvert_Layoput->addWidget(MhdToFeimos_processButton);
		VolumeConvert_Layoput->addWidget(FeimosToMhd_processButton);
	}
	~QVolumeConvert_Frame(){ }

	QPushButton *DcmToMhd_processButton;
	QPushButton *DcmToFeimos_processButton;
	QPushButton *MhdToFeimos_processButton;
	QPushButton *FeimosToMhd_processButton;

	QVBoxLayout *VolumeConvert_Layoput;
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


class QParseDcmLibFormat_Frame : public QGroupBox {
	Q_OBJECT
public:
	QParseDcmLibFormat_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("Parse Lib");
		setMinimumWidth(200);

		ParseDcmLibLayout = new QGridLayout;
		setLayout(ParseDcmLibLayout);

		ParseDcmLib_Label_GDCM = new QLabel;
		ParseDcmLib_Label_GDCM->setText("GDCM");
		ParseDcmLib_Label_DCMTK = new QLabel;
		ParseDcmLib_Label_DCMTK->setText("DCMTK");

		Label_Null1 = new QLabel;

		Label_Null2 = new QLabel;
		
		Label_Null3 = new QLabel;
		
		Label_Null4 = new QLabel;


		ParseDcmLib_Radio_GDCM = new QRadioButton;
		ParseDcmLib_Radio_DCMTK = new QRadioButton;
		ParseDcmLib_ButtonGroup = new QButtonGroup;
		ParseDcmLib_ButtonGroup->addButton(ParseDcmLib_Radio_GDCM);
		ParseDcmLib_ButtonGroup->addButton(ParseDcmLib_Radio_DCMTK);

		ParseDcmLibLayout->addWidget(ParseDcmLib_Label_GDCM, 0, 0);
		ParseDcmLibLayout->addWidget(ParseDcmLib_Radio_GDCM, 0, 1);
		ParseDcmLibLayout->addWidget(ParseDcmLib_Label_DCMTK, 0, 2);
		ParseDcmLibLayout->addWidget(ParseDcmLib_Radio_DCMTK, 0, 3);

		ParseDcmLib_Radio_GDCM->setChecked(true);

		ParseDcmLibLayout->addWidget(Label_Null1, 0, 4);
		ParseDcmLibLayout->addWidget(Label_Null2, 0, 5);
		ParseDcmLibLayout->addWidget(Label_Null3, 0, 6);
		ParseDcmLibLayout->addWidget(Label_Null4, 0, 7);
	}
	~QParseDcmLibFormat_Frame() { }

	DcmParseLib getParseSet() {
		if (ParseDcmLib_Radio_GDCM->isChecked()) return Dez_GDCM;
		if (ParseDcmLib_Radio_DCMTK->isChecked()) return Dez_DCMTK;
	}

	QGridLayout * ParseDcmLibLayout;
	QRadioButton *ParseDcmLib_Radio_DCMTK, *ParseDcmLib_Radio_GDCM;
	QButtonGroup* ParseDcmLib_ButtonGroup;
	QLabel *ParseDcmLib_Label_DCMTK, *ParseDcmLib_Label_GDCM;
	QLabel *Label_Null1, *Label_Null2, *Label_Null3, *Label_Null4;
};

class QGenerateFormat_Frame : public QGroupBox {
	Q_OBJECT
public:
	QGenerateFormat_Frame(QWidget * parent = Q_NULLPTR) {
		setTitle("Generate Format");
		setMinimumWidth(200);

		GenerateFormatLayout = new QGridLayout;
		setLayout(GenerateFormatLayout);

		Label_Origin = new QLabel;
		Label_UnsignedInt = new QLabel;
		Label_SignedInt = new QLabel; 
		Label_UnsignedShort = new QLabel;
		Label_SignedShort = new QLabel;
		Label_UnsignedChar = new QLabel;
		Label_SignedChar = new QLabel;
		Label_Float = new QLabel;
		Label_Double = new QLabel;

		Label_Origin->setText("Origin");
		Label_UnsignedInt->setText("Unsigned Int");
		Label_SignedInt->setText("Signed Int");
		Label_UnsignedShort->setText("Unsigned Short");
		Label_SignedShort->setText("Signed Short");
		Label_UnsignedChar->setText("Unsigned Char");
		Label_SignedChar->setText("Signed Char");
		Label_Float->setText("Float");
		Label_Double->setText("Double");

		RadioButton_Origin = new QRadioButton;
		RadioButton_UnsignedInt = new QRadioButton;
		RadioButton_SignedInt = new QRadioButton;
		RadioButton_UnsignedShort = new QRadioButton;
		RadioButton_SignedShort = new QRadioButton;
		RadioButton_UnsignedChar = new QRadioButton;
		RadioButton_SignedChar = new QRadioButton;
		RadioButton_Float = new QRadioButton;
		RadioButton_Double = new QRadioButton;

		ButtonGroup = new QButtonGroup;
		ButtonGroup->addButton(RadioButton_Origin);
		ButtonGroup->addButton(RadioButton_UnsignedInt);
		ButtonGroup->addButton(RadioButton_SignedInt);
		ButtonGroup->addButton(RadioButton_UnsignedShort);
		ButtonGroup->addButton(RadioButton_SignedShort);
		ButtonGroup->addButton(RadioButton_UnsignedChar);
		ButtonGroup->addButton(RadioButton_SignedChar);
		ButtonGroup->addButton(RadioButton_Float);
		ButtonGroup->addButton(RadioButton_Double);

		RadioButton_Origin->setChecked(true);

		GenerateFormatLayout->addWidget(Label_UnsignedInt, 0, 0);
		GenerateFormatLayout->addWidget(Label_SignedInt, 0, 2);
		GenerateFormatLayout->addWidget(Label_UnsignedShort, 0, 4);
		GenerateFormatLayout->addWidget(Label_SignedShort, 0, 6);

		GenerateFormatLayout->addWidget(RadioButton_UnsignedInt, 0, 1);
		GenerateFormatLayout->addWidget(RadioButton_SignedInt, 0, 3);
		GenerateFormatLayout->addWidget(RadioButton_UnsignedShort, 0, 5);
		GenerateFormatLayout->addWidget(RadioButton_SignedShort, 0, 7);

		GenerateFormatLayout->addWidget(Label_UnsignedChar, 1, 0);
		GenerateFormatLayout->addWidget(Label_SignedChar, 1, 2);
		GenerateFormatLayout->addWidget(Label_Float, 1, 4);
		GenerateFormatLayout->addWidget(Label_Double, 1, 6);

		GenerateFormatLayout->addWidget(RadioButton_UnsignedChar, 1, 1);
		GenerateFormatLayout->addWidget(RadioButton_SignedChar, 1, 3);
		GenerateFormatLayout->addWidget(RadioButton_Float, 1, 5);
		GenerateFormatLayout->addWidget(RadioButton_Double, 1, 7);

		GenerateFormatLayout->addWidget(Label_Origin, 3, 0);
		GenerateFormatLayout->addWidget(RadioButton_Origin, 3, 1);

	}
	~QGenerateFormat_Frame() { }

	DataFormat getDataFormatSet() {
		if (RadioButton_Origin->isChecked()) return Dez_Origin;
		if (RadioButton_UnsignedInt->isChecked()) return Dez_UnsignedLong;
		if (RadioButton_SignedInt->isChecked()) return Dez_SignedLong;
		if (RadioButton_UnsignedShort->isChecked()) return Dez_UnsignedShort;
		if (RadioButton_SignedShort->isChecked()) return Dez_SignedShort;
		if (RadioButton_UnsignedChar->isChecked()) return Dez_UnsignedChar;
		if (RadioButton_SignedChar->isChecked()) return Dez_SignedChar;
		if (RadioButton_Float->isChecked()) return Dez_Float;
		if (RadioButton_Double->isChecked()) return Dez_Double;
	}

	QGridLayout * GenerateFormatLayout;

	QButtonGroup* ButtonGroup;

	QRadioButton *RadioButton_Origin, *RadioButton_UnsignedInt, *RadioButton_SignedInt, *RadioButton_UnsignedShort;
	QRadioButton *RadioButton_SignedShort, *RadioButton_UnsignedChar, *RadioButton_SignedChar, *RadioButton_Float;
	QRadioButton *RadioButton_Double;

	QLabel *Label_Origin, *Label_UnsignedInt, *Label_SignedInt, *Label_UnsignedShort;
	QLabel *Label_SignedShort, *Label_UnsignedChar, *Label_SignedChar, *Label_Float;
	QLabel *Label_Double;
};


class InteractionDockWidget : public QDockWidget {
	Q_OBJECT

public:
	InteractionDockWidget(QWidget * parent = Q_NULLPTR);
	~InteractionDockWidget();

	QVolumeConvert_Frame *VolumeConvert_Frame;

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
	DataFormat getGenerateDataFormat() {
		return m_QGenerateFormat_Frame->getDataFormatSet();
	}
	DcmParseLib getDcmParseLib() {
		return m_QParseDcmLibFormat_Frame->getParseSet();
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
	
	QParseDcmLibFormat_Frame * m_QParseDcmLibFormat_Frame;
	QGenerateFormat_Frame * m_QGenerateFormat_Frame;

	void setupInputOutput();

	QGridLayout * framesLayout;

	void setupFrames();

public:
	QPushButton *processButton;

protected:
	void closeEvent(QCloseEvent *event);
	
private slots :
	
	
};




	






	
#endif







