#ifndef QT_FILE_DIALOG_UTILITIES_H
#define QT_FILE_DIALOG_UTILITIES_H

#include <vector>
#include <string>
#include <QString>


/**
 * Utilitaires pour sélecteur de fichiers Qt.
 */
class QtFileDialogUtilities
{
	public :

	/**
	 * @return	La partie "terminale" de l'extension, c.a.d. ce qui suit le point ("jpg" pour "*.jpg").
	 */
	static std::string extensionTail (const std::string& extension);

	/**
	 * @return	Un vecteur contenant les parties terminales des extensions du filtre Qt transmis en argument.
	 */
	static std::vector<std::string> nameFilterToExtensions (const QString& filters);

	/**
	 * @return	<I>true</I> si la partie terminale de l'extension tranmise en premier argument est dans
	 *			le vecteur transmis en second argument (comparaison non "case sensitive").
	 */
	static bool extensionMatches (const std::string& extension, const std::vector<std::string>& extensions);
	
	/**
	 * @param	Nom de fichier proposé
	 * @param	Filtre Qt utilisé lors du choix du fichier
	 * @return	Le nom de fichier avec extension. Si le nom proposé n'en n'a pas alors la première du filtre est ajoutée.
	 * @since	6.7.0
	 */
	static std::string completeFileName (const std::string& path, const std::string& filter);
	
	
	private :
	
	QtFileDialogUtilities ( );
	QtFileDialogUtilities (const QtFileDialogUtilities&);
	QtFileDialogUtilities& operator = (const QtFileDialogUtilities&);
	~QtFileDialogUtilities ( );
};	// class QtFileDialogUtilities


#endif	// QT_FILE_DIALOG_UTILITIES_H
