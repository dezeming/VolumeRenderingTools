#include "MainWindow.h"
#include "DebugText.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
	setMinimumSize(800, 800);

	setCentralWidget(&centralWidget);

	MainWindowLayout.setAlignment(Qt::AlignCenter);

	centralWidget.setLayout(&MainWindowLayout);

	setMenu();

	setWidget();

	setDock();

	connect(m_InteractionDockWidget.renderButton, SIGNAL(clicked()), this, SLOT(setRendering()));

	
	/*TextDinodonS("Let's start !");

	DcmTools::Vector4f vec(2.0, 1.0, 3.0, 2.0);
	TextDinVector("vec", vec);
	vec / 0;

	DcmTools::Point4f pot(2.0, 1.0, 3.0, 2.0);
	TextDinPoint("pot", pot);
	pot / 0;

	DcmTools::Normal n(1.0, 1.0, 1.0);
	n = DcmTools::normalize(n);
	TextDinNormal("n", n);

	TextDinodonS("Let's end !");*/
	
	
	/*TextDinodonS("Matrix Test start !");

	DcmTools::Vector4f vec1(1.0, 2.0, 3.0, 1.0);
	DcmTools::Vector4f vec2(2.0, 1.0, 1.0, 3.0);
	DcmTools::Vector4f vec3(1.0, 3.0, 1.0, 2.0);
	DcmTools::Vector4f vec4(4.0, 1.0, 2.0, 3.0);
	DcmTools::Matrix4x4 m(vec1, vec2, vec3, vec4);
	DcmTools::Matrix4x4 m_ivt;
	bool success = m.invert(m_ivt);
	if (!success) TextDinodonS("EError Unable to invert singular matrix");
	else TextDinMatrix("m_ivt", m_ivt);

	DcmTools::Matrix4x4 I = m * m_ivt;
	TextDinMatrix("I", I);

	TextDinodonS("Matrix Test end !");*/


}

void MainWindow::closeEvent(QCloseEvent *event) {
	// 关闭渲染线程
	m_DisplayWidget.killRenderThread();

}

void MainWindow::setMenu(void) {


}

void MainWindow::setWidget(void) {
	
	MainWindowLayout.addWidget(&m_DisplayWidget);

}

void MainWindow::setDock(void) {

	addDockWidget(Qt::LeftDockWidgetArea, &m_InteractionDockWidget);

}

void MainWindow::setRendering() {

	if (!m_DisplayWidget.renderFlag) {
		// 启动渲染
		m_InteractionDockWidget.renderButton->setText("Processing");
		m_DisplayWidget.startRenderThread();
	}

}







