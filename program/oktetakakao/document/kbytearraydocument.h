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
// kakao
#include <kiversionable.h>
#include <kabstractdocument.h>
// Qt
#include <QtCore/QString>

namespace KHECore {
class KAbstractByteArrayModel;
class KPieceTableByteArrayModel;
}


class KByteArrayDocument : public KAbstractDocument, public KDE::If::Versionable//, public KDE::If::ByteArray
{
    Q_OBJECT
    Q_INTERFACES(KDE::If::Versionable)
//     Q_INTERFACES(KDE::If::ByteArray)

  public:
    explicit KByteArrayDocument( const QString &initDescription );
    KByteArrayDocument( KHECore::KPieceTableByteArrayModel *byteArray, const QString &initDescription );
    virtual ~KByteArrayDocument();

  public: // KAbstractDocument API
    virtual QString title() const;
    virtual QString typeName() const;
    virtual QString mimeType() const;
    virtual SynchronizationStates synchronizationStates() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // KDE::If::Versionable
    virtual int versionIndex() const;
    virtual KDocumentVersionData versionData( int versionIndex ) const;
    virtual int versionCount() const;
    virtual void revertToVersionByIndex( int versionIndex );

  public: // KDE::If::ByteArray
    virtual KHECore::KAbstractByteArrayModel *content() const;

  public:
    void setTitle( const QString &title );

  Q_SIGNALS: // KDE::If::Versionable
    virtual void revertedToVersionIndex( int versionIndex );
    virtual void headVersionDataChanged( const KDocumentVersionData &versionData );
    virtual void headVersionChanged( int newHeadVersionIndex );

  protected Q_SLOTS:
    void onModelModification( bool newState );
    void onHeadVersionDescriptionChanged( const QString &newDescription );

  protected:
    KHECore::KPieceTableByteArrayModel *mByteArray;

    mutable QString mTitle;

    const QString mInitDescription;
};

#endif
