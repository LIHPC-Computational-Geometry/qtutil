#ifndef QT_ENCODING_MAIN_WINDOW_H
#define QT_ENCODING_MAIN_WINDOW_H

#include <QMainWindow>

class QtEncodingMainWindow : public QMainWindow
{
	Q_OBJECT

	public :

	QtEncodingMainWindow (QWidget* parent);
	virtual ~QtEncodingMainWindow ( );


	private slots :

	virtual void exitCallback ( );


	private :


	QtEncodingMainWindow& operator = (const QtEncodingMainWindow&);
};	// class QtEncodingMainWindow


#endif	// QT_ENCODING_MAIN_WINDOW_H
