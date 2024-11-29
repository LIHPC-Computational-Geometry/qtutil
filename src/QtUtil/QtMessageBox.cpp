#include "QtUtil/QtMessageBox.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <TkUtil/Exception.h>
#include <TkUtil/Process.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <assert.h>

#include <QCursor>
#include <QApplication>
#ifdef QT_5
#include <QDesktopWidget>
#else	// QT_5
#include <QGuiApplication>
#include <QScreen>
#endif	// QT_5
#include <QMessageBox>


USING_UTIL
USING_STD

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


#define BEGIN_TRY_CATCH_BLOCK                                              \
	bool	hasError	= false;                                           \
	try {

#define COMPLETE_TRY_CATCH_BLOCK                                           \
	}                                                                      \
	catch (const IN_UTIL Exception& exc)                                   \
	{                                                                      \
		hasError	= true;                                                \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			UTF8TOQSTRING(exc.getFullMessage( )),\
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}                                                                      \
	catch (const IN_STD exception& exc)                                    \
	{                                                                      \
		hasError	= true;                                                \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			QSTR (exc.what ( )),                                   \
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}                                                                      \
	catch (...)                                                            \
	{                                                                      \
		hasError	= true;                                                \
		QMessageBox::critical (0,                                          \
		             "ERREUR APPLICATIVE LORS DE L'AFFICHAGE D'UN MESSAGE",\
			QSTR ("Erreur non documentée"),                                \
			QMessageBox::Ok, QMessageBox::NoButton);                       \
	}


// ===========================================================================
//                          LA CLASSE QtMessageDialog
// ===========================================================================


#include "QtUtil/QtConfiguration.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>


QtMessageDialog::QtMessageDialog (QMessageBox::Icon icon, QWidget* parent, const QString& title, const QString& text, int columns, const char* button1, const char* button2, const char* button3, int defaultButton)
	: QDialog (parent), _processing (false), _parentState (true)
{
	setWindowTitle (title);

	QVBoxLayout*	mainLayout	= new QVBoxLayout (this);
	mainLayout->setSpacing (QtConfiguration::spacing);
#ifdef QT_5
	mainLayout->setMargin (QtConfiguration::margin);
#else	// => Qt6
	mainLayout->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5

	QHBoxLayout*	hboxLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hboxLayout);
	QLabel*			iconLabel	= new QLabel (this);
	iconLabel->setPixmap (QMessageBox::standardIcon (icon));
	hboxLayout->addWidget (iconLabel);
	QString	formattedText	= UTF8TOQSTRING (QtUnicodeHelper::qstringToUTF8String (text).format (columns));
	QTextEdit*	textEdit	= new QTextEdit (this);
	textEdit->setReadOnly (true);
	textEdit->setLineWrapMode (QTextEdit::NoWrap);
	textEdit->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	textEdit->setFrameStyle (QTextEdit::NoFrame);
	textEdit->setText (formattedText);
	textEdit->setMinimumSize (textEdit->sizeHint ( ));
	textEdit->setAutoFillBackground (true);
	textEdit->viewport ( )->setBackgroundRole (QPalette::NoRole);
	hboxLayout->addWidget (textEdit);

	hboxLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hboxLayout);
	_buttons [0]	= 0 == button1 ? 0 : new QPushButton (button1, this);
	_buttons [1]	= 0 == button2 ? 0 : new QPushButton (button2, this);
	_buttons [2]	= 0 == button3 ? 0 : new QPushButton (button3, this);
	hboxLayout->addStretch (200);
	for (int i = 0; i < 3; i++)
		if (0 != _buttons [i])
		{
			hboxLayout->addWidget (_buttons [i]);
			connect (_buttons [i], SIGNAL(clicked ( )), this, SLOT (buttonClicked ( )));
			_buttons [i]->setDefault (defaultButton == i ? true : false);
			_buttons [i]->setAutoDefault (defaultButton == i ? true : false);
		}
	hboxLayout->addStretch (200);

	setLayout (mainLayout);
	mainLayout->activate ( );
	// Qt considère qu'on est dans une fenêtre scrollée et donc que la largeur
	// de cette fenêtre importe peu.
	// On force donc ici à prendre la largeur idéale du document :
	int	width	= (int)textEdit->document ( )->idealWidth ( );
	width	+= 2 * QtConfiguration::margin;
	int	height	= (int)textEdit->document ( )->size ( ).height ( );
	height	+= 2 * QtConfiguration::margin;
	// Les dimensions calculées sont elles acceptables ?
#ifdef QT_5
	QDesktopWidget*	desktop	= QApplication::desktop ( );
	if (0 != desktop)
	{
		const QRect	screenSize	= desktop->availableGeometry (QCursor::pos ( ));
		if (2 * width > screenSize.width ( ))
			width	= screenSize.width ( ) / 2;
		if (2 * height > screenSize.height ( ))
			height	= screenSize.height ( ) / 2;
	}	// if (0 != desktop)
