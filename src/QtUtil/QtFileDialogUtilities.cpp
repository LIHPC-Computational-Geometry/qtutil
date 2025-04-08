#include "QtUtil/QtFileDialogUtilities.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/File.h>
#include <TkUtil/UTF8String.h>
#include <assert.h>
#include <sstream>

using namespace TkUtil;
using namespace std;

static const	Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


/**
 * @return	La première extension du filtre Qt reçu en argument (format <I>NOM (*.ext1 *.ext2 ... *.extn)</I>.
 */
static string getFirstExtension (const string& filter)
{
	UTF8String	prepared (filter);
	prepared.replace (string ("("), string (" "), true);	// => nom *.ext1)
	prepared.replace (string ("*"), string (""), true);	// => nom .ext1)
	prepared.replace (string (")"), string (""), true);	// => nom *.ext1
	istringstream	stream (prepared.utf8 ( ));
	string	name, ext;
	stream >> name >> ext;


	if ((false == stream.fail ( )) && (false == stream.bad ( )))
		return ext;

	return string ( );
}	// getFirstExtension



QtFileDialogUtilities::QtFileDialogUtilities ( )
{
	assert (0 && "QtFileDialogUtilities constructor is not allowed.");
}	// QtFileDialogUtilities::QtFileDialogUtilities


QtFileDialogUtilities::QtFileDialogUtilities (const QtFileDialogUtilities&)
{
	assert (0 && "QtFileDialogUtilities constructor is not allowed.");
}	// QtFileDialogUtilities::QtFileDialogUtilities


QtFileDialogUtilities& QtFileDialogUtilities::operator = (const QtFileDialogUtilities&)
{
	assert (0 && "QtFileDialogUtilities assignment operator is not allowed.");
	return *this;
}	// QtFileDialogUtilities::operator =


QtFileDialogUtilities::~QtFileDialogUtilities ( )
{
	assert (0 && "QtFileDialogUtilities destructor is not allowed.");
}	// QtFileDialogUtilities::~QtFileDialogUtilities


string QtFileDialogUtilities::extensionTail (const string& extension)
{
	QString	work	= QString (extension.c_str ( )).trimmed ( );
	if (0 == work.indexOf ("*."))
		return work.toStdString ( ).substr (2);
	if (0 == work.indexOf ("."))
		return work.toStdString ( ).substr (1);
		
	return extension;
}	// QtFileDialogUtilities::extensionTail


vector<string> QtFileDialogUtilities::nameFilterToExtensions (const QString& filters)
{
	vector<string>	extensions;
	
	int start	= filters.lastIndexOf ("(");
	start	= -1 == start ? 0 : start + 1;
	int end	= filters.indexOf (")", start);
	end		= -1 == end ? filters.length ( ) : end - 1;
	string	extensionsList	= QString (filters.toStdString ( ).substr (start, end - start + 1).c_str ( )).trimmed ( ).toStdString ( );
	
	size_t	pos	= 0;
	while (string::npos != (pos = extensionsList.find (' ')))
	{
		const string	extension	= extensionsList.substr (0, pos);
		extensions.push_back (extensionTail (extension));
		extensionsList.erase (0, pos + 1);
	}	// while (string::npos != (pos = extensionsList.find (' ')))
	if (0 != extensionsList.length ( ))
	{
		extensions.push_back (extensionTail (extensionsList));
	}
		
	return extensions;
}	// QtFileDialogUtilities::nameFilterToExtensions


bool QtFileDialogUtilities::extensionMatches (const string& extension, const vector<string>& extensions)
{
	const QString	ext (extension.c_str ( ));
	for (vector<string>::const_iterator it = extensions.begin ( ); extensions.end ( ) != it; it++)
		if (0 == QString::compare (ext, QString ((*it).c_str ( )), Qt::CaseInsensitive))
			return true;
			
	return false;
}	// QtFileDialogUtilities::extensionMatches


string QtFileDialogUtilities::completeFileName (const string& path, const string& filter)
{
	File	file (path);
	if (0 != file.getExtension ( ).length ( ))
		return path;

	// Le fichier n'a pas d'extension, on rajoute la première du filtre 
	const string	ext	= getFirstExtension (filter);
	UTF8String	newPath (charset);
	newPath << path << ext;

	return newPath.utf8 ( );	
}	// QtFileDialogUtilities::completeFileName
