/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H


// lib
#include "kastencore_export.h"
// lib
#include "abstractmodel.h"


namespace Kasten
{

class AbstractModelSynchronizer;

// TODO: store creation time? And time of last modification or access?
// last both might be too much overhead, unless modification and access are grained enough
// in multiuser environment also author/creator and group/identity
// we would end with a in-memory file/document system, why not?
class KASTENCORE_EXPORT AbstractDocument : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY( SynchronizationStates synchronizationStates READ synchronizationStates )
    Q_FLAGS( SynchronizationStates SynchronizationState )

  friend class AbstractModelSynchronizer;
  friend class DocumentManager;

  public:
    //TODO: some things are a tristate, is it the right thing to embed them here?
    // or make it a value of its own?
    // see RemoteHasChanges or not, RemoteUnknown
    enum SynchronizationState
    {
        InSync = 0, //TODO: find better name
        LocalHasChanges  = 1,
        RemoteHasChanges = 2,
        /// unknown, e.g. because connection not available/lost
        RemoteUnknown = 4
    };
    Q_DECLARE_FLAGS( SynchronizationStates, SynchronizationState )

  protected:
    AbstractDocument();
  public:
    virtual ~AbstractDocument();

  public: // API to be implemented
    // TODO: what about plurals?
    virtual QString typeName() const = 0;
    virtual QString mimeType() const = 0;

    virtual SynchronizationStates synchronizationStates() const = 0;
// virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const = 0

  public:
    void setSynchronizer( AbstractModelSynchronizer* synchronizer );
    void setLiveSynchronizer( AbstractModelSynchronizer* synchronizer );

  public: // helper or basic?
    bool hasLocalChanges() const;
    AbstractModelSynchronizer* synchronizer() const;
    AbstractModelSynchronizer* liveSynchronizer() const;
    QString id() const;

  Q_SIGNALS:
    // TODO: should be signal the diff? how to say then remote is in synch again?
    // could be done by pairs of flags instead of notset = isnot
    void syncStatesChanged( Kasten::AbstractDocument::SynchronizationStates newStates );
    void synchronizerChanged( Kasten::AbstractModelSynchronizer* newSynchronizer );
    void liveSynchronizerChanged( Kasten::AbstractModelSynchronizer* newSynchronizer );

  protected:
    void setId( const QString& id );

  protected:
    class Private;
    Private* const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( AbstractDocument::SynchronizationStates )

}

#endif
