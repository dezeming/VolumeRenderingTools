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

#include "QDoubleSlider.hpp"

QDoubleSlider::QDoubleSlider(QWidget* pParent) :
	QSlider(pParent),
	multiplier(10000.0)
{
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

	setSingleStep(1);

	setOrientation(Qt::Horizontal);
	setFocusPolicy(Qt::NoFocus);
}

void QDoubleSlider::setValue(int Value)
{
	
	emit valueChanged((double)Value / multiplier);
}

void QDoubleSlider::setValue(double Value, bool BlockSignals)
{
	QSlider::blockSignals(BlockSignals);

	QSlider::setValue(Value * multiplier);

	if (!BlockSignals)
		emit valueChanged(Value);

	QSlider::blockSignals(false);
}

void QDoubleSlider::setRange(double Min, double Max)
{
	QSlider::setRange(Min * multiplier, Max * multiplier);

	emit rangeChanged(Min, Max);
}

void QDoubleSlider::setMinimum(double Min)
{
	QSlider::setMinimum(Min * multiplier);

	emit rangeChanged(minimum(), maximum());
}

double QDoubleSlider::minimum() const
{
	return QSlider::minimum() / multiplier;
}

void QDoubleSlider::setMaximum(double Max)
{
	QSlider::setMaximum(Max * multiplier);

	emit rangeChanged(minimum(), maximum());
}

double QDoubleSlider::maximum() const
{
	return QSlider::maximum() / multiplier;
}

double QDoubleSlider::value() const
{
	int Value = QSlider::value();
	return (double)Value / multiplier;
}

