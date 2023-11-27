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

#include "DisplayDockWidget.hpp"

#include <QFile>
#include <fstream>
#include <string>

DisplayDockWidget::DisplayDockWidget(QWidget * parent) {
	setWindowTitle("Display Interaction");

	setFeatures(QDockWidget::DockWidgetMovable);

	dockCentralWidget = new QFrame;

	setWidget(dockCentralWidget);
	centerLayout = new QVBoxLayout;
	dockCentralWidget->setWindowFlags(Qt::FramelessWindowHint);
	dockCentralWidget->setLayout(centerLayout);

	setpuWidgets();

	setMinimumWidth(200);
}

DisplayDockWidget::~DisplayDockWidget() {

}

void DisplayDockWidget::setpuWidgets()
{
	m_FileDirSet_Widget = new FileDirSet_Widget;
	centerLayout->addWidget(m_FileDirSet_Widget);

}

#include <QFileInfo>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>

void DisplayDockWidget::getPredefinedInfo() {
	// Obtain predefined information

	InputFilePath = m_FileDirSet_Widget->getInputFilePath();
	//DebugTextPrintString("inputfilePath: " + inputfilePath);
	InputFolder = m_FileDirSet_Widget->getInputFolder();
	//DebugTextPrintString("inputDir: " + inputDir);
	OutputFolder = m_FileDirSet_Widget->getOutputFolder();
	//DebugTextPrintString("outputDir: " + outputDir);
	OutputFileName = m_FileDirSet_Widget->getOutputFileName();
	//DebugTextPrintString("outputFileName: " + outputFileName);

}







