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

	/** ��Dicom����MHD�ļ� DCMTK��
	* ��Ҫ��֤����dcm�ļ��Ŀ��һ��
	* dirPath: ���dicom�ļ���·��
	* outputDir������ļ���
	* ����������ŵ�Output�ļ�����
	* ��Ѱ治֧��JPEG2000ѹ����ʽ�����Ѱ�֧�֣�
	*/
	void DcmMakeMHDFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName);


	/** ��Dicom����MHD�ļ� DCMTK��
	* ��Ҫ��֤����dcm�ļ��Ŀ��һ��
	* dirPath: ���dicom�ļ���·��
	* outputDir������ļ���
	* ����������ŵ�Output�ļ�����
	* ֧�ָ�ʽ��ȫ
	*/
	void DcmMakeMHDFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName);


	/** ��MHD��ʽ�������ݸ��� permute�趨���������ת
	* ��0,1,2��䵽������
	* filePath��mhd�ļ�·��
	* outputDir������ļ���
	* ����������ŵ�outputDir�ļ�����
	*/
	void MHDRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]);


	/** MHD����ѡ���������/���ҷ�ת
	* filePath��mhd�ļ�·��
	* outputDir������ļ���
	*/
	void MHDFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]);


	/** ��MHD�ļ����ݱ��˵�VolumeRender�ı߽��ʽ���ü�������
	* filePath: �����.mhd�ļ�·��
	* outputDir������ļ���
	* outName: ������ļ���
	* ԭʼ�߽����ᶼ�� [-bound,bound]=[-0.5,0.5]������Ϊ[center]=[0]��
	* �ü���Ҫ��֤ÿ���᷶Χ�� [center-bound, center+bound]��[-0.5,0.5]����
	*/
	void MHDClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]);


	/** ��MHD�ļ�resize
	* filePath: �����.mhd�ļ�·��
	* outputDir������ļ���
	* outName: ������ļ���
	* Interval: ������������൱�ڷֱ��ʽ���Interval����ֻ֧��������
	*/
	void ResizeMHD_IntervalSampling(const QString& filePath, const QString& outputDir, const QString& outName, int Interval = 2);


	/** ��MHD�ļ������ҵ��׶��������ݸ�ʽ����������ѹ����
	* filePath: �����.mhd�ļ�·��
	* outputDir������ļ���
	* outName: ������ļ���
	*/
	void MHDGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName);

	
	/** ��MHD�ļ�����PBRT�����ݸ�ʽ����ѹ����
	* filePath: �����.mhd�ļ�·��
	* outputDir������ļ���
	* outName: ������ļ���
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






