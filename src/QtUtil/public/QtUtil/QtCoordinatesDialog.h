#ifndef QT_COORDINATES_DIALOG_H
#define QT_COORDINATES_DIALOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <QtUtil/QtTextField.h>
#include <QtUtil/QtDlgClosurePanel.h>

#include <QDialog>
#include <QCheckBox>


/** Classe de boite de dialogue modale de saisie de coordonnées 3D.
 */
class QtCoordinatesDialog : public QDialog
{
	public	:

	/**
	 * Constructeur. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle aide contextuelle.
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		titre de la première composante.
	 * @param		titre de la seconde composante.
	 * @param		titre de la troisème composante.
	 * @param		valeurs par défaut des composantes.
	 * @param		True si les cases à cocher "active" doivent être activées, false dans le cas contraire.
	 * @param		true si la composante est active, false dans le cas contraire.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 * @see			createGui
	 */
	QtCoordinatesDialog (QWidget* parent, const IN_UTIL UTF8String& title, 
					const IN_UTIL UTF8String& xLabel, const IN_UTIL UTF8String& yLabel, const IN_UTIL UTF8String& zLabel,
					double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
					const IN_STD string& helpURL = "", const IN_STD string& helpTag = "");

	/**
	 * Constructeur. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle aide contextuelle.
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		titre de la première composante.
	 * @param		titre de la seconde composante.
	 * @param		titre de la troisème composante.
	 * @param		valeurs par défaut des composantes.
	 * @param		True si les cases à cocher "active" doivent être activées, false dans le cas contraire.
	 * @param		true si la composante est active, false dans le cas contraire.
	 * @param		Valeurs min et max des 3 composantes. Utilisé pour affichage informatif lorsque min <= max.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 * @see			createGui
	 * @since		6.3.0
	 */
	QtCoordinatesDialog (QWidget* parent, const IN_UTIL UTF8String& title, 
					const IN_UTIL UTF8String& xLabel, const IN_UTIL UTF8String& yLabel, const IN_UTIL UTF8String& zLabel,
					double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
					double xmin, double xmax, double ymin, double ymax, double zmin, double zmax,
					const IN_STD string& helpURL = "", const IN_STD string& helpTag = "");

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtCoordinatesDialog ( );

	/**
	 * @return		Les composantes x, y, z
	 * @param		Lève une exception si une des trois composantes est
	 *				incorrecte.
	 */
	virtual void getCoordinates (double& x, double& y, double& z) const;

	/**
	 * @return		true si la composante est activée, sinon false
	 */
	virtual void getActivations (bool& x, bool& y, bool& z) const;

	/**
	 * Demande une confirmation si aucune case à cocher "Modifier" n'est
	 * cochée.
	 */
	virtual void accept ( );


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtCoordinatesDialog (const QtCoordinatesDialog&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtCoordinatesDialog& operator = (const QtCoordinatesDialog&);
	
	/**
	 * Création de l'IHM. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle aide contextuelle.
	 * @param		Titre de la fenêtre
	 * @param		titre de la première composante.
	 * @param		titre de la seconde composante.
	 * @param		titre de la troisème composante.
	 * @param		valeurs par défaut des composantes.
	 * @param		True si les cases à cocher "active" doivent être activées, false dans le cas contraire.
	 * @param		true si la composante est active, false dans le cas contraire.
	 * @param		Valeurs min et max des 3 composantes. Utilisé pour affichage informatif lorsque min <= max.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 * @since		6.3.0
	 */
	void createGui (const IN_UTIL UTF8String& title, 
					const IN_UTIL UTF8String& xLabel, const IN_UTIL UTF8String& yLabel, const IN_UTIL UTF8String& zLabel,
					double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
					double xmin, double xmax, double ymin, double ymax, double zmin, double zmax,
					const IN_STD string& helpURL = "", const IN_STD string& helpTag = "");

	/**
	 * La saisie des composantes.
	 */
	QtTextField			*_xTextField, *_yTextField, *_zTextField;

	/**
	 * Les cases à cocher "Modifier". 
	 */
	QCheckBox			*_modifyXCheckBox, *_modifyYCheckBox, *_modifyZCheckBox;

	/**
	 * Le panneau contenant les boutons "OK" / "Annuler".
	 */
	QtDlgClosurePanel*	_closurePanel;
};	// class QtCoordinatesDialog

#endif	// QT_COORDINATES_DIALOG_H
