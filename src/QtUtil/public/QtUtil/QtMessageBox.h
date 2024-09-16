// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py

#ifndef QT_MESSAGE_BOX_H
#define QT_MESSAGE_BOX_H

#include <TkUtil/UTF8String.h>

#include <QWidget>
#include <QMessageBox>
#include <QScreen>



/**
 * Fonctions permettant d'afficher une boite de dialogue contenant un message.
 */
class QtMessageBox
{
	public :

	/**
	 * Boite de dialogue modale affichant un message d'information. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 */
	static void displayInformationMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'information. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la boite de dialogue.
	 */
	static int displayInformationMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant un message d'avertissement. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 */
	static void displayWarningMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue affichant un message d'avertissement. Formate le message avant affichage. Désactive l'éventuel parent le temps du message. Non modale,
	 * elle évite que la fenêtre parente ne passe dans le bureau virtuel courant et désorganise celui-ci.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @since		6.5.0
	 */
	static void displayWarningMessageInAppWorkspace (QWidget* parent,	const IN_UTIL UTF8String& title, const IN_UTIL UTF8String& message, size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'avertissement. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @param		Nombre de colonnes du message.
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la boite de dialogue.
	 */
	static int displayWarningMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant un message d'erreur. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la boite de dialogue.
	 */
	static void displayErrorMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue affichant un message d'avertissement. Formate le message avant affichage. Désactive l'éventuel parent le temps du message. Non modale,
	 * elle évite que la fenêtre parente ne passe dans le bureau virtuel courant et désorganise celui-ci.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @since		6.5.0
	 */
	static void displayErrorMessageInAppWorkspace (QWidget* parent, const IN_UTIL UTF8String& title, const IN_UTIL UTF8String& message, size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'erreur. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la boite de dialogue.
	 */
	static int displayErrorMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant une question. Formate le message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		 
	 *				boite de dialogue.
	 */
	static int displayQuestionMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	enum URGENCY_LEVEL { URGENCY_LOW, URGENCY_NORMAL, URGENCY_CRITICAL };
	
	/**
	 * Envoie la notification système transmise en argument. Repose sur <I>notify-send</I>. Attention, les caractères accentués semblent ne pas passer.
	 * @param	Titre de l'application
	 * @param	Message à afficher
	 * @param	Niveau d'urgence
	 * @param	Durée (en millisecondes) de la notification.
	 * @return	0 si la notification s'est bien passée, ou un code d'erreur.
	 * @since	6.5.0
	 */
	static int systemNotification (const IN_UTIL UTF8String& appTitle, const IN_UTIL UTF8String& message, URGENCY_LEVEL level = URGENCY_NORMAL, size_t duration = 5000);
	

	private :

	/** Constructeurs et opérateur = : interdits.
	 */
	QtMessageBox ( );
	QtMessageBox (const QtMessageBox&);
	QtMessageBox& operator = (const QtMessageBox&);

	/**
	 * Destructeur : interdit.
	 */
	~QtMessageBox ( );
};	// class QtMessageBox


#include <QMessageBox>
#include <QPushButton>

class QtMessageDialog : public QDialog
{
	Q_OBJECT

	public :

	QtMessageDialog (QMessageBox::Icon icon, QWidget* parent,
				const QString& title, const QString& text, int columns,
				const char* button1, const char* button2, const char* button3,
				int defaultButton = 0);

	virtual ~QtMessageDialog ( );
	
	// v 6.5.0 : possibilité de marquer la boite de dialogue comme en cours de traitement.
	virtual void setProcessing (bool processing);
	virtual bool isProcessing ( ) const
	{ return _processing; }
	

	protected slots :

	virtual void buttonClicked ( );


	private :

	QtMessageDialog (const QtMessageDialog&);
	QtMessageDialog& operator = (const QtMessageDialog&);

	QPushButton*			_buttons [3];
	bool					_processing;	// v 6.5.0
	bool					_parentState;	// v 6.5.0 le parent était-il actif ?
};	// class QtMessageDialog



#endif	// QT_MESSAGE_BOX_H
