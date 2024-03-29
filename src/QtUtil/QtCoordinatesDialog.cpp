#include "QtUtil/QtCoordinatesDialog.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/QtGroupBox.h"
#include "QtUtil/QtMessageBox.h"

#include <TkUtil/Exception.h>
#include <TkUtil/NumericServices.h>

#include <QValidator>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>

#include <assert.h>



USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtCoordinatesDialog::QtCoordinatesDialog (
			QWidget* parent, const UTF8String& title, const UTF8String& xLabel, const UTF8String& yLabel, const UTF8String& zLabel,
			double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
			const string& helpURL, const string& helpTag)
	: QDialog (parent, QtConfiguration::modalDialogWFlags),
	  _xTextField (0), _yTextField (0), _zTextField (0), _modifyXCheckBox (0), _modifyYCheckBox (0), _modifyZCheckBox (0), _closurePanel (0)
{
//	createGui (title, xLabel, yLabel, zLabel, x, y, z, 1., -1., 1., -1., 1., -1., checkboxesEnabled, xEnabled, yEnabled, zEnabled, helpURL, helpTag);	// v 6.3.0
	createGui (title, xLabel, yLabel, zLabel, x, y, z, checkboxesEnabled, xEnabled, yEnabled, zEnabled, -NumericServices::doubleMachMax ( ), NumericServices::doubleMachMax ( ), -NumericServices::doubleMachMax ( ), NumericServices::doubleMachMax ( ), -NumericServices::doubleMachMax ( ), NumericServices::doubleMachMax ( ), helpURL, helpTag);
}	// QtCoordinatesDialog::QtCoordinatesDialog


QtCoordinatesDialog::QtCoordinatesDialog (	// v 6.3.0
			QWidget* parent, const UTF8String& title, const UTF8String& xLabel, const UTF8String& yLabel, const UTF8String& zLabel,
			double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
			double xmin, double xmax, double ymin, double ymax, double zmin, double zmax,
			const string& helpURL, const string& helpTag)
	: QDialog (parent, QtConfiguration::modalDialogWFlags),
	  _xTextField (0), _yTextField (0), _zTextField (0), _modifyXCheckBox (0), _modifyYCheckBox (0), _modifyZCheckBox (0), _closurePanel (0)
{
	createGui (title, xLabel, yLabel, zLabel, x, y, z, checkboxesEnabled, xEnabled, yEnabled, zEnabled, xmin, xmax, ymin, ymax, zmin, zmax, helpURL, helpTag);
}	// QtCoordinatesDialog::QtCoordinatesDialog


