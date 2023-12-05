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

#ifndef __DisplayDockWidget_h__
#define __DisplayDockWidget_h__

#include <QDockWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>

#include "Process/DataReaderAndWriter.hpp"

#include "Utility/WidgetsCommon.hpp"
#include "Utility/DicomCommon.hpp"
#include "Utility/QSliderDoubleRange.hpp"

#include "Utility/QDoubleSlider.hpp"
#include "Utility/QDoubleSliderDoubleRange.hpp"

#include <QSlider>
#include <QDoubleSpinBox>

class QVolumeReadWrite_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeReadWrite_Widget(QWidget * parent = Q_NULLPTR);
	~QVolumeReadWrite_Widget();

	QVBoxLayout *centerLayout;

	// widgets
	QGridLayout *readDataLayout;
	void setup_readwrite_Widgets();

	QPushButton *readDcms_processButton;
	QPushButton *readMhd_processButton;
	QPushButton *readFeimos_processButton;
	QPushButton *readPngs_processButton;
	QPushButton *readJpgs_processButton;

	QPushButton *writeMhd_processButton;
	QPushButton *writeFeimos_processButton;

	void NoDataDisableWidgets();
	void NoStatisticsDisableWidgets();
	void HaveDataEnableWidgets();
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData);

};

class QVolumeStatistics_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeStatistics_Widget(QWidget * parent = Q_NULLPTR);
	~QVolumeStatistics_Widget();

	QVBoxLayout *centerLayout;

	QGridLayout *GetStatisticsLayout;
	QPushButton *getOriginInfo_processButton;

	// widgets: scale
	QGridLayout *ScaleTheDataValue_Layout;
	void setupScale_Widgets();
	QPushButton *Scale_processButton;
	QDoubleSlider * Scale_Slider;
	QDoubleSpinBox * Scale_SpinBox;

	// widgets: clamp
	QGridLayout *Clamp_Layout;
	void setupClamp_Widgets();
	QPushButton *Clamp_Button;
	QDoubleSliderDoubleRange *ClampRangeSlider;
	QDoubleSpinBox * ClampRangeMin_SpinBox;
	QDoubleSpinBox * ClampRangeMax_SpinBox;

	void NoDataDisableWidgets();
	void NoStatisticsDisableWidgets();
	void HaveDataEnableWidgets();
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData);
};

class QVolumeDisplayRange_Widget : public QGroupBox {
	Q_OBJECT
public:
	QVolumeDisplayRange_Widget(QWidget * parent = Q_NULLPTR);
	~QVolumeDisplayRange_Widget();

	// data
	float WindowWidth, WindowLevel;
	float originDataMin, originDataMax;
	float dataMin, dataMax;

	// widgets
	QVBoxLayout *centerLayout;

	// widgets: data range
	QGridLayout *WWWL_Layout;
	void setupWWWL_Widgets();
	QPushButton *WW_Button;
	QDoubleSlider * WW_Slider;
	QDoubleSpinBox * WW_SpinBox;
	QPushButton *WL_Button;
	QDoubleSlider * WL_Slider;
	QDoubleSpinBox * WL_SpinBox;

	QGridLayout *DataRange_Layout;
	void setupAxis_Widgets();
	QPushButton *dataRange_Button;
	QDoubleSliderDoubleRange *dataRangeSlider;
	QDoubleSpinBox * dataRangeMin_SpinBox;
	QDoubleSpinBox * dataRangeMax_SpinBox;

	// widgets: display axis
	QGridLayout * axis_Layout;
	QLabel* display_axis_xy_label;
	QRadioButton* display_axis_xy_radio;
	QLabel* display_axis_yz_label;
	QRadioButton* display_axis_yz_radio;
	QLabel* display_axis_xz_label;
	QRadioButton* display_axis_xz_radio;
	QSlider* display_axis_xy_slider;
	QSpinBox* display_axis_xy_spinbox;
	QSlider* display_axis_yz_slider;
	QSpinBox* display_axis_yz_spinbox;
	QSlider* display_axis_xz_slider;
	QSpinBox* display_axis_xz_spinbox;
	QButtonGroup* display_axis_ButtonGroup;

	void NoDataDisableWidgets();
	void NoStatisticsDisableWidgets();
	void HaveDataEnableWidgets();
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData);

signals:
	void dataRangeChanged();
private slots:
	void setNewRange_WW(double ww);
	void setNewRange_WL(double wl);
	void setNewRange_DataMin(double datamin);
	void setNewRange_DataMax(double datamax);
};

#include "FrameBuffer.hpp"

class DisplayDockWidget : public QDockWidget {
	Q_OBJECT
public:
	DisplayDockWidget(QWidget * parent = Q_NULLPTR);
	~DisplayDockWidget();

public:
	void getPredefinedInfo();

private:
	QVBoxLayout *centerLayout;
	QFrame *dockCentralWidget;
	void setpuWidgets();

	FileDirSet_Widget *m_FileDirSet_Widget;
	QSetFormat_Widget * m_QSetFormat_Widget;

	QVolumeReadWrite_Widget * m_QVolumeReadWrite_Widget;
	QVolumeStatistics_Widget * m_QVolumeStatistics_Widget;
	QVolumeDisplayRange_Widget * m_QVolumeDisplayRange_Widget;

	void displayVolumeInfo();
	void displayVolumeStatistics();

	void clearVolumeInfo();
	void clearVolumeStatistics();

private slots:
	void readDcmsData();
	void readMhdData();
	void readFeimosData();
	void readPNGsData();
	void readJPGsData();

	void writeMhdData();
	void writeFeimosData();

	void getStatistics();

private:
	VolumeData volumeData;

	// display buffer
	FrameBuffer framebuffer;

	DataReaderAndWriter m_DataRW;
	GenerateFormat generateFormat;

	// process setting
	QString InputFolder;
	QString OutputFolder;
	QString OutputFileName;
	QString InputFilePath;

private:
	void NoDataDisableWidgets() {
		m_QVolumeReadWrite_Widget->NoDataDisableWidgets();
		m_QVolumeStatistics_Widget->NoDataDisableWidgets();
		m_QVolumeDisplayRange_Widget->NoDataDisableWidgets();

		m_QVolumeReadWrite_Widget->NoStatisticsDisableWidgets();
		m_QVolumeStatistics_Widget->NoStatisticsDisableWidgets();
		m_QVolumeDisplayRange_Widget->NoStatisticsDisableWidgets();
	}
	void NoStatisticsDisableWidgets() {
		m_QVolumeReadWrite_Widget->NoStatisticsDisableWidgets();
		m_QVolumeStatistics_Widget->NoStatisticsDisableWidgets();
		m_QVolumeDisplayRange_Widget->NoStatisticsDisableWidgets();
	}

	void HaveDataEnableWidgets() {
		m_QVolumeReadWrite_Widget->HaveDataEnableWidgets();
		m_QVolumeStatistics_Widget->HaveDataEnableWidgets();
		m_QVolumeDisplayRange_Widget->HaveDataEnableWidgets();
	}
	void HaveStatisticsDataEnableWidgets(const VolumeData& volumeData) {
		m_QVolumeReadWrite_Widget->HaveStatisticsDataEnableWidgets(volumeData);
		m_QVolumeStatistics_Widget->HaveStatisticsDataEnableWidgets(volumeData);
		m_QVolumeDisplayRange_Widget->HaveStatisticsDataEnableWidgets(volumeData);
	}
};


#endif



