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
#ifndef __QDoubleSlider_H__
#define __QDoubleSlider_H__

#include <QSlider>

class QDoubleSlider : public QSlider {
	Q_OBJECT

public:
	QDoubleSlider(QWidget * parent = Q_NULLPTR);
	//~QDoubleSlider();

	/**
	* Note that when using, the parameters inside must be decimal
	* Otherwise, the integer setValue of the parent class will be called!
	*/ 
	void setRange(double Min, double Max);
	void setMinimum(double Min);
	double minimum() const;
	void setMaximum(double Max);
	double maximum() const;
	double value() const;

public slots:
	void setValue(int value);
	

public slots:

	/**
	* Note that when using, the parameters inside must be decimal
	* Otherwise, the integer setValue of the parent class will be called!
	*/
	void setValue(double Value, bool BlockSignals = false);

signals:
	void valueChanged(double Value);
signals:
	void rangeChanged(double Min, double Max);

private:
	double	multiplier;

private:
	
};

#endif



