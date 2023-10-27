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
	* outputDir��Output Folder
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


	/** Generate MHD files into my readable body data format (binary uncompressed)
	* filePath: Input. mhd file path
	* outputDir��Output Folder
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






