/***************************************************************************
                          kbytearraydocument.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTEARRAYDOCUMENT_H
#define KBYTEARRAYDOCUMENT_H


// program
// #include "kbytearrayinterface.h"
// libkvmt
#include <kabstractdocument.h>
// Qt
#include <QtCore/QString>

namespace KHECore {
class KByteArrayModel;
}


class KByteArrayDocument : public KAbstractDocument//, public KDE::If::ByteArray
{
    Q_OBJECT
//     Q_INTERFACES(KDE::If::ByteArray)

  public:
    KByteArrayDocument();
    explicit KByteArrayDocument( KHECore::KByteArrayModel *byteArray );
    virtual ~KByteArrayDocument();

  public: // KAbstractDocument API
    virtual QString title() const;
    virtual QString typeName() const;
    virtual QString mimeType() const;
    virtual SynchronizationStates synchronizationStates() const;

  public: // KDE::If::ByteArray
    virtual KHECore::KByteArrayModel *content() const;

  public:
    void setTitle( const QString &title );

  protected Q_SLOTS:
    void onModelModification( bool newState );

  protected:
    KHECore::KByteArrayModel *mByteArray;

    mutable QString mTitle;
};

#endif
