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

#include "InteractionDockWidget.h"

InteractionDockWidget::InteractionDockWidget(QWidget * parent) {
	setWindowTitle("Interaction");
	
	setFeatures(QDockWidget::DockWidgetMovable);
	setFeatures(QDockWidget::AllDockWidgetFeatures);
	
	dockCentralWidget = new QFrame;

	setWidget(dockCentralWidget);
	centerLayout = new QVBoxLayout;
	dockCentralWidget->setWindowFlags(Qt::FramelessWindowHint);
	dockCentralWidget->setLayout(centerLayout);
	
	setupInputOutput();

	setupFrames();

	setupDock();



	setMinimumWidth(200);
}

InteractionDockWidget::~InteractionDockWidget() {

}

void InteractionDockWidget::closeEvent(QCloseEvent * event) {

}

void InteractionDockWidget::setupDock() {

	renderButton = new QPushButton;
	renderButton->setText("Start Processing");

	centerLayout->addWidget(renderButton);
}

void InteractionDockWidget::setupInputOutput() {

	FolderLayout = new QGridLayout;
	InputFolderButton = new QPushButton;
	InputFolderEdit = new QLineEdit;
	InputFilePathButton = new QPushButton;
	InputFilePathEdit = new QLineEdit;
	OutputFolderButton = new QPushButton;
	OutputFolderEdit = new QLineEdit;
	OutputFileNameButton = new QPushButton;
	OutputFileNameEdit = new QLineEdit;

	FolderLayout->addWidget(InputFolderButton, 0, 0);
	InputFolderButton->setText("Input Folder");
	FolderLayout->addWidget(InputFolderEdit, 0, 1);
	InputFolderEdit->setText("E:/Datasets/DcmToolsTest/");

	FolderLayout->addWidget(InputFilePathButton, 1, 0);
	InputFilePathButton->setText("Input File Path");
	FolderLayout->addWidget(InputFilePathEdit, 1, 1);
	InputFilePathEdit->setText("E:/Datasets/DcmToolsTest/Sample.mhd");

	FolderLayout->addWidget(OutputFolderButton, 2, 0);
	OutputFolderButton->setText("Output Folder");
	FolderLayout->addWidget(OutputFolderEdit, 2, 1);
	OutputFolderEdit->setText("E:/Datasets/DcmToolsTest/Output/");

	FolderLayout->addWidget(OutputFileNameButton, 3, 0);
	OutputFileNameButton->setText("Output Folder");
	FolderLayout->addWidget(OutputFileNameEdit, 3, 1);
	OutputFileNameEdit->setText("Sample");

	centerLayout->addLayout(FolderLayout);
}

void InteractionDockWidget::setupFrames() {

	framesLayout = new QGridLayout;

	DcmToMhd_Frame_DCMTK_Frame = new QDcmToMhd_DCMTK_Frame;
	framesLayout->addWidget(DcmToMhd_Frame_DCMTK_Frame, 0, 0);

	DcmToMhd_Frame_GDCM_Frame = new QDcmToMhd_GDCM_Frame;
	framesLayout->addWidget(DcmToMhd_Frame_GDCM_Frame, 0, 1);

	MhdToFeimos_Frame = new QMhdToFeimos_Frame;
	framesLayout->addWidget(MhdToFeimos_Frame, 1, 0);

	MhdToPbrt_Frame = new QMhdToPbrt_Frame;
	framesLayout->addWidget(MhdToPbrt_Frame, 1, 1);



	MhdRotateAxis_Frame = new QMhdRotateAxis_Frame;
	framesLayout->addWidget(MhdRotateAxis_Frame, 2, 0);

	MhdFlipAxis_Frame = new QMhdFlipAxis_Frame;
	framesLayout->addWidget(MhdFlipAxis_Frame, 2, 1);

	MhdClip_Frame = new QMhdClip_Frame;
	framesLayout->addWidget(MhdClip_Frame, 3, 0);

	MhdResize_Frame = new QMhdResize_Frame;
	framesLayout->addWidget(MhdResize_Frame, 3, 1);




	centerLayout->addLayout(framesLayout);
}






