#include "ClientView.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ClientView window;
	window.show();

	return app.exec();
}
