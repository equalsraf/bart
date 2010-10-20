#ifndef __BART_CLOCKWIDGET__
#define __BART_CLOCKWIDGET__

#include <QLabel>
#include "bartwidget.h"

class ClockWidget: public QLabel
{
	Q_OBJECT
public:
	ClockWidget(BartWidget *parent=0);

public slots:
	void updateWidget();

};


#endif
