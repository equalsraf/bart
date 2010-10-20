#include "commandwidget.moc"

CommandWidget::CommandWidget(BartWidget *parent, const QString& cmd, bool refresh)
:QLabel(parent), cmd(cmd), refresh(refresh)
{
	connect(&process, SIGNAL(finished(int, QProcess::ExitStatus )),
			this, SLOT(finished(int , QProcess::ExitStatus )) );

	if ( refresh ) {
		connect(parent, SIGNAL(timeout()), this, SLOT(updateWidget()));
	}

	updateWidget();
}

void CommandWidget::updateWidget()
{
	if ( process.state() != QProcess::NotRunning ) {
		return;
	}

	QString app = cmd.section(" ", 0, 0);
	QStringList args = cmd.section(" ", 1).split(" ",  QString::SkipEmptyParts);

	process.start( app, args, QIODevice::ReadOnly);
}

void CommandWidget::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
	setText( process.readAll().trimmed() );

	update();
}
