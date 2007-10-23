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
#include <kistorable.h>
// Qt
#include <QtCore/QString>

namespace KHECore {
class KByteArrayModel;
}


class KByteArrayDocument : public KAbstractDocument, public KDE::If::Storable//, public KDE::If::ByteArray
{
    Q_OBJECT
//     Q_INTERFACES(KDE::If::Storable KDE::If::ByteArray)
    Q_INTERFACES(KDE::If::Storable)

  public:
    KByteArrayDocument();
    explicit KByteArrayDocument( const QString &fileName );
    virtual ~KByteArrayDocument();

  public: // KAbstractDocument API
    virtual QString title() const;
    virtual SynchronizationStates synchronizationStates() const;

  public: // KDE::If::Storable
    virtual bool load( const QString &fileName );
    virtual bool save( const QString &fileName );
    virtual void setUrl( const QString &url );
    virtual QString url() const;
    virtual QString localFilePath() const;
  Q_SIGNALS:
    void urlChanged( const QString &url );

  public: // KDE::If::ByteArray
    virtual KHECore::KByteArrayModel *content() const;

  public:
    void setTitle( const QString &title );

  protected Q_SLOTS:
    void onModelModification( bool newState );

  protected:
    KHECore::KByteArrayModel *mByteArray;
    QString mLocalFilePath;
    QString mUrl;

    mutable QString mTitle;
};

#endif
