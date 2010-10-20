#ifndef __BART_BATTERYWIDGET__
#define __BART_BATTERYWIDGET__

#include "bartwidget.h"
#include "ui_simplewidget.h"

class BatteryWidget: public QWidget, private Ui::SimpleWidget
{
	Q_OBJECT
public:
	BatteryWidget(BartWidget *parent=0);

public slots:
	void updateWidget();
private:
	void parse();

	int design, full, now;
	float current, lost;
};

#endif
