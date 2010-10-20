#include "clockwidget.moc"

#include <QDateTime>
#include <QFont>
#include <QTimer>
#include <QtGui>

ClockWidget::ClockWidget(BartWidget* parent)
:QLabel(parent)
{
	setScaledContents(true);

	setMaximumHeight(16);
	setFixedHeight(16);
	updateWidget();

	connect(parent, SIGNAL(timeout()), this, SLOT(updateWidget()));
}

void ClockWidget::updateWidget()
{
	setText(QDateTime::currentDateTime().toString("hh:mm dd/MM/yyyy"));
	update();
}