void QtCoordinatesDialog::createGui (const UTF8String& title, const UTF8String& xLabel, const UTF8String& yLabel, const UTF8String& zLabel,
			double x, double y, double z, bool checkboxesEnabled, bool xEnabled, bool yEnabled, bool zEnabled,
			double xmin, double xmax, double ymin, double ymax, double zmin, double zmax,
			const string& helpURL, const string& helpTag)
{
	setModal (true);
	setWindowTitle (UTF8TOQSTRING (title));

	// Creation de l'ihm :
	QVBoxLayout*	layout	= new QVBoxLayout (this);
#ifdef QT_5
	layout->setMargin (QtConfiguration::margin);
#else	// => Qt6
	layout->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5
	layout->setSizeConstraint (QLayout::SetMinimumSize);
	
	// Le panneau :
	QtGroupBox*	groupBox	= new QtGroupBox ("Composantes", this);
	groupBox->setSpacing (QtConfiguration::spacing);
#ifdef QT_5
	groupBox->setMargin (QtConfiguration::margin);
#else	// => Qt6
	groupBox->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5

	layout->addWidget (groupBox);
	QVBoxLayout*	coordsLayout	= new QVBoxLayout (groupBox);
	groupBox->setLayout (coordsLayout);
	// Ligne 1 : X
	QWidget*		hbox		= new QWidget (groupBox);
	QHBoxLayout*	hboxLayout	= new QHBoxLayout ( );
#ifdef QT_5
	hboxLayout->setMargin (QtConfiguration::margin);
#else	// => Qt6
	hboxLayout->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5
	hboxLayout->setSpacing (QtConfiguration::spacing);
	hbox->setLayout (hboxLayout);
	coordsLayout->addWidget (hbox);
	QLabel*	label	= new QLabel (UTF8TOQSTRING (xLabel), hbox);
	UTF8String	xVal (x);
	_xTextField		= new QtTextField (UTF8TOQSTRING (xVal), hbox);
	_xTextField->setValidator (new QDoubleValidator (_xTextField));
	_modifyXCheckBox	= new QCheckBox ("Modifier", hbox);
	_modifyXCheckBox->setChecked (xEnabled);
	_modifyXCheckBox->setEnabled (checkboxesEnabled);
	hboxLayout->addWidget (label);
	hboxLayout->addWidget (_xTextField);
	hboxLayout->addWidget (_modifyXCheckBox);
	if (xmax >= xmin)
	{	// v 6.3.0
		UTF8String	rangeLabel;
		rangeLabel << "[" << xmin << ", " << xmax << "]";
		UTF8String	tooltip	= UTF8String ("Domaine : ", charset) + rangeLabel;
		label->setToolTip (UTF8TOQSTRING (tooltip));
		_xTextField->setToolTip (UTF8TOQSTRING (tooltip));
		label	= new QLabel (UTF8TOQSTRING (rangeLabel), hbox);
		hboxLayout->addWidget (label);
	}	// if (xmax >= xmin)

	// Ligne 2 : Y
	hbox		= new QWidget (groupBox);
	hboxLayout	= new QHBoxLayout ( );
#ifdef QT_5
	hboxLayout->setMargin (QtConfiguration::margin);
#else	// => Qt6
	hboxLayout->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5
	hboxLayout->setSpacing (QtConfiguration::spacing);
	hbox->setLayout (hboxLayout);
	coordsLayout->addWidget (hbox);
	label	= new QLabel (UTF8TOQSTRING (yLabel), hbox);
	UTF8String	yVal (y);
	_yTextField		= new QtTextField (UTF8TOQSTRING (yVal), hbox);
	_yTextField->setValidator (new QDoubleValidator (_yTextField));
	_modifyYCheckBox	= new QCheckBox ("Modifier", hbox);
	_modifyYCheckBox->setChecked (yEnabled);
	_modifyYCheckBox->setEnabled (checkboxesEnabled);
	hboxLayout->addWidget (label);
	hboxLayout->addWidget (_yTextField);
	hboxLayout->addWidget (_modifyYCheckBox);
	if (ymax >= ymin)
	{	// v 6.3.0
		UTF8String	rangeLabel;
		rangeLabel << "[" << ymin << ", " << ymax << "]";
		UTF8String	tooltip	= UTF8String ("Domaine : ", charset) + rangeLabel;
		label->setToolTip (UTF8TOQSTRING (tooltip));
		_yTextField->setToolTip (UTF8TOQSTRING (tooltip));
		label	= new QLabel (UTF8TOQSTRING (rangeLabel), hbox);
		hboxLayout->addWidget (label);
	}	// if (ymax >= ymin)

	// Ligne 3 : Y
	hbox		= new QWidget (groupBox);
	hboxLayout	= new QHBoxLayout ( );
#ifdef QT_5
	hboxLayout->setMargin (QtConfiguration::margin);
#else	// => Qt6
	hboxLayout->setContentsMargins (QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin, QtConfiguration::margin);
#endif	// QT_5
	hboxLayout->setSpacing (QtConfiguration::spacing);
	hbox->setLayout (hboxLayout);
	coordsLayout->addWidget (hbox);
	label	= new QLabel (UTF8TOQSTRING (zLabel), hbox);
	UTF8String	zVal (z);
	_zTextField		= new QtTextField (UTF8TOQSTRING (zVal), hbox);
	_zTextField->setValidator (new QDoubleValidator (_zTextField));
	_modifyZCheckBox	= new QCheckBox ("Modifier", hbox);
	_modifyZCheckBox->setChecked (zEnabled);
	_modifyZCheckBox->setEnabled (checkboxesEnabled);
	hboxLayout->addWidget (label);
	hboxLayout->addWidget (_zTextField);
	hboxLayout->addWidget (_modifyZCheckBox);
	if (zmax >= zmin)
	{	// v 6.3.0
		UTF8String	rangeLabel;
		rangeLabel << "[" << zmin << ", " << zmax << "]";
		UTF8String	tooltip	= UTF8String ("Domaine : ", charset) + rangeLabel;
		label->setToolTip (UTF8TOQSTRING (tooltip));
		_zTextField->setToolTip (UTF8TOQSTRING (tooltip));
		label	= new QLabel (UTF8TOQSTRING (rangeLabel), hbox);
		hboxLayout->addWidget (label);
	}	// if (zmax >= zmin)

	int	width	= _xTextField->sizeHint ( ).width ( ) > _yTextField->sizeHint ( ).width ( ) ? _xTextField->sizeHint ( ).width ( ) : _yTextField->sizeHint ( ).width ( );
	width		= _zTextField->sizeHint ( ).width ( ) > width ? _zTextField->sizeHint ( ).width ( ) : width;
	_xTextField->setMinimumWidth (width);	// v 6.3.0
	_yTextField->setMinimumWidth (width);	// v 6.3.0
	_zTextField->setMinimumWidth (width);	// v 6.3.0

	// Les boutons :
	layout->addWidget (new QLabel (" ", this));
	_closurePanel	= new QtDlgClosurePanel (this, false, "Appliquer", "", "Annuler", helpURL, helpTag);
	layout->addWidget (_closurePanel);
	_closurePanel->setMinimumSize (_closurePanel->sizeHint ( ));
	connect (_closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this, SLOT(accept ( )));
	connect (_closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this, SLOT(reject ( )));

	// Par defaut le bouton OK est artificellement clique par QDialog quand l'utilisateur fait return dans un champ de texte => on inhibe ce comportement par defaut :
	_closurePanel->getApplyButton ( )->setAutoDefault (false);
	_closurePanel->getApplyButton ( )->setDefault (false);
	_closurePanel->getCancelButton ( )->setAutoDefault (false);
	_closurePanel->getCancelButton ( )->setDefault (false);

	layout->activate ( );
	setMinimumSize (layout->sizeHint ( ));
}	// QtCoordinatesDialog::QtCoordinatesDialog


