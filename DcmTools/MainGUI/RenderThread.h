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

	/** 将Dicom生成MHD文件 DCMTK版
	* 需要保证所有dcm文件的宽高一致
	* dirPath: 存放dicom文件的路径
	* outputDir：输出文件夹
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


	/** 将MHD文件生成我的易读的体数据格式（二进制无压缩）
	* filePath: 输入的.mhd文件路径
	* outputDir：输出文件夹
	* outName: 输出的文件名
	*/
	void MHDGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName);

	
	/** 将MHD文件生成PBRT体数据格式（无压缩）
	* filePath: 输入的.mhd文件路径
	* outputDir：输出文件夹
	* outName: 输出的文件名
	*/
	void MHDGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName);

signals:
	void PrintString(const char* s);
	void PrintDataD(const char* s, const double data);
	//void PrintDataF(char* s, float data);
	//void PrintDataI(char* s, int data);
	void PaintBuffer(DcmTools::Uchar* buffer, int width, int height, int channals);

private slots:
	


public:
	void run();


};








#endif






