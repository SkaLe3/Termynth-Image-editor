#include "MainWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	app.setApplicationName("Termynth Image Editor");
	app.setApplicationVersion("1.0");
	app.setOrganizationName("SkaLe Studios");

	MainWindow window;
	window.show();

	app.setStyleSheet(R"(
	QMainWindow, QDockWidget, QWidget {
	    background-color: #4b4b4b;
	    color: white;
	}
	QDockWidget::title {
	    background-color: #353535;
	    padding: 4px;
	}
	)");

	return app.exec();
}