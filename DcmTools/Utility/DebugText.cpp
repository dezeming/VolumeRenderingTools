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

#include "DebugText.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>

static DebugText *dt = NULL;


DebugText::DebugText(QWidget * parent) : QWidget(parent) {
	setMinimumSize(800,800);
	
	qlayout = new QHBoxLayout(this);
	ShowDebugArea = new QTextEdit(this);
	ShowDebugArea->setFontPointSize(14);
	qlayout->setAlignment(Qt::AlignCenter);
	qlayout->addWidget(ShowDebugArea);
	show();
}

DebugText::~DebugText() {
}

static QMutex mutexInDebugText;
void DebugText::addContents(const QString& s1)
{
	//QMutexLocker locker(&mutexInDebugText);
	mutexInDebugText.lock();
	QTextCursor cursor = ShowDebugArea->textCursor();
	cursor.movePosition(QTextCursor::End);
	ShowDebugArea->setTextCursor(cursor);
	ShowDebugArea->append(s1);
	show();
	mutexInDebugText.unlock();
}

void DebugText::addHtmlContents(const QString& s1)
{
	//QMutexLocker locker(&mutexInDebugText);
	mutexInDebugText.lock();
	QTextCursor cursor = ShowDebugArea->textCursor();
	cursor.movePosition(QTextCursor::End);
	ShowDebugArea->setTextCursor(cursor);
	ShowDebugArea->insertHtml(s1);
	show();
	mutexInDebugText.unlock();
}

static QMutex mutexInStaticDebugText;
DebugText* DebugText::getDebugText() {
	//QMutexLocker locker(&mutexInStaticDebugText);
	mutexInStaticDebugText.lock();
	if (dt == NULL)
		dt = new DebugText;
	mutexInStaticDebugText.unlock();
	return dt;
}


