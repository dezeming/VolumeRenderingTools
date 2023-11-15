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

#include "IMAGraphicsView.h"
#include "DebugText.hpp"


IMAGraphicsView::IMAGraphicsView(QGraphicsView * parent) {
	setFrameShadow(Sunken);
	setFrameShape(NoFrame);
	//Set the minimum display area for View
	setMinimumHeight(400);
	setMinimumWidth(300);
	//Set rendering area anti aliasing
	setRenderHint(QPainter::Antialiasing);

	getMap("Icons/background.png");
	
	
	setScene(&scene);
	//scene.setBackgroundBrush(QColor(0, 255, 255, 255));

	setCacheMode(CacheBackground);
	scale(_scale, _scale);
}

IMAGraphicsView::~IMAGraphicsView() {
	


}


void IMAGraphicsView::mouseMoveEvent(QMouseEvent *event) {

}

void IMAGraphicsView::mousePressEvent(QMouseEvent *event) {
	//QGraphicsView Pos
	QPoint viewPoint = event->pos();
	//QGraphicsScene Pos
	QPointF scenePoint = mapToScene(viewPoint);
	//DebugText::getDebugText()->addContents(QString::number(scenePoint.x())+" "+ QString::number(scenePoint.y()));
}

void IMAGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
	//QGraphicsView Pos
	QPoint viewPoint = event->pos();
	//QGraphicsScene Pos
	QPointF scenePoint = mapToScene(viewPoint);
	//DebugText::getDebugText()->addContents(QString::number(scenePoint.x()) + " " + QString::number(scenePoint.y()));
}

void IMAGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {
	painter->drawPixmap(int(sceneRect().left()), int(sceneRect().top()), map);
	
}

void IMAGraphicsView::getMap(QString mapname) {
	map.load(mapname);

	width = map.width();
	height = map.height();
	bottom = -0.5 * height;
	left = -0.5 * width;

	scene.setSceneRect(left, bottom, width, height);
}

void IMAGraphicsView::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0) {
		_scale = 1.1f;
	}
	else {
		_scale = 0.9f; 
	}
	scale(_scale, _scale); 
}

void IMAGraphicsView::PaintBuffer(DcmTools::Uchar* buffer, int width, int height, int channals) {
	QImage::Format format;
	if (channals == 4) format = QImage::Format_ARGB32;
	else if (channals == 3) format = QImage::Format_RGB888;
	QImage image(buffer, width, height, static_cast<int>(width * channals * sizeof(unsigned char)), format);
	image.constBits();
	map = QPixmap::fromImage(image.rgbSwapped());

	width = map.width();
	height = map.height();
	bottom = -0.5 * height;
	left = -0.5 * width;
	scene.setSceneRect(left, bottom, width, height);

	setScene(&scene);
	scale(0.5, 0.5);
	scale(2.0, 2.0);
}

