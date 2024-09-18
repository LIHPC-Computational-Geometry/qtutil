#include <qworkspaces.h>

#include <iostream>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QApplication>
#include <TkUtil/Exception.h>
#include <TkUtil/Process.h>
#include <QtUtil/QtMessageBox.h>

#include <unistd.h>

using namespace std;
using namespace TkUtil;


class QtWSAboutDialog : public QtAboutDialog
{
	public :
	
	QtWSAboutDialog (QWidget* parent, const TkUtil::UTF8String& title, const std::string& version,
	                 const std::string& url, const TkUtil::UTF8String& description, int workspace)
		: QtAboutDialog (parent, title, version, url, description), _workspace (workspace)
	{ }
	virtual ~QtWSAboutDialog ( )
	{ }
	
	
	protected :
	
	void showEvent (QShowEvent *event);
	
	
	private :
	
	int		_workspace;
};	// class QtWSAboutDialog


void QtWSAboutDialog::showEvent (QShowEvent* event)
{
	cout << "QtWSAboutDialog::showEvent called for workspace " << _workspace << endl;
	QtAboutDialog::showEvent (event);
}	// QtWSAboutDialog::showEvent


QtWorkspacesMainWindow::QtWorkspacesMainWindow (QWidget* parent)
	: QMainWindow (parent)
{
	QMenu*	menu	= new QMenu ("Application", this);
	menuBar ( )->addMenu (menu);
	menu->setToolTipsVisible (true);
	menu->addAction ("1 boite de dialogue au centre de chaque espaces de travail ...", this, SLOT (dialogForEachWorkspaceCallback ( )));
	menu->addAction ("1 boite de dialogue d'avertissement qui apparait dans 3 secondes ...", this, SLOT (timeoutWarningDialogCallback ( )));
	menu->addAction ("1 boite de dialogue d'erreur qui apparait dans 5 secondes ...", this, SLOT (timeoutErrorDialogCallback ( )));
	menu->addAction ("Quitter", this, SLOT (exitCallback ( )));
}	// QtWorkspacesMainWindow::QtAboutMainWindow


void QtWorkspacesMainWindow::dialogForEachWorkspaceCallback ( )
{
	try
	{
		QList<QScreen*>	screens	= QGuiApplication::screens ( );
		for (int i = 0; i < screens.count ( ); i++)
		{
			QScreen*	screen		= screens [i];
			const QRect	geometry	= screen->geometry ( );
			UTF8String	message;
			message << "Screen " << (long)i << " geometry : (" << (long)geometry.x ( ) << ", " << (long)geometry.y ( ) 
			        << ", " << (long)geometry.width ( ) << ", " << (long)geometry.height ( ) << ")";
			QtWSAboutDialog*	dialog	= new QtWSAboutDialog (0, "App. test", "1.0.0", "http://www.myapp.com", message, i);
			dialog->updateGeometry( );
			dialog->setGeometry (geometry.x ( ) + (geometry.width ( ) - dialog->width ( )) / 2, geometry.y ( ) + (geometry.height ( ) - dialog->height ( )) / 2,
			                     dialog->width ( ), dialog->height ( ));
			dialog->show ( );
		}	// for (int i = 0; i < screens.count ( ); i++)
	}
	catch (const Exception& exc)
	{
		cerr << "Exception : " << exc.getFullMessage ( ) << endl;
	}
	catch (const exception& e)
	{
		cerr << "Exception : " << e.what ( ) << endl;
	}
	catch (...)
	{
		cerr << "Exception non documentée." << endl;
	}
}	// QtWorkspacesMainWindow::dialogForEachWorkspaceCallback


void QtWorkspacesMainWindow::timeoutWarningDialogCallback ( )
{
	try
	{
		sleep (3);
		QtMessageBox::systemNotification (UTF8String ("Dialogs and Workspaces"), "", UTF8String ("Un message d'avertissement a été affiché."), QtMessageBox::URGENCY_NORMAL, 10000);
		QtMessageBox::displayWarningMessageInAppWorkspace (this, "Dialogs and Workspaces", "Ceci est un message d'avertissement");
//		QtWSAboutDialog*	dialog	= new QtWSAboutDialog (this, "App. test", "1.0.0", "http://www.myapp.com", "Boite de dialogue affichée après un délai de 5 secondes.", 0);
//		dialog->show ( );	// S'affiche dans le même bureau que l'application, sans changement de bureau
//dialog->exec ( );	// Change la boite de dialogue et l'application de bureau pour rejoindre celui qui a le focus.
	}
	catch (const Exception& exc)
	{
		cerr << "Exception : " << exc.getFullMessage ( ) << endl;
	}
	catch (const exception& e)
	{
		cerr << "Exception : " << e.what ( ) << endl;
	}
	catch (...)
	{
		cerr << "Exception non documentée." << endl;
	}
}	// QtWorkspacesMainWindow::timeoutWarningDialogCallback


void QtWorkspacesMainWindow::timeoutErrorDialogCallback ( )
{
	try
	{
		sleep (5);
		QtMessageBox::systemNotification (UTF8String ("Dialogs and Workspaces"), "", UTF8String ("Un message d'erreur a été affiché."), QtMessageBox::URGENCY_CRITICAL, 3000);
		QtMessageBox::displayErrorMessageInAppWorkspace (this, "Dialogs and Workspaces", "Ceci est un message d'erreur");
//		QtWSAboutDialog*	dialog	= new QtWSAboutDialog (this, "App. test", "1.0.0", "http://www.myapp.com", "Boite de dialogue affichée après un délai de 5 secondes.", 0);
//		dialog->show ( );	// S'affiche dans le même bureau que l'application, sans changement de bureau
//dialog->exec ( );	// Change la boite de dialogue et l'application de bureau pour rejoindre celui qui a le focus.
	}
	catch (const Exception& exc)
	{
		cerr << "Exception : " << exc.getFullMessage ( ) << endl;
	}
	catch (const exception& e)
	{
		cerr << "Exception : " << e.what ( ) << endl;
	}
	catch (...)
	{
		cerr << "Exception non documentée." << endl;
	}
}	// QtWorkspacesMainWindow::timeoutErrorDialogCallback


void QtWorkspacesMainWindow::exitCallback ( )
{
	switch (QtMessageBox::displayQuestionMessage (this, "QUITTER", "Souhaitez-vous réellement quitter l'application ?", 100, "OK", "Annuler", 0, 0))
	{
		case	0	:
			QApplication::exit (0);
			break;
	}
}	// QtWorkspacesMainWindow::exitCallback


int main (int argc, char* argv[], char* envp [])
{
	try
	{
		Process::initialize (argc, argv, envp);
		QApplication		app (argc, argv);
		QtWorkspacesMainWindow*	window	= new QtWorkspacesMainWindow (0);
		window->show ( );

		return app.exec ( );
	}
	catch (const Exception& exc)
	{
		cerr << "Erreur : " << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "Erreur non documentée." << endl;
		return -1;
	}

	return 0;
}	// main
