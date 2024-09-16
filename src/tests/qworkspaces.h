#ifndef QT_WORKSPACES_H
#define QT_WORKSPACES_H

#include "QtUtil/QtAboutDialog.h"

#include <QtWidgets/QMainWindow>


class QtWorkspacesMainWindow : public QMainWindow
{
	Q_OBJECT

	public :

	QtWorkspacesMainWindow (QWidget* parent);


	protected slots :

	void dialogForEachWorkspaceCallback ( );
	void timeoutWarningDialogCallback ( );
	void timeoutErrorDialogCallback ( );
	void exitCallback ( );


	private :

};	// class QtWorkspacesMainWindow

#endif	// QT_WORKSPACES_H
