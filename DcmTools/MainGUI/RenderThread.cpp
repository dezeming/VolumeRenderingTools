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

#include "RenderThread.h"
#include "DebugText.hpp"
#include <QTime>

// DCMTK
#include "dcmdata/dcfilefo.h"
#include "config/osconfig.h"
#include "dcmdata/dctk.h"
#include "dcmimage/diargimg.h"

// GDCM
#include "gdcmReader.h"
#include "gdcmWriter.h"
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmTesting.h"
#include "gdcmGlobal.h"
#include "gdcmDicts.h"
#include "gdcmDict.h"
#include "gdcmStringFilter.h"
#include "gdcmImageReader.h"

// VTK库
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>
#include <vtkDataArray.h>
#include <vtkImageImport.h>

// Qt
#include <QDir>
#include <QStringList>
// STL
#include <algorithm>
#include <iostream>
#include <fstream>


RenderThread::RenderThread() {
	paintFlag = false;
	renderFlag = false;
}

RenderThread::~RenderThread() {
	
}

void RenderThread::process() {
	emit PrintString("Prepared to Process");

	// DCMTK_Test();
	// GDCM_Test();


	// 测试例子1：Dicom转MHD-RAW文件
	if (0) {
		emit PrintString("Convert .dcm files into VTK' .mdh-.raw format using DCMTK lib.");
		DcmMakeMHDFile_DCMTK(
			"D:/Datasets/DicomImages/Heart/", 
			"./Output", 
			"sample");
	}
	if (0) {
		emit PrintString("Convert .dcm files into VTK' .mdh-.raw format using GDCM lib.");
		DcmMakeMHDFile_GDCM(
			"E:/Datasets/MaxinData/3D_AX+C_Series0010/",
			"./Output", 
			"PT096");
	}

	if (0) {

		for (int i = 1; i < 486; i++) {
			QString inputfolderPath = "S:/CT-DicomDatasets/Cerebral_aneurysm/AIneurysm/R"
				+ QString::number(i / 100) + QString::number(i % 100 / 10) +QString::number(i % 10) + "/";

			QString outputfolderPath = "S:/CT-DicomDatasets/GeneratedMhdData/R"
				+ QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10) + "/";

			QDir dir;
			if (!dir.exists(outputfolderPath)) {
				if (dir.mkpath(outputfolderPath)) {
					emit PrintString("Folder created successfully."); 
				}
				else {
					emit PrintString("Folder creation failed.");
					continue;
				}
			}
			else {
				emit PrintString("Folder already exists.");
			}

			DcmMakeMHDFile_GDCM(
				inputfolderPath,
				outputfolderPath,
				"R" + QString::number(i / 100) + QString::number(i % 100 / 10) + QString::number(i % 10));

		}




	}


	// 降采样
	if (0) {
		emit PrintString("Resize .mhd-.raw file.");
		ResizeMHD_IntervalSampling(
			"D:/DataSets/OpenScientificVisData-Origin/synthetic_truss_with_five_defects/synthetic_truss.mhd", 
			"./Output", 
			"synthetic_truss_small", 2);
	}

	// 测试例子2：MHD文件剪裁
	if (0) {
		emit PrintString("Clip .mhd-.raw file.");
		double center[3] = { 0.0, 0, 0.0 };
		double bound[3] = { 0.212, 0.5, 0.5 };
		MHDClip(
			"D:/DataSets/OpenScientificVisData/Chameleon/chameleon.mhd", 
			"Chameleon_small", 
			"./Output",
			center, bound);
	}

	// 测试例子3：MHD根据轴来旋转
	if (0) {
		emit PrintString("Permute axis of .mhd-.raw file.");
		int permute[3] = {0,2,1};
		MHDRotateAxis(
			"D:/DataSets/OpenScientificVisData/Bunny/bunny.mhd", 
			"bunny_r", 
			"./Output", 
			permute);
	}

	// 测试例子4：MHD根据选择的轴上下/左右颠倒
	if (0) {
		emit PrintString("Flip axis of .mhd-.raw file.");
		int flip[3] = { 0,1,0 };
		MHDFlipAxis(
			"C:/Developer/1_DataSet/MhdVolumeData/mhd_Heart2/sample.mhd", 
			"sample", 
			"./Output", 
			flip);
	}

	// 测试例子4：MHD生成我的数据格式
	if (0) {
		emit PrintString("Convert .mhd-.raw file into Feimos file format.");
		MHDGenerateFeimosData(
			"sample.mhd", 
			"./Output", 
			"sample");
	}

	// 开始执行处理
	{
		QTime t;
		t.start();
		
		// 目前并不做实际任务

		for (int i = 0; i < 800; i++) {
			for(int j = 0; j < 600; j++) {
				
				p_framebuffer->set_uc(i, j, 0, 255);
				p_framebuffer->set_uc(i, j, 1, 0);
				p_framebuffer->set_uc(i, j, 2, 0);
				p_framebuffer->set_uc(i, j, 3, 255);
			}
		}

		//emit PaintBuffer(p_framebuffer->getUCbuffer(), 800, 600, 4);
	}


	emit PrintString("Process finished");
}

void RenderThread::run() {

}



struct DcmFile {
	Uint16 *pixData16;
	float position;
};
bool DateUpSort(const DcmFile &f1, const DcmFile &f2) {
	if (f1.position < f2.position) return true;
	else return false;
}
/** 将Dicom生成MHD文件 DCMTK版
* 需要保证所有dcm文件的宽高一致
* dirPath: 存放dicom文件的路径
* 将输出结果存放到Output文件夹内
*/ 
void RenderThread::DcmMakeMHDFile_DCMTK(const QString& dirPath, const QString& outputDir, const QString& outName) {
	QDir dir(dirPath);
	if (!dir.exists()) {
		emit PrintDataD("(!dir.exists()) : ", 0);
		return;
	}
		
	QStringList filters;
	filters << "*.dcm";
	//设置类型过滤器，只为文件格式，不要符号链接
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	//设置文件名称过滤器，filters格式
	dir.setNameFilters(filters);
	// 记录文件数量
	int file_count = dir.count();
	emit PrintDataD("file_count : ", file_count);
	//TextDinodonN("file_count: ", file_count);
	if (file_count <= 0)
		return;
	QString _DirPath = dirPath;
	if (dirPath[dirPath.size() - 1] != '/') _DirPath = _DirPath + "/";
	
	int width = 0, height = 0, imageNum = file_count;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	// 获取宽高信息
	for(int i = 0;i < file_count;i++)
	{
		
		QString file_Path = _DirPath + dir[i];
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();

		if (!Metalnfo) {
			imageNum--;
			emit PrintDataD("No Metalnfo in File : ", i);
			continue;
		}

		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();

		if (!data) {
			imageNum--;
			emit PrintDataD("No data in File : ", i);
			continue;
		}

		DcmElement *element = NULL;
		data->findAndGetElement(DCM_PixelData, element);

		OFString ImageWidth;
		data->findAndGetOFString(DCM_Columns, ImageWidth);
		if (width != 0 && width != atoi(ImageWidth.data())) {
			emit PrintDataD("The width of the image sequence is inconsistent.", i);
			return;
		}else width = atoi(ImageWidth.data());
		OFString ImageHeight;
		data->findAndGetOFString(DCM_Rows, ImageHeight);
		if (height != 0 && height != atoi(ImageHeight.data())) {
			emit PrintDataD("The height of the image sequence is inconsistent.", i);
			return;
		}
		else height = atoi(ImageHeight.data());


		OFString PixelSpacing;
		data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing);
		pixelSpacing_X = atof(PixelSpacing.data());
		pixelSpacing_Y = pixelSpacing_X;
		OFString SliceThickness;
		data->findAndGetOFString(DCM_SliceThickness, SliceThickness);
		pixelSpacing_Z = atof(SliceThickness.data());

		{
			//emit PrintDataD("width : ", width);
			//emit PrintDataD("height : ", height);
			//emit PrintDataD("pixelSpacing_X : ", pixelSpacing_X);
			//emit PrintDataD("pixelSpacing_Y : ", pixelSpacing_Y);
			//emit PrintDataD("SliceThickness : ", pixelSpacing_Z);
		}
	}
	
	Uint16 * m_data = new Uint16[width * height * imageNum];
	if (!m_data) {
		emit PrintDataD("m_data is null ", 0);
		return;
	}

	// 开始处理每幅图像
	std::vector<DcmFile> dcmFileVec;
	for (int i = 0; i < file_count; i++) {
		// 文件路径
		QString file_Path = _DirPath + dir[i];

		// DcmFileFormat 会自动释放内存
		DcmFileFormat dfile;
		dfile.loadFile(file_Path.toStdString().c_str());
		DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
		if (!Metalnfo) {
			emit PrintDataD("No Metalnfo in File : ", i);
			continue;
		}
		DcmTag Tag;
		Tag = Metalnfo->getTag();
		Uint16 G_tag = Tag.getGTag();
		DcmDataset *data = dfile.getDataset();
		if (!data) {
			emit PrintDataD("No data in File : ", i);
			continue;
		}
		DcmElement *element = NULL;
		data->findAndGetElement(DCM_PixelData, element);

		OFString ImageWidth;
		data->findAndGetOFString(DCM_Columns, ImageWidth);
		width = atoi(ImageWidth.data());
		OFString ImageHeight;
		data->findAndGetOFString(DCM_Rows, ImageHeight);
		height = atoi(ImageHeight.data());
		//emit PrintDataD("width : ", width);
		//emit PrintDataD("height : ", height);
		OFString ImagePos;
		data->findAndGetOFString(DCM_SliceLocation, ImagePos);

		Uint16* dat_t;
		element->getUint16Array(dat_t);
		memcpy(m_data + i * width * height, dat_t, width * height * sizeof(Uint16));

		DcmFile dcmF;
		dcmF.pixData16 = m_data + i * width * height;
		dcmF.position = atof(ImagePos.data());
		dcmFileVec.push_back(dcmF);
	}

	std::sort(dcmFileVec.begin(), dcmFileVec.end(), DateUpSort);

	for (int i = 0; i < file_count; i++) {
		QTime t;
		t.start();
		// emit PrintDataD("dcmFileVec : ", dcmFileVec[i].position); 
		while (t.elapsed() < 20);
	}

	int shape1[3] = { width, height, imageNum };
	int stride1[3] = { 1, width, width * height };

	// 以前写的代码，现在使用VTK生成压缩版本的数据
	{
		//std::ofstream file("sample.raw", std::ios::binary);
		//for (int i = 0; i < imageNum; i++) file.write((char*)dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
		//file.close();
		//FILE *fp = fopen("./sample.raw", "wb");
		//for (int i = 0; i < imageNum; i++) fwrite(dcmFileVec[i].pixData16, sizeof(Uint16), width * height, fp);
		//fclose(fp);
	}


	Uint16 * aim_data = new Uint16[width * height * imageNum];
	if (!aim_data) {
		emit PrintDataD("aim_data is null ", 0);
		return;
	}
	bool flag = false;
	for (int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			emit PrintDataD("(dcmFileVec[i].pixData16 == nullptr) ", i);
			flag = true;
		}
	}

	for (int i = 0; i < imageNum; i++) {
		memcpy((Uint16*)aim_data + i * width * height, dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
	}

	// 以前写的代码，现在使用VTK生成压缩版本的数据
	{
		//std::ofstream file("sample.raw", std::ios::binary);
		//file.write((char*)aim_data, width * height * imageNum * sizeof(Uint16));
		//file.close();
	}


	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, width - 1, 0, height - 1, 0, imageNum - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(aim_data);
	imageImport->Update();

	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName(( outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] m_data;
	delete[] aim_data;
}

// ********************************** //
// **** 将Dicom生成MHD文件 GDCM版 *** //
// ********************************** //
bool compareInstance(gdcm::DataSet const & ds1, gdcm::DataSet const & ds2)
{
	// Study Instance UID
	gdcm::Attribute<0x0020, 0x000d> at1;
	at1.Set(ds1);
	gdcm::Attribute<0x0020, 0x000d> at2;
	at2.Set(ds2);
	return at1 < at2;
}
/** 将Dicom生成MHD文件 GDCM版
* dirPath: 存放dicom文件的路径
* 将输出结果存放到Output文件夹内
*/
void RenderThread::DcmMakeMHDFile_GDCM(const QString& dirPath, const QString& outputDir, const QString& outName) {
	QDir dir(dirPath);
	if (!dir.exists()) {
		emit PrintDataD("(!dir.exists()) : ", 0);
		return;
	}

	QStringList filters;
	filters << "*.dcm";
	//设置类型过滤器，只为文件格式，不要符号链接
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	//设置文件名称过滤器，filters格式
	dir.setNameFilters(filters);
	// 记录文件数量
	int file_count = dir.count();
	emit PrintDataD("file_count : ", file_count);
	//TextDinodonN("file_count: ", file_count);
	if (file_count <= 0)
		return;
	QString _DirPath = dirPath;
	if (dirPath[dirPath.size() - 1] != '/') _DirPath = _DirPath + "/";


	std::vector<DcmFile> dcmFileVec;
	int width, height, imageNum = file_count;
	double pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z;
	for (int i = 0; i < file_count; i++) {
		gdcm::Reader reader;

		QString file_Path = _DirPath + dir[i];
		reader.SetFileName(file_Path.toStdString().c_str());
		if (!reader.Read()) continue;
		// The output of gdcm::Reader is a gdcm::File
		gdcm::File &file = reader.GetFile();
		// the dataset is the the set of element we are interested in:
		gdcm::DataSet &ds = file.GetDataSet();

		const gdcm::Global& g = gdcm::Global::GetInstance();
		const gdcm::Dicts &dicts = g.GetDicts();
		const gdcm::Dict &pubdict = dicts.GetPublicDict();

		// Image Position
		gdcm::Attribute<0x0020, 0x1041> at_position;
		at_position.SetFromDataSet(ds);
		gdcm::Attribute<0x0020, 0x1041>::ArrayType v_position = at_position.GetValue();
		
		// Rows
		gdcm::Attribute<0x0028, 0x0010> at_rows;
		at_rows.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0010>::ArrayType v_rows = at_rows.GetValue();
		height = v_rows;

		// Columns
		gdcm::Attribute<0x0028, 0x0011> at_columns;
		at_columns.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0011>::ArrayType v_columns = at_columns.GetValue();
		width = v_columns;

		// Spacing between slices
		gdcm::Attribute<0x0018, 0x0050> at_Spacing_slices;
		at_Spacing_slices.SetFromDataSet(ds);
		gdcm::Attribute<0x0018, 0x0050>::ArrayType v_Spacing_slices = at_Spacing_slices.GetValue();
		pixelSpacing_Z = v_Spacing_slices;

		// Spacing between slices
		gdcm::Attribute<0x0028, 0x0030> at_Pixel_Spacing;
		at_Pixel_Spacing.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0030>::ArrayType vv_Pixel_Spacing = at_Pixel_Spacing.GetValue();
		pixelSpacing_X = pixelSpacing_Y = vv_Pixel_Spacing;

		// 像素位数
		gdcm::Attribute<0x0028, 0x0100> at_bit;
		at_bit.SetFromDataSet(ds);
		gdcm::Attribute<0x0028, 0x0100>::ArrayType v_bit = at_bit.GetValue();

		DcmFile dcmfile1;
		dcmfile1.position = v_position;
		dcmFileVec.push_back(dcmfile1);

		{
			//std::cout << "Image Position = " << v_position << std::endl;
			//std::cout << "Height = " << v_rows << std::endl;
			//std::cout << "Width = " << v_columns << std::endl;
			//std::cout << "Spacing between slices = " << v_Spacing_slices << std::endl;
			//std::cout << "Pixel Spacing = " << vv_Pixel_Spacing << std::endl;
			//std::cout << "bit allocate = " << v_bit << std::endl;
		}

		// data
		{
			//gdcm::Attribute<0x7fe0, 0x0010> at_dcm_data;
			//at_dcm_data.SetFromDataSet(ds);
		}

	}

	Uint16 * m_data = new Uint16[width * height * imageNum];
	if (!m_data) {
		emit PrintDataD("m_data is null ", 0);
		return;
	}
	for (int i = 0; i < file_count; i++) {
		gdcm::Reader reader;

		QString file_Path = _DirPath + dir[i];

		gdcm::ImageReader ir;
		ir.SetFileName(file_Path.toStdString().c_str());
		if (!ir.Read())
		{
			//Read failed
			continue;
		}
		const gdcm::Image &gimage = ir.GetImage();

		std::vector<char> vbuffer;
		vbuffer.resize(gimage.GetBufferLength());
		char *buffer = &vbuffer[0];
		
		{
			// 512 * 512 * 2 = 524288
			//std::cout << "gimage.GetBufferLength() = " << gimage.GetBufferLength() << std::endl;
		}

		gimage.GetBuffer(buffer);


		memcpy(m_data + i * width * height, buffer, width * height * sizeof(Uint16));
		dcmFileVec[i].pixData16 = m_data + i * width * height;
	}

	std::sort(dcmFileVec.begin(), dcmFileVec.end(), DateUpSort);

	Uint16 * aim_data = new Uint16[width * height * imageNum];
	if (!aim_data) {
		emit PrintDataD("aim_data is null ", 0);
		return;
	}
	bool flag = false;
	for (int i = 0; i < imageNum; i++) {
		if (dcmFileVec[i].pixData16 == nullptr) {
			emit PrintDataD("(dcmFileVec[i].pixData16 == nullptr) ", i);
			flag = true;
		}
	}

	for (int i = 0; i < imageNum; i++) {
		memcpy((Uint16*)aim_data + i * width * height, dcmFileVec[i].pixData16, width * height * sizeof(Uint16));
	}

	{
		//std::ofstream file("sample.raw", std::ios::binary);
		//file.write((char*)aim_data, width * height * imageNum * sizeof(Uint16));
		//file.close();
	}


	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(pixelSpacing_X, pixelSpacing_Y, pixelSpacing_Z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, width - 1, 0, height - 1, 0, imageNum - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(aim_data);
	imageImport->Update();

	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] m_data;
	delete[] aim_data;

}


/** 将MHD格式的体数据根据 permute设定的轴进行旋转
* 将0,1,2填充到数组内
* filePath：mhd文件路径
** 将输出结果存放到Output文件夹内
*/ 
void RenderThread::MHDRotateAxis(const QString& filePath, const QString& outputDir, const QString& outName, int permute[3]) {

	if (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == permute[1]) {
		emit PrintString("Error in MHDRotateAxis: (permute[0] == permute[1] || permute[0] == permute[2] || permute[2] == flip[1])");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (permute[i] > 2 || permute[i] < 0) {
			emit PrintString("Error in MHDRotateAxis: (permute[i] > 2 || permute[i] < 0)");
			return;
		}
	}

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace[3];
	PixelSpace[0] = spacing[0];
	PixelSpace[1] = spacing[1];
	PixelSpace[2] = spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	// 维度置换
	int dimM_new[3];
	double PixelSpace_new[3];
	for (int i = 0; i < 3; i++) {
		dimM_new[i] = dimM[permute[i]];
		PixelSpace_new[i] = PixelSpace[permute[i]];
	}
	int stride[3] = { 1, width, width * height };
	short * data_aim = new short[width * height * imageNum];
	for (int i = 0; i < dimM_new[0]; i++) {
		for (int j = 0; j < dimM_new[1]; j++) {
			for (int k = 0; k < dimM_new[2]; k++) {
				int offsetnew = i + j * dimM_new[0] + k * dimM_new[0] * dimM_new[1];

				int offset = i * stride[permute[0]] + j * stride[permute[1]] + k * stride[permute[2]];

				data_aim[offsetnew] = data_m[offset];
			}
		}
	}


	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_new[0], PixelSpace_new[1], PixelSpace_new[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM_new[0] - 1, 0, dimM_new[1] - 1, 0, dimM_new[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;
}


/** MHD根据选择的轴上下/左右翻转 flip[i]不为0的轴都会进行翻转
* dirPath: 存放dicom文件的路径
* 将输出结果存放到Output文件夹内
*/
void RenderThread::MHDFlipAxis(const QString& filePath, const QString& outputDir, const QString& outName, int flip[3]) {

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace[3];
	PixelSpace[0] = spacing[0];
	PixelSpace[1] = spacing[1];
	PixelSpace[2] = spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	if (flip[0] != 0) {
		for (int i = 0; i < width / 2; i++) {
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < imageNum; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + width - i - 1 + j * width + k * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[1] != 0) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height / 2; j++) {
				for (int k = 0; k < imageNum; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + i + (height - j - 1) * width + k * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}
	if (flip[2] != 0) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				for (int k = 0; k < imageNum / 2; k++) {
					short * p1 = data_m + i + j * width + k * width * height;
					short * p2 = data_m + i + j * width + (imageNum - k - 1) * width * height;
					short a = *p1;
					*p1 = *p2;
					*p2 = a;
				}
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace[0], PixelSpace[1], PixelSpace[2]);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimM[0] - 1, 0, dimM[1] - 1, 0, dimM[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(data_m);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_m;
}



inline void set_shortArray(short* data, short dat, int dim[3], int pos[3]) {
	int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	data[offset] = dat;
}
inline short At_shortArray(short* data, int dim[3], int pos[3]) {
	int offset = pos[0] + dim[0] * pos[1] + dim[0] * dim[1] * pos[2];
	return data[offset];
}
/** 将MHD文件根据本人的VolumeRender的边界格式来裁剪体数据
* filePath: 输入的.mhd文件路径
* outName: 输出的文件名
* 原始边界三轴都是 [-bound,bound]=[-0.5,0.5]，中心为[center]=[0]。
* 裁剪需要保证每个轴范围在 [center-bound, center+bound]在[-0.5,0.5]以内
*/
void RenderThread::MHDClip(const QString& filePath, const QString& outputDir, const QString& outName, double center[3], double bound[3]) {
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	int xRange[2] = { (int)((center[0] - bound[0] + 0.5) * width), (int)((center[0] + bound[0] + 0.5) * width) };
	int yRange[2] = { (int)((center[1] - bound[1] + 0.5) * height), (int)((center[1] + bound[1] + 0.5) * height) };
	int zRange[2] = { (int)((center[2] - bound[2] + 0.5) * imageNum), (int)((center[2] + bound[2] + 0.5) * imageNum) };
	int dimLength[3] = { xRange[1] - xRange[0], yRange[1] - yRange[0], zRange[1] - zRange[0] };
	short * data_aim = new short[dimLength[0] * dimLength[1] * dimLength[2]];
	for (int i = 0; i < dimLength[0]; i++) {
		for (int j = 0; j < dimLength[1]; j++) {
			for (int k = 0; k < dimLength[2]; k++) {
				int pos_aim[3] = { i,j,k };
				int pos_m[3] = { i + xRange[0],j + yRange[0],k + zRange[0] };
				short dat = At_shortArray(data_m, dimM, pos_m);
				set_shortArray(data_aim, dat, dimLength, pos_aim);
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_x, PixelSpace_y, PixelSpace_z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimLength[0] - 1, 0, dimLength[1] - 1, 0, dimLength[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();

	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;

}


/** 将MHD文件resize
* filePath: 输入的.mhd文件路径
* outName: 输出的文件名
* Interval: 降采样间隔，相当于分辨率降低Interval倍，只支持整数。
*/
void RenderThread::ResizeMHD_IntervalSampling(const QString& filePath, const QString& outputDir, const QString& outName, int Interval) {

	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	int dimLength[3] = { width / Interval, height / Interval, imageNum / Interval };
	short * data_aim = new short[dimLength[0] * dimLength[1] * dimLength[2]];
	for (int i = 0; i < dimLength[0]; i++) {
		for (int j = 0; j < dimLength[1]; j++) {
			for (int k = 0; k < dimLength[2]; k++) {
				int pos_aim[3] = { i,j,k };
				int pos_m[3] = { i * Interval, j * Interval, k * Interval };
				short dat = At_shortArray(data_m, dimM, pos_m);
				set_shortArray(data_aim, dat, dimLength, pos_aim);
			}
		}
	}

	vtkSmartPointer<vtkImageImport> imageImport = vtkSmartPointer<vtkImageImport>::New();
	imageImport->SetDataSpacing(PixelSpace_x, PixelSpace_y, PixelSpace_z);
	imageImport->SetDataOrigin(0, 0, 0);
	imageImport->SetWholeExtent(0, dimLength[0] - 1, 0, dimLength[1] - 1, 0, dimLength[2] - 1);
	imageImport->SetDataExtentToWholeExtent();
	imageImport->SetDataScalarTypeToShort();
	imageImport->SetNumberOfScalarComponents(1); // 通道
	imageImport->SetImportVoidPointer(data_aim);
	imageImport->Update();
	vtkSmartPointer<vtkMetaImageWriter> writer =
		vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetInputConnection(imageImport->GetOutputPort());
	writer->SetFileName((outputDir + "/" + outName + ".mhd").toStdString().c_str());
	writer->SetRAWFileName((outputDir + "/" + outName + ".raw").toStdString().c_str());
	writer->Write();
	emit PrintDataD("Write Finished! ", 0);
	delete[] data_aim;
	delete[] data_m;


}


/** 将MHD文件生成我的易读的体数据格式（二进制无压缩）
* filePath: 输入的.mhd文件路径
* outName: 输出的文件名
*/
#include <string>
#include <iostream>
#include <fstream>
void RenderThread::MHDGenerateFeimosData(const QString& filePath, const QString& outputDir, const QString& outName) {
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();

	reader->SetFileName(filePath.toStdString().c_str());
	reader->Update();
	vtkSmartPointer<vtkImageCast> ImageCast = vtkImageCast::New();
	ImageCast->SetInputData((vtkImageData*)reader->GetOutput());
	ImageCast->SetOutputScalarTypeToShort();
	ImageCast->Update();

	int * volumeData = ImageCast->GetOutput()->GetExtent();
	int width = volumeData[1] + 1;
	int height = volumeData[3] + 1;
	int imageNum = volumeData[5] + 1;
	int dimM[3] = { width, height, imageNum };
	double* spacing = ImageCast->GetOutput()->GetSpacing();
	double PixelSpace_x = (double)spacing[0];
	double PixelSpace_y = (double)spacing[1];
	double PixelSpace_z = (double)spacing[2];
	short * data_m = new short[width * height * imageNum];
	for (int i = 0; i < imageNum; i++) {
		memcpy(data_m + i * width * height, (short*)ImageCast->GetOutput()->GetScalarPointer() + i * width * height, width * height * sizeof(short));
	}

	std::ofstream fileInfo((outputDir + "/" + outName + ".Info" ).toStdString());
	{
		fileInfo << "X_Length " << width << std::endl;
		fileInfo << "Y_Length " << height << std::endl;
		fileInfo << "Z_Length " << imageNum << std::endl;
		fileInfo << "X_Space " << PixelSpace_x << std::endl;
		fileInfo << "Y_Space " << PixelSpace_y << std::endl;
		fileInfo << "Z_Space " << PixelSpace_z << std::endl;
	}
	fileInfo.close();

	std::ofstream file((outputDir + "/" + outName + ".data").toStdString(), std::ios::binary);
	for (int i = 0; i < imageNum; i++) {
		file.write((char*)((short *)data_m + i * width * height), sizeof(short) * width * height);
	}
	file.close();

	delete[] data_m;

}


/** 将MHD文件生成PBRT体数据格式（无压缩）
* filePath: 输入的.mhd文件路径
* outName: 输出的文件名
*/
void RenderThread::MHDGeneratePbrtVolume(const QString& filePath, const QString& outputDir, const QString& outName) {


	

}



// **********************************************//
// *** 一些简单的测试代码 *** //
// **********************************************//
void RenderThread::DCMTK_Test() {

	/*DcmFileFormat dfile;
	dfile.loadFile("Images/aa1.dcm");
	DcmMetaInfo *Metalnfo = dfile.getMetaInfo();
	DcmTag Tag;
	Tag = Metalnfo->getTag();
	Uint16 G_tag = Tag.getGTag();
	TextDinodonN("G_tag: ", G_tag);
	DcmDataset *data = dfile.getDataset();

	DcmElement *element = NULL;
	data->findAndGetElement(DCM_PixelData, element);

	double data_len = data->getLength();
	double element_len = element->getLength();
	TextDinodonN("data_len: ", data_len);
	TextDinodonN("element_len: ", element_len);


	OFString patientName;
	data->findAndGetOFString(DCM_PatientName, patientName);
	TextDinodonS("patientName: " + QString(patientName.data()));

	OFString patientId;
	data->findAndGetOFString(DCM_PatientID, patientId);
	TextDinodonS("patientId: " + QString(patientId.data()));

	OFString patientAge;
	data->findAndGetOFString(DCM_PatientAge, patientAge);
	TextDinodonS("patientAge: " + QString(patientAge.data()));

	OFString PatientPosition;
	data->findAndGetOFString(DCM_PatientPosition, PatientPosition);
	TextDinodonS("PatientPosition: " + QString(PatientPosition.data()));

	OFString ImagePositionPatient;
	data->findAndGetOFString(DCM_ImagePositionPatient, ImagePositionPatient);

	OFString PixelSpacing;
	data->findAndGetOFString(DCM_PixelSpacing, PixelSpacing);
	TextDinodonS("PixelSpacing: " + QString(PixelSpacing.data()));

	OFString ImageWidth;
	data->findAndGetOFString(DCM_Columns, ImageWidth);
	TextDinodonS("ImageWidth: " + QString(ImageWidth.data()));
	OFString ImageHeight;
	data->findAndGetOFString(DCM_Rows, ImageHeight);
	TextDinodonS("ImageHeight: " + QString(ImageHeight.data()));

	// 图像数据
	Uint16 *pixData16;
	element->getUint16Array(pixData16);*/

}
void RenderThread::GDCM_Test() {
	/*
	gdcm::Reader reader;
	reader.SetFileName("Images/IM-0001-0001.dcm");
	if (!reader.Read())
	{
		emit PrintDataD("(!reader.Read()) : ", 1);
		return;
	}
	else {
		emit PrintDataD("(success reader.Read()) : ", 0);
	}
	*/


}












