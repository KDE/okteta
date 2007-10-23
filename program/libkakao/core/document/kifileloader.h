/***************************************************************************
                          kstorableinterface.h  -  description
                             -------------------
    begin                : Sun Sep 2 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDE_IF_FILELOADER_H
#define KDE_IF_FILELOADER_H

// Qt
#include <QtCore/QtPlugin>

class KAbstractDocument;
class QString;

namespace KDE
{
namespace If
{

/** Interface for factories which load from filesystem
  * 
  * @author Friedrich W. H. Kossebau <kossebau@kde.org>
  */
class FileLoader
{
  public:
    virtual ~FileLoader();

  public: // load
    virtual KAbstractDocument *load( const QString &tmpFileName, const QString &originUrl ) = 0;
//     virtual void reload( KAbstractDocument* Document ) = 0;
//     virtual void save( KAbstractDocument* Document ) = 0;
};

inline FileLoader::~FileLoader() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::FileLoader, "org.kde.if.fileloader/1.0" )

#endif
