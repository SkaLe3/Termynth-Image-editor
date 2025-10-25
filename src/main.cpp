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

	return app.exec();
}