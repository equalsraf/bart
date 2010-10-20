#include <Qt/QtGui>
#include "bartwidget.h"
#include "clockwidget.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QFile f(QDir::home().absoluteFilePath(".bart/style"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream styleIn(&f);
		QString style = styleIn.readAll();
		f.close();

		app.setStyleSheet(style);
	}


	BartWidget w;
	w.show();

	return app.exec();
}
