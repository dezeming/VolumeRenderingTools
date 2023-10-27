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

#ifndef __RenderThread_h__
#define __RenderThread_h__

#include <QThread>
#include <QString>

#include "Core\FrameBuffer.h"
#include "IMAGraphicsView.h"

class RenderThread :public QThread {

	Q_OBJECT
public:
	RenderThread();
	~RenderThread();

public:
	bool renderFlag;
	bool paintFlag;
	FrameBuffer* p_framebuffer;

	void process();


	void DCMTK_Test();
	void GDCM_Test();

	/** Generate DCMTK version of MHD file from Dicom
	* It is necessary to ensure that the width and height of all DCM files are consistent
	* dirPath: Path to store dicom files
	* outputDir：Output Folder
	* 将输出结果存放到Output文件夹内
	* 免费版不支持JPEG2000压缩格式（付费版支持）
	*/
	void DcmMakeMHDFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName);

	/** 将Dicom生成MHD文件 DCMTK版
	* 需要保证所有dcm文件的宽高一致
	* dirPath: 存放dicom文件的路径
	* outputDir：输出文件夹
	* 将输出结果存放到Output文件夹内
	* 支持格式最全
	*/
	void DcmMakeMHDFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName);


	/** 将MHD格式的体数据根据 permute设定的轴进行旋转
	* 将0,1,2填充到数组内
	* filePath：mhd文件路径
	* outputDir：输出文件夹
	* 将输出结果存放到outputDir文件夹内
	*/
	void MHDRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]);


	/** MHD根据选择的轴上下/左右翻转
	* filePath：mhd文件路径
	* outputDir：输出文件夹
	*/
	void MHDFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]);


	/** 将MHD文件根据本人的VolumeRender的边界格式来裁剪体数据
	* filePath: 输入的.mhd文件路径
	* outputDir：输出文件夹
	* outName: 输出的文件名
	* 原始边界三轴都是 [-bound,bound]=[-0.5,0.5]，中心为[center]=[0]。
	* 裁剪需要保证每个轴范围在 [center-bound, center+bound]在[-0.5,0.5]以内
	*/
	void MHDClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]);


	/** 将MHD文件resize
	* filePath: 输入的.mhd文件路径
	* outputDir：输出文件夹
	* outName: 输出的文件名
	* Interval: 降采样间隔，相当于分辨率降低Interval倍，只支持整数。
	*/
	void ResizeMHD_IntervalSampling(const QString& filePath, const QString& outputDir, const QString& outName, int Interval = 2);


	/** Generate MHD files into my readable body data format (binary uncompressed)
	* filePath: Input. mhd file path
	* outputDir：Output Folder
	* outName: Output file name
	*/
	void MHDGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName);

	
	/**Generate MHD files into PBRT volume data format (without compression)
	*filePath: The path to the input. mhd file
	*outputDir: Output folder
	*outName: The file name of the output
	*/
	void MHDGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName);

signals:
	void PrintString(const char* s);
	void PrintDataD(const char* s, const double data);

	void PaintBuffer(DcmTools::Uchar* buffer, int width, int height, int channals);

private slots:
	


public:
	void run();


};








#endif






