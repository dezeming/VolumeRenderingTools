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

/*
	This code is transplanted from <https://github.com/ThisIsClark/Qt-RangeSlider>

	The way to use this widget:

	rsH = new RangeSlider(Qt::Horizontal, RangeSlider::Option::DoubleHandles, nullptr);
		rsH->setMinimumSize(120,30);
		rsH->SetRange(0, 100);
		rsH->SetLowerValue(0);
		rsH->SetUpperValue(100);

	rsV = new RangeSlider(Qt::Vertical, RangeSlider::Option::DoubleHandles, nullptr);
	rsHsingleLeft = new RangeSlider(Qt::Horizontal, RangeSlider::Option::LeftHandle, nullptr);
	rsVsingleLeft = new RangeSlider(Qt::Vertical, RangeSlider::Option::LeftHandle, nullptr);
	rsHsingleRight = new RangeSlider(Qt::Horizontal, RangeSlider::Option::RightHandle, nullptr);
	rsVsingleRight = new RangeSlider(Qt::Vertical, RangeSlider::Option::RightHandle, nullptr);

*/

#ifndef __QDoubleSliderDoubleRange_h__
#define __QDoubleSliderDoubleRange_h__


#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class QDoubleSliderDoubleRange : public QWidget
{
	Q_OBJECT
		Q_ENUMS(QSliderDoubleRangeTypes)

public:
	enum Option {
		NoHandle = 0x0,
		LeftHandle = 0x1,
		RightHandle = 0x2,
		DoubleHandles = LeftHandle | RightHandle
	};
	Q_DECLARE_FLAGS(Options, Option)

		QDoubleSliderDoubleRange(QWidget* aParent = Q_NULLPTR);
	QDoubleSliderDoubleRange(Qt::Orientation ori, Options t = DoubleHandles, QWidget* aParent = Q_NULLPTR);

	QSize minimumSizeHint() const override;

	double GetMinimun() const;
	void SetMinimum(double aMinimum);

	int GetMaximun() const;
	void SetMaximum(double aMaximum);

	int GetLowerValue() const;
	void SetLowerValue(double aLowerValue);

	int GetUpperValue() const;
	void SetUpperValue(double aUpperValue);

	void SetRange(double aMinimum, double aMaximum);
	void SetTracking(bool enable);

protected:
	void paintEvent(QPaintEvent* aEvent) override;
	void mousePressEvent(QMouseEvent* aEvent) override;
	void mouseMoveEvent(QMouseEvent* aEvent) override;
	void mouseReleaseEvent(QMouseEvent* aEvent) override;
	void changeEvent(QEvent* aEvent) override;

	QRectF firstHandleRect() const;
	QRectF secondHandleRect() const;
	QRectF handleRect(double aValue) const;

signals:
	void lowerValueChanged(double aLowerValue);
	void upperValueChanged(double aUpperValue);
	void rangeChanged(double aMin, double aMax);

public slots:
	void setLowerValue(double aLowerValue);
	void setUpperValue(double aUpperValue);
	void setMinimum(double aMinimum);
	void setMaximum(double aMaximum);

private:
	Q_DISABLE_COPY(QDoubleSliderDoubleRange)
		float currentPercentage();
	double validLength() const;

	double mMinimum;
	double mMaximum;
	double mLowerValue;
	double mUpperValue;
	bool mEnableTracking;
	bool mFirstHandlePressed;
	bool mSecondHandlePressed;
	double mInterval;
	double mDelta;
	QColor mBackgroudColorEnabled;
	QColor mBackgroudColorDisabled;
	QColor mBackgroudColor;
	Qt::Orientation orientation;
	Options type;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDoubleSliderDoubleRange::Options)








#endif



