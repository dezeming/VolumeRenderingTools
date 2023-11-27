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

#include "Utility/WidgetsCommon.hpp"
#include "Process/DataReaderAndWriter.hpp"
#include "Utility/DicomCommon.hpp"

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
	void setpuWidgets();

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



