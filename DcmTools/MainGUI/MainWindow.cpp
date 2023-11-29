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

#include "MainWindow.hpp"
#include "Utility/DebugText.hpp"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
	QPalette pal;
	pal.setColor(QPalette::Background, QColor(49, 54, 59));
	setAutoFillBackground(true);
	setPalette(pal);
	pal.setColor(QPalette::Background, Qt::blue);

	setStyleSheet("background-color: rgb(49,54,59);\
		color:rgb(180,180,180);");

	setMinimumSize(800, 800);

	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	MainWindowLayout = new QVBoxLayout;
	MainWindowLayout->setAlignment(Qt::AlignCenter);

	centralWidget->setLayout(MainWindowLayout);

	setMenu();

	setWidgetAndDock();

}

MainWindow::~MainWindow() {
	
}

void MainWindow::closeEvent(QCloseEvent *event) {
	m_DisplayWidget->killRenderThread();

}

void MainWindow::setMenu(void) {


}

void showMemoryInfo(void);
void MainWindow::setWidgetAndDock(void) {
	
	m_DisplayWidget = new DisplayWidget;
	MainWindowLayout->addWidget(m_DisplayWidget);

	m_ProcessDockWidget = new ProcessDockWidget;
	addDockWidget(Qt::LeftDockWidgetArea, m_ProcessDockWidget);

	m_DataPresentDockWidget = new DataPresentDockWidget;
	addDockWidget(Qt::RightDockWidgetArea, m_DataPresentDockWidget);

	m_DisplayDockWidget = new DisplayDockWidget;
	addDockWidget(Qt::LeftDockWidgetArea, m_DisplayDockWidget);

	tabifyDockWidget(m_ProcessDockWidget, m_DisplayDockWidget);
	m_ProcessDockWidget->raise();

	showMemoryInfo();
}

void MainWindow::setRendering() {
	if (!m_DisplayWidget->renderFlag) {
		// start rendering
		m_DisplayWidget->startRenderThread();
	}
}











