#include "batterywidget.moc"


BatteryWidget::BatteryWidget(BartWidget *parent)
:QWidget(parent), design(0), full(0), now(0), current(-1)
{
	setupUi(this);

	connect(parent, SIGNAL(timeout()),
			this, SLOT(updateWidget()));

	label->setText("Battery:");
	content->setText("??");
}

void BatteryWidget::parse()
{

	QFile f("/sys/class/power_supply/BAT0/uevent");

	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QStringList args;
	QByteArray line;
	do {
		line = f.readLine();
		args = QString(line).split('=');

		if ( args.length() != 2 ) {
			continue;
		}
		
		if ( args[0] == "POWER_SUPPLY_ENERGY_FULL_DESIGN" ) {
			design = args[1].toInt();
		} else if ( args[0] == "POWER_SUPPLY_ENERGY_FULL" ) {
			full = args[1].toInt();
		} else if ( args[0] == "POWER_SUPPLY_ENERGY_NOW" ) {
			now = args[1].toInt();
		}
	} while (!line.isEmpty());

	current = now*1.0 / full * 100;
	lost = 100 - full*1.0 /design*100;
	
}

void BatteryWidget::updateWidget()
{
	parse();
	content->setText( QString::number( (int)current) + "%" );
	update();
}