#else	// QT_5
	QScreen*	screen	= QGuiApplication::primaryScreen ( );
	if (0 != screen)
	{
		const QRect	screenSize	= screen->virtualGeometry ( );
		if (2 * width > screenSize.width ( ))
			width	= screenSize.width ( ) / 2;
		if (2 * height > screenSize.height ( ))
			height	= screenSize.height ( ) / 2;
	}	// if (0 != screen)
#endif	// QT_5
	textEdit->setMinimumSize (QSize (width, height));
	textEdit->setFixedSize (QSize (width, height));
	setMinimumSize (sizeHint ( ));
}	// QtMessageDialog::QtMessageDialog


QtMessageDialog::QtMessageDialog (const QtMessageDialog&)
{
	assert (0 && "QtMessageDialog copy constructor is not allowed.");
}	// QtMessageDialog::QtMessageDialog


QtMessageDialog& QtMessageDialog::operator = (const QtMessageDialog&)
{
	assert (0 && "QtMessageDialog operator = is not allowed.");
	return *this;
}	// QtMessageDialog::operator =


QtMessageDialog::~QtMessageDialog ( )
{
}	// QtMessageDialog::~QtMessageDialog


void QtMessageDialog::setProcessing (bool processing)	// v 6.5.0
{
	if (processing != _processing)
	{
		_processing	= processing;
		if (0 != parentWidget ( ))
		{
			if (true == processing)
			{
				_parentState	= parentWidget ( )->isEnabled ( );
				parentWidget ( )->setEnabled (false);
				// L'inactivation du parent entraîne celui de la boite de dialogue, donc on la réactive :
				setEnabled (true);
			}
			else if (true == _parentState)
				parentWidget ( )->setEnabled (_parentState);
		}
	}	// if (processing != _processing)
}	// QtMessageDialog::setProcessing


void QtMessageDialog::buttonClicked ( )
{
	const QObject*		s	= sender ( );
	for (int i = 0; i < 3; i++)
		if (s == _buttons [i])
		{
			setProcessing (false);	// v 6.5.0
			done (i);
			return;
		}	// if (s == _buttons [i])
}	// QtMessageDialog::buttonClicked


// ===========================================================================
//                           LA CLASSE QtMessageBox
// ===========================================================================

QtMessageBox::QtMessageBox ( )
{
	assert (0 && "QtMessageBox::QtMessageBox is not allowed.");
}	// QtMessageBox::QtMessageBox


QtMessageBox::QtMessageBox (const QtMessageBox&)
{
	assert (0 && "QtMessageBox::QtMessageBox is not allowed.");
}	// QtMessageBox::QtMessageBox


QtMessageBox& QtMessageBox::operator = (const QtMessageBox&)
{
	assert (0 && "QtMessageBox::operator = is not allowed.");
	return *this;
}	// QtMessageBox::operator =


QtMessageBox::~QtMessageBox ( )
{
	assert (0 && "QtMessageBox::~QtMessageBox is not allowed.");
}	// QtMessageBox::~QtMessageBox


void QtMessageBox::displayInformationMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum)
{
	displayInformationMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayInformationMessage


int QtMessageBox::displayInformationMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum, const char* button0Text, const char* button1Text, const char* button2Text, int defaultButtonNumber)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Information, parent, UTF8TOQSTRING (title), UTF8TOQSTRING (message), columnNum, button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );
	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayInformationMessage


void QtMessageBox::displayWarningMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum)
{
	displayWarningMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayWarningMessage


void QtMessageBox::displayWarningMessageInAppWorkspace (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum)	// v 6.5.0
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));
	const bool	security	= 0 == parent ? true : parent->isEnabled ( );

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Warning, parent, UTF8TOQSTRING (title),UTF8TOQSTRING (message), columnNum, "OK", 0, 0);
	dialog.setProcessing (true);
	dialog.show ( );
	while (true == dialog.isProcessing ( ))
		QApplication::processEvents (QEventLoop::AllEvents, 1000);

	COMPLETE_TRY_CATCH_BLOCK
	
	if ((true == hasError) && (0 != parent) && (true == security))
		parent->setEnabled (true);

	QApplication::restoreOverrideCursor ( );
}	// QtMessageBox::displayWarningMessageInAppWorkspace


int QtMessageBox::displayWarningMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum, const char* button0Text, const char* button1Text, const char* button2Text, int defaultButtonNumber)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Warning, parent, UTF8TOQSTRING (title), UTF8TOQSTRING (message), columnNum, button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayWarningMessage


void QtMessageBox::displayErrorMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum)
{
	displayErrorMessage (parent, title, message, columnNum, "OK");
}	// QtMessageBox::displayErrorMessage


