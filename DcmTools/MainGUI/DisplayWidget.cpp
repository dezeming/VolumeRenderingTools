#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(QGroupBox * parent) {

	renderFlag = false;
	rThread = nullptr;

	setLayout(&displayWidgetLayout);
	displayWidgetLayout.addWidget(&m_IMAGraphicsView);

	framebuffer.InitBuffer(800, 600, 4);
}

DisplayWidget::~DisplayWidget() { 
	killRenderThread();
	framebuffer.FreeBuffer();
}

void DisplayWidget::closeEvent(QCloseEvent *event) {
	killRenderThread();
}

void DisplayWidget::startRenderThread() {
	if (!rThread) {
		rThread = new RenderThread;
		//renderFlag = true;
		rThread->renderFlag = true;
		rThread->p_framebuffer = &framebuffer;

		connect(rThread, SIGNAL(PrintString(const char*)), this, SLOT(PrintString(const char*)));
		connect(rThread, SIGNAL(PrintDataD(const char*, const double)), this, SLOT(PrintDataD(const char*, const double)));
		//connect(rThread, SIGNAL(PrintDataI(char* s, int data)), this, SLOT(PrintDataI(char* s, int data)));
		connect(rThread, SIGNAL(PaintBuffer(DcmTools::Uchar*, int, int, int)), &m_IMAGraphicsView, SLOT(PaintBuffer(DcmTools::Uchar*, int, int, int)));

		// Temporarily disable thread
		//rThread->start();
	}

	rThread->process();

	//renderFlag = true;
}

void DisplayWidget::killRenderThread() {
	if (!rThread)
		return;
	renderFlag = false;
	rThread->renderFlag = false;
	// Kill the render thread
	rThread->quit();
	// Wait for thread to end
	rThread->wait();
	// Remove the render thread
	delete rThread;

	rThread = nullptr;
}

void DisplayWidget::PrintString(const char* s) {
	TextDinodonS(s);
}
void DisplayWidget::PrintQString(const QString s) {
	TextDinodonS(s);
}
void DisplayWidget::PrintDataD(const char* s, const double data) {
	TextDinodonN(s, data);
}
/*void DisplayWidget::PrintDataF(char* s, float data) {
	TextDinodonN(s, data);
}
void DisplayWidget::PrintDataI(char* s, int data) {
	TextDinodonN(s, data);
}*/









