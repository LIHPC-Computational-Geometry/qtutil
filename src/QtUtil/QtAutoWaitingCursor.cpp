#include "QtUtil/QtAutoWaitingCursor.h"
#include <iostream>
#include <iomanip>
using namespace std;

#include <QThread>
#include <QCursor>
#include <QApplication>


QtAutoWaitingCursor::QtAutoWaitingCursor (bool s)
	: _shown (false)
{
	if (true == s)
		show ( );
}	// QtAutoWaitingCursor::QtAutoWaitingCursor


QtAutoWaitingCursor::~QtAutoWaitingCursor ( )
{
	hide ( );
}	// QtAutoWaitingCursor::~QtAutoWaitingCursor


void QtAutoWaitingCursor::show ( )
{
	if (false == _shown)
	{
		if (QApplication::instance ( )->thread( ) == QThread::currentThread ( ))
		{
			QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
			_shown	= true;
		}	// if (QApplication::instance ( )->thread( ) == ..
	}	// if (false == _shown)
}	// QtAutoWaitingCursor::show


void QtAutoWaitingCursor::hide ( )
{
	if (true == _shown)
	{
		if (QApplication::instance ( )->thread( ) == QThread::currentThread ( ))
		{
			QApplication::restoreOverrideCursor ( );
			_shown	= false;
		}	// if (QApplication::instance ( )->thread( ) == ..
	}	// if (true == _shown)
}	// QtAutoWaitingCursor::hide


