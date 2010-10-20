#include "bartwidget.moc"

#include "clockwidget.h"
#include "i3pagerwidget.h"
#include "notifywidget.h"
#include "commandwidget.h"
#include "batterywidget.h"

BartWidget::BartWidget(QWidget *parent)
:QWidget(parent)
{
	setAttribute(Qt::WA_X11NetWmWindowTypeDock);
	setMaximumHeight(16);
	setFixedHeight(16);

	QHBoxLayout *l = new QHBoxLayout(this);
	l->setContentsMargins(0, 0, 0, 0);
	l->setSpacing(3);
	this->layout = l;

	this->m_timer = new QTimer(this);
	this->m_timer->start(10000);

	connect( this->m_timer, SIGNAL(timeout()),
	      this, SIGNAL(timeout()) );

	fromConfig();
}

void BartWidget::defaultConfig()
{
	qDebug() << "Loading default configuration";
	addWidget(new CommandWidget(this, "/bin/hostname"));
	addWidget(new ClockWidget(this));
}

void BartWidget::fromConfig()
{
	
	QFile f(QDir::home().absoluteFilePath(".bart/config"));

	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		defaultConfig();
		return;
	}

	QByteArray line;
	do {
		line = f.readLine();
		QStringList widget = QString(line).trimmed().split(' ');

		if ( widget.startsWith("#") ) {
			continue;
		}

		switch(widget.length()) {
		case 1:
			if ( widget[0] == "\%i3pager" ) {
				addWidget(new I3PagerWidget(this));
			} else if ( widget[0] == "\%notify" ) {
				addWidget( new NotifyWidget(this) );
			} else if ( widget[0] == "\%battery" ) {
				addWidget( new BatteryWidget(this) );
			} else if ( widget[0] == "\%clock" ) {
				addWidget(new ClockWidget(this));
			}
			break;
		case 2:
			if ( widget[0] == "\%cmd" ) {
				addWidget(new CommandWidget(this, widget[1]));
			}
		}

	} while (!line.isEmpty());
}


void BartWidget::addWidget(QWidget *w)
{
	layout->addWidget( w );
}


