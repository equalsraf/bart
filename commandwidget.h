#ifndef __BART_COMMANDWIDGET__
#define __BART_COMMANDWIDGET__

#include <QLabel>
#include <QProcess>

#include "bartwidget.h"

class CommandWidget: public QLabel
{
	Q_OBJECT
public:
	CommandWidget(BartWidget *, const QString& cmd, bool refresh=false);

public slots:
	void updateWidget();

private slots:
	void finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	QString cmd;
	bool refresh;
	QProcess process;

};


#endif
