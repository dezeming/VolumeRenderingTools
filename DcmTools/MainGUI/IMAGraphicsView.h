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
#ifndef __IMAGraphicsView_h__
#define __IMAGraphicsView_h__

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QPixmap>
#include <QMouseEvent>

#include "Core\Fwd.h"

class IMAGraphicsView : public QGraphicsView {
	Q_OBJECT

public:
	IMAGraphicsView(QGraphicsView * parent = Q_NULLPTR);
	~IMAGraphicsView();
public:
	void getMap(QString mapname);

private:
	int bottom, left, top, right, width, height;
	float _scale = 1.0f;
private:
	QGraphicsScene scene;
	QPixmap map;

signals:
	

private slots:
	void PaintBuffer(DcmTools::Uchar* buffer, int width, int height, int channals);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void drawBackground(QPainter *painter, const QRectF &rect);
	void mouseReleaseEvent(QMouseEvent *event);
};





#endif