void QtMessageBox::displayErrorMessageInAppWorkspace (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum)	// v 6.5.0
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));
	const bool	security	= 0 == parent ? true : parent->isEnabled ( );
	
	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Critical, parent, UTF8TOQSTRING (title), UTF8TOQSTRING (message), columnNum, "OK", 0, 0);
	dialog.setProcessing (true);
	dialog.show ( );
	while (true == dialog.isProcessing ( ))
		QApplication::processEvents (QEventLoop::AllEvents, 1000);

	COMPLETE_TRY_CATCH_BLOCK

	if ((true == hasError) && (0 != parent) && (true == security))
		parent->setEnabled (true);
	
	QApplication::restoreOverrideCursor ( );
}	// QtMessageBox::displayErrorMessageInAppWorkspace


int QtMessageBox::displayErrorMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum, const char* button0Text, const char* button1Text, const char* button2Text, int defaultButtonNumber)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Critical, parent, UTF8TOQSTRING (title),UTF8TOQSTRING (message), columnNum, button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayErrorMessage


int QtMessageBox::displayQuestionMessage (QWidget* parent, const UTF8String& title, const UTF8String& message, size_t columnNum,const char* button0Text, const char* button1Text, const char* button2Text, int defaultButtonNumber)
{
	QApplication::setOverrideCursor (QCursor (Qt::ArrowCursor));

	int	button	= -1;

	BEGIN_TRY_CATCH_BLOCK

	QtMessageDialog	dialog (QMessageBox::Question, parent, UTF8TOQSTRING (title), UTF8TOQSTRING (message), columnNum, button0Text, button1Text, button2Text, defaultButtonNumber);
	button	= (QMessageBox::StandardButton)dialog.exec ( );

	COMPLETE_TRY_CATCH_BLOCK

	QApplication::restoreOverrideCursor ( );
	
	return button;
}	// QtMessageBox::displayQuestionMessage


int QtMessageBox::systemNotification (const UTF8String& appTitle, const string& appIconFile, const UTF8String& message, URGENCY_LEVEL level, size_t duration)	// v 6.6.0
{
	if (0 == duration)
		return 0;

	static bool	available	= true;
	if (false == available)
		return -1;

	unique_ptr<Process>	notifySend (new Process ("notify-send"));
	
	BEGIN_TRY_CATCH_BLOCK
	
	notifySend->getOptions ( ).addOption ("-u");
	switch (level)
	{
		case QtMessageBox::URGENCY_LOW		: notifySend->getOptions ( ).addOption ("low");		break;
		case QtMessageBox::URGENCY_CRITICAL	: notifySend->getOptions ( ).addOption ("critical");break;
		default								:  notifySend->getOptions ( ).addOption ("normal");
	}	// switch (level)
	
	char	time [1024];
	sprintf (time, "%lu", duration);
	notifySend->getOptions ( ).addOption ("-t");
	notifySend->getOptions ( ).addOption (time);
	if (false == appTitle.empty ( ))
	{
		notifySend->getOptions ( ).addOption ("-a");
		notifySend->getOptions ( ).addOption (appTitle.utf8 ( ));
	}	// if (false == appTitle.empty ( ))
	if (false == appIconFile.empty ( ))
	{
		notifySend->getOptions ( ).addOption ("-i");
		notifySend->getOptions ( ).addOption (appIconFile);
	}	// if (false == appIconFile.empty ( ))
	notifySend->getOptions ( ).addOption (message.utf8 ( ));
	notifySend->execute (false);
	notifySend->wait ( );
	available	= 0 == notifySend->getCompletionCode ( ) ? true : false;
	
	COMPLETE_TRY_CATCH_BLOCK
	
	return notifySend->getCompletionCode ( );
}	// QtMessageBox::systemNotification


// ===========================================================================
//                    LA CLASSE ActionCompletionNotifier
// ===========================================================================

ActionCompletionNotifier::ActionCompletionNotifier (
	const UTF8String& appTitle, const string& appIconFile, const UTF8String& message, QtMessageBox::URGENCY_LEVEL level, size_t duration, size_t minimumTimeLapse)
	: _timer ( ), _appTitle (appTitle), _message (message), _appIconFile (appIconFile), _urgencyLevel (level), _duration (duration), _minimumTimeLapse (minimumTimeLapse)
{
	if (0 != _minimumTimeLapse)
		_timer.start ( );
}


ActionCompletionNotifier::ActionCompletionNotifier (const ActionCompletionNotifier&)
{
	assert (0 && "ActionCompletionNotifier copy constructor is not allowed.");
}	// ActionCompletionNotifier::ActionCompletionNotifier


ActionCompletionNotifier& ActionCompletionNotifier::operator = (const ActionCompletionNotifier&)
{
	assert (0 && "ActionCompletionNotifier assignment operator is not allowed.");
	return *this;
}	// ActionCompletionNotifier::operator =


ActionCompletionNotifier::~ActionCompletionNotifier ( )
{
	if (0 != _minimumTimeLapse)
		_timer.stop ( );
	
	if ((0 == _minimumTimeLapse) || (_timer.duration ( ) >= _minimumTimeLapse))
		QtMessageBox::systemNotification (_appTitle, _appIconFile, _message, _urgencyLevel, _duration);
}	// ActionCompletionNotifier::~ActionCompletionNotifier
