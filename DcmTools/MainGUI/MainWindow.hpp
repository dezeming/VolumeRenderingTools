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

#pragma once

#ifndef __MainWindows_h__
#define __MainWindows_h__

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QCloseEvent>

#include "Display/DisplayWidget.hpp"
#include "Process/ProcessDockWidget.hpp"
#include "InfoPresent/DataPresentDockWidget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

public:


private:
	QWidget* centralWidget;
	QVBoxLayout* MainWindowLayout;

	DisplayWidget* m_DisplayWidget;

	ProcessDockWidget* m_ProcessDockWidget;

	DataPresentDockWidget* m_DataPresentDockWidget;

private:
	void setMenu(void);
	void setWidgetAndDock(void);
	void closeEvent(QCloseEvent *event);

private slots:
	void setRendering();

	
};


#endif


