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

private slots:
	void fromConfig();
	void loadStyle();
	void configChanged(const QString &path);

private:

	void defaultConfig();
	static QString configPath();
	static QString stylePath();
	static QString configDirPath();

	QLayout *layout;
	QTimer *m_timer;
	QFileSystemWatcher fsWatcher;

};


#endif
