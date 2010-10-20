#ifndef __BART_WIDGET__
#define __BART_WIDGET__

#include <QtGui>

class BartWidget: public QWidget
{
	Q_OBJECT
public:
	BartWidget(QWidget *parent=0);
	void addWidget(QWidget *);

signals:
	void timeout();

private:
	void fromConfig();
	void defaultConfig();

	QLayout *layout;
	QTimer *m_timer;

};


#endif
