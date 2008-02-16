/***************************************************************************
                          kpiecetablebytearraymodel_p.h  -  description
                             -------------------
    begin                : Fri Jan 18 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KPIECETABLEBYTEARRAYMODEL_PRIVATE_H
#define KHE_CORE_KPIECETABLEBYTEARRAYMODEL_PRIVATE_H

// lib
#include "kpiecetablebytearraymodel.h"
// piecetable
#include "piecetable/revertablepiecetable.h"
// Qt
#include <QtCore/QByteArray>


namespace KHECore
{

/** base class for all mData buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class KPieceTableByteArrayModel::Private
{
  public:
    /** creates a readonly buffer around the given data */
    Private( KPieceTableByteArrayModel *parent, const char *data, unsigned int size,
             bool careForMemory = true );
    /** creates a writeable buffer which is deleted at the end */
    explicit Private( KPieceTableByteArrayModel *parent, unsigned int size, char fillByte = '\0' );
    ~Private();

  public: // KAbstractByteArrayModel API
    char datum( unsigned int offset ) const;
    int size() const;
    bool isReadOnly() const;
    bool isModified() const;

    int insert( int offset, const char *insertData, int insertLength );
    int remove( const KSection &removeSection );
    unsigned int replace( const KSection &removeSection, const char *insertData, unsigned int insertLength );
    bool swap( int firstStart, const KSection &secondSection );
    int fill( const char fillByte, unsigned int Pos = 0, int fillLength = -1 );
    void setDatum( unsigned int offset, const char byte );

    void setModified( bool modified = true );
    void setReadOnly( bool readonly = true );

  public: // Versionable API
    int versionIndex() const;
    int versionCount() const;
    QString versionDescription( int versionIndex ) const;

  public: // set/action
    void revertToVersionByIndex( int versionIndex );

  public:
    void setData( const char *data, unsigned int size, bool careForMemory = true );

  protected:
    KPieceTableByteArrayModel *p;
    /**  */
    bool mReadOnly:1;
    /** */
    bool mModified:1;
    /** */
    bool mAutoDelete:1;

    const char *mData;
    KPieceTable::RevertablePieceTable mPieceTable;
    QByteArray mChangeByteArray;
};


inline int KPieceTableByteArrayModel::Private::size() const  { return mPieceTable.size(); }

inline bool KPieceTableByteArrayModel::Private::isReadOnly()   const { return mReadOnly; }
inline bool KPieceTableByteArrayModel::Private::isModified()   const { return mModified; }

inline void KPieceTableByteArrayModel::Private::setReadOnly( bool readOnly )
{
    if( mReadOnly != readOnly )
    {
        mReadOnly = readOnly;
        emit p->readOnlyChanged( readOnly );
    }
}
inline void KPieceTableByteArrayModel::Private::setModified( bool modified )
{
    if( mModified != modified )
    {
        mModified = modified;
        emit p->modificationChanged( modified );
    }
}

inline int KPieceTableByteArrayModel::Private::versionIndex() const { return mPieceTable.appliedChangesCount(); }
inline int KPieceTableByteArrayModel::Private::versionCount() const { return mPieceTable.changesCount()+1; }
inline QString KPieceTableByteArrayModel::Private::versionDescription( int versionIndex ) const
{ return mPieceTable.changeDescription( versionIndex-1 ); }

}

#endif
