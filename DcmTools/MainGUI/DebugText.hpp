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
	static DebugText* getDebugText();

private:
	QTextEdit *ShowDebugArea;
	QHBoxLayout *qlayout;

	DebugText(QWidget * parent = Q_NULLPTR);
	

};

#define  TextDinodonS(text)  DebugText::getDebugText()->addContents(text)

#define  TextDinodonN(text, num)  DebugText::getDebugText()->addContents(QString(text) + " " + QString::number(num))
#define  TextDinodonN2(text, num1, num2)  DebugText::getDebugText()->addContents(QString(text) + " " + QString::number(num1) + " " + QString::number(num2))
#define  TextDinodonN3(text, num1, num2, num3)  DebugText::getDebugText()->addContents(QString(text) + " " + QString::number(num1) + " " + QString::number(num2) + " " + QString::number(num3))
#define  TextDinodonN4(text, num1, num2, num3, num4)  DebugText::getDebugText()->addContents(QString(text) + " " + QString::number(num1) + " " + QString::number(num2) + " " + QString::number(num3) + " " + QString::number(num4))




#endif


