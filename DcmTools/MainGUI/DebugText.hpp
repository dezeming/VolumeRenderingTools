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
#ifndef _DEBUGSTEXT_H__
#define _DEBUGSTEXT_H__

#include <QWidget>
#include <qtextedit.h>
#include <qlayout.h>
#include <qmutex.h>
#include <QString>

class DebugText : public QWidget {
	Q_OBJECT

public:
	~DebugText();
	void addContents(const QString& s1);
	void addHtmlContents(const QString& s1);
	static DebugText* getDebugText();

private:
	QTextEdit *ShowDebugArea;
	QHBoxLayout *qlayout;

	DebugText(QWidget * parent = Q_NULLPTR);
	

};

#define DebugTextPrintLineBreak() DebugText::getDebugText()->addContents("")
#define DebugTextPrintString(text)  DebugText::getDebugText()->addContents(QString("<font color='black' size='14'>Info:[") + text + QString("]</font>"))

#define DebugTextPrintNum(text, num)  DebugText::getDebugText()->addContents(QString("<font color='black' size='14'>") + text + ":[" + QString::number(num) + "]</font>")
#define DebugTextPrint2Nums(text, num1, num2)  DebugText::getDebugText()->addContents(QString("<font color='black' size='14'>") + text + ":[" + QString::number(num1) + "," + QString::number(num2) + "]</font>")
#define DebugTextPrint3Nums(text, num1, num2, num3)  DebugText::getDebugText()->addContents(QString("<font color='black' size='14'>") + text + ":[" + QString::number(num1) + "," + QString::number(num2) + "," + QString::number(num3) + "]</font>")
#define DebugTextPrint4Nums(text, num1, num2, num3, num4)  DebugText::getDebugText()->addContents(QString("<font color='black' size='14'>") + text + ":[" + QString::number(num1) + "," + QString::number(num2) + "," + QString::number(num3) + "," + QString::number(num4) + "]</font>")

#define DebugTextPrintErrorString(text) DebugText::getDebugText()->addHtmlContents(QString("<font color='red' size='14'>Error:[") + QString(text) + QString("]</font>"))
#define DebugTextPrintWarningString(text) DebugText::getDebugText()->addHtmlContents(QString("<font color='blue' size='14'>Warning:[") + QString(text) + QString("]</font>"))


#endif


