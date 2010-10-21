#include <Qt/QtGui>
#include "bartwidget.h"
#include "clockwidget.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	BartWidget w;
	w.show();

	return app.exec();
}
