#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget
{
	Q_OBJECT

	public:
	Window();

	protected slots :
	virtual void undoCallback ( );
	virtual void redoCallback ( );
	virtual void dashDotDotLineCallback ( );
	virtual void dashDotLineCallback ( );
	virtual void dashLineCallback ( );
	virtual void dotLineCallback ( );
	virtual void paletteCallback ( );
	virtual void printCallback ( );
	virtual void zoomInCallback ( );
	virtual void zoomOutCallback ( );
};

#endif	// WINDOW_H
