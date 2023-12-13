#ifndef QT_ABOUT_MAIN_WINDOW_H
#define QT_ABOUT_MAIN_WINDOW_H

#include "QtUtil/QtHelpWindow.h"
#include "QtUtil/QtLogsView.h"
#include "QtUtil/QtStatusLogOutputStream.h"
#include "QtUtil/QtStringListSelectionPanel.h"
#include "QtUtil/QtWebBrowser.h"

#include <TkUtil/LogDispatcher.h>

#include <QDialog>
#include <QMainWindow>


class QtAboutMainWindow : public QMainWindow
{
	Q_OBJECT

	public :

	QtAboutMainWindow (QWidget* parent);
	virtual void log (const TkUtil::Log& log);
	static std::string	homeURL, indexFile;
	static std::string	webPage, webTag;


	protected slots :

	void webCallback ( );
	void helpCallback ( );
	void sessionWebBrowserCallback ( );
	void coordinatesCallback ( );
	void rotationCallback ( );
	void transformationMatrixCallback ( );
	void minMaxCallback ( );
	void scientificTextCallback ( );
	void warningCallback ( );
	void errorCallback ( );
	void stringListCallback ( );
	void aboutCallback ( );
	void exitCallback ( );

	private :

	QtWebBrowser*				_webBrowser;
	QtLogsView*					_logView;
	QtStatusLogOutputStream*	_statusLogOutputStream;
	TkUtil::LogDispatcher		_logDispatcher;
};	// class QtAboutMainWindow


class QStringListDialog : public QDialog
{
	public :

	QStringListDialog (QWidget* parent,
	                   const IN_STD vector <IN_STD string>& unselected,
	                   const IN_STD vector <IN_STD string>& selected);
	virtual ~QStringListDialog ( );

	virtual IN_STD vector <IN_STD string> getSelection ( ) const;


	private :

	QStringListDialog (const QStringListDialog&);
	QStringListDialog& operator = (const QStringListDialog&);

	QtStringListSelectionPanel*	_stringListPanel;
};	// class QStringListDialog


#endif	// QT_ABOUT_MAIN_WINDOW_H
