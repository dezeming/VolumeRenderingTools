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

		setLayout(centerLayout);
		centerLayout->addLayout(GetStatisticsLayout);
	}
	~QVolumeStatistics_Widget() { }

	QVBoxLayout *centerLayout;

	QGridLayout *GetStatisticsLayout;
	QPushButton *getOriginInfo_processButton;
	QPushButton *getGragientInfo_processButton;


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



