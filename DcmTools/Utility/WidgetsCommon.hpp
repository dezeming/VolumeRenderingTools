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

class FileDirSet_Widget : public QGroupBox {
	Q_OBJECT
public:
	FileDirSet_Widget(QWidget * parent = Q_NULLPTR);
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

class QSetFormat_Widget : public QGroupBox {
	Q_OBJECT
public:
	QSetFormat_Widget(QWidget * parent = Q_NULLPTR);
	~QSetFormat_Widget();

	DataFormat getDataFormatSet();

	QGridLayout * GenerateFormatLayout;

	QButtonGroup* ButtonGroup;

	QRadioButton *RadioButton_Origin, *RadioButton_UnsignedInt, *RadioButton_SignedInt, *RadioButton_UnsignedShort;
	QRadioButton *RadioButton_SignedShort, *RadioButton_UnsignedChar, *RadioButton_SignedChar, *RadioButton_Float;
	QRadioButton *RadioButton_Double;

	QLabel *Label_Origin, *Label_UnsignedInt, *Label_SignedInt, *Label_UnsignedShort;
	QLabel *Label_SignedShort, *Label_UnsignedChar, *Label_SignedChar, *Label_Float;
	QLabel *Label_Double;
};

class QParseDcmLibSet_Widget : public QGroupBox {
	Q_OBJECT
public:
	QParseDcmLibSet_Widget(QWidget * parent = Q_NULLPTR);
	~QParseDcmLibSet_Widget();

	DcmParseLib getParseSet();

	QGridLayout * ParseDcmLibLayout;
	QRadioButton *ParseDcmLib_Radio_DCMTK, *ParseDcmLib_Radio_GDCM;
	QButtonGroup* ParseDcmLib_ButtonGroup;
	QLabel *ParseDcmLib_Label_DCMTK, *ParseDcmLib_Label_GDCM;
	QLabel *Label_Null1, *Label_Null2, *Label_Null3, *Label_Null4;
};



#endif





