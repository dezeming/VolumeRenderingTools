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


#ifndef __WidgetsCommon_h__
#define __WidgetsCommon_h__

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
#include <QFile>

#include "Utility/DicomCommon.hpp"

class QSetFormat_Widget : public QGroupBox {
	Q_OBJECT
public:
	QSetFormat_Widget(QWidget * parent = Q_NULLPTR) {
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
	~QSetFormat_Widget() { }

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

class FileDirSet_Widget : public QGroupBox {
	Q_OBJECT
public:
	FileDirSet_Widget(QWidget * parent = Q_NULLPTR) {
		setTitle("Data File Path");
		setMinimumWidth(200);

		mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		setupWidgets();
	}
	~FileDirSet_Widget();

	void setupWidgets();

	void updataDirFromPresetFile(QString filename);

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

	QVBoxLayout * mainLayout;

	QGridLayout * FolderLayout;
	QPushButton * InputFolderButton;
	QLineEdit * InputFolderEdit;
	QPushButton * InputFilePathButton;
	QLineEdit * InputFilePathEdit;
	QPushButton * OutputFolderButton;
	QLineEdit * OutputFolderEdit;
	QPushButton * OutputFileNameButton;
	QLineEdit * OutputFileNameEdit;

	QGridLayout * openLayout;
	QPushButton *OpenInputDirButton, *OpenOutputDirButton,
		*OpenInputFileDirButton, *OpenIconsDirButton;

private slots:
	void OpenInputDir();
	void OpenInputFileDir();
	void OpenOutputDir();
	void OpenIconsDir();


};

class QParseDcmLibSet_Widget : public QGroupBox {
	Q_OBJECT
public:
	QParseDcmLibSet_Widget(QWidget * parent = Q_NULLPTR) {
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
	~QParseDcmLibSet_Widget() { }

	DcmParseLib getParseSet() {
		if (ParseDcmLib_Radio_GDCM->isChecked()) return Dez_GDCM;
		if (ParseDcmLib_Radio_DCMTK->isChecked()) return Dez_DCMTK;
		return Dez_GDCM;
	}

	QGridLayout * ParseDcmLibLayout;
	QRadioButton *ParseDcmLib_Radio_DCMTK, *ParseDcmLib_Radio_GDCM;
	QButtonGroup* ParseDcmLib_ButtonGroup;
	QLabel *ParseDcmLib_Label_DCMTK, *ParseDcmLib_Label_GDCM;
	QLabel *Label_Null1, *Label_Null2, *Label_Null3, *Label_Null4;
};



#endif