QtCoordinatesDialog::QtCoordinatesDialog (const QtCoordinatesDialog&)
{
	assert (0 && "QtCoordinatesDialog copy constructor is forbidden.");
}	// QtCoordinatesDialog::QtCoordinatesDialog (const QtCoordinatesDialog&)


QtCoordinatesDialog& QtCoordinatesDialog::operator = (const QtCoordinatesDialog&)
{
	assert (0 && "QtCoordinatesDialog assignment operator is forbidden.");
	return *this;
}	// QtCoordinatesDialog::operator =


QtCoordinatesDialog::~QtCoordinatesDialog ( )
{
}	// QtCoordinatesDialog::~QtCoordinatesDialog


void QtCoordinatesDialog::getCoordinates (double& x, double& y, double& z) const
{	
	assert (0 != _xTextField);
	assert (0 != _yTextField);
	assert (0 != _zTextField);

	bool	xOk	= true, yOk = true, zOk = true;
	x	= _xTextField->text ( ).toDouble (&xOk);
	y	= _yTextField->text ( ).toDouble (&yOk);
	z	= _zTextField->text ( ).toDouble (&zOk);

	if ((false == xOk) || (false == yOk) || (false == zOk))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Valeur incorrecte d'une des trois composantes.";
		throw Exception (errorMsg);
	}
}	// QtCoordinatesDialog::getCoordinates 


void QtCoordinatesDialog::getActivations (bool& x, bool& y, bool& z) const
{
	assert (0 != _modifyXCheckBox);
	assert (0 != _modifyYCheckBox);
	assert (0 != _modifyZCheckBox);
	x	= _modifyXCheckBox->isChecked ( );
	y	= _modifyYCheckBox->isChecked ( );
	z	= _modifyZCheckBox->isChecked ( );
}	// QtCoordinatesDialog::getActivations


void QtCoordinatesDialog::accept ( )
{
	bool	doX = false, doY = false, doZ = false;

	getActivations (doX, doY, doZ);
	if ((false == doX) && (false == doY) && (false == doZ))
	{
		string	title	= windowTitle ( ).toStdString ( );
		UTF8String	message (charset);
		message << "Aucune des trois composantes ne sera modifiée. Continuer ?";
		if (0 != QtMessageBox::displayWarningMessage (this, title, message, 100, "Oui", "Annuler", 0, -1))
			return;
	}	// if ((false == doX) && (false == doY) && (false == doZ))

	QDialog::accept ( );
}	// QtCoordinatesDialog::accept
