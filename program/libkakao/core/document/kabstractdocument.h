/***************************************************************************
                          kabstractdocument.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
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


#ifndef KABSTRACTDOCUMENT_H
#define KABSTRACTDOCUMENT_H


// Qt
#include <QtCore/QObject>

class QString;

// TODO: store creation time? And time of last modification or access?
// last both might be too much overhead, unless modification and access are grained enough
// in multiuser environment also author/creator and group/identity
// we would end with a in-memory file/document system, why not?
class KAbstractDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY( SynchronizationStates synchronizationStates READ synchronizationStates )
    Q_FLAGS( SynchronizationStates SynchronizationState )

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

  public:
    virtual ~KAbstractDocument();

  public: // API to be implemented
    virtual QString title() const = 0;

    virtual SynchronizationStates synchronizationStates() const = 0;
// virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const = 0

  public: // helper or basic?
    bool hasLocalChanges() const;

  Q_SIGNALS:
    // TODO: should be signal the diff? how to say then remote is in synch again?
    void modified( KAbstractDocument::SynchronizationStates newStates );
    void titleChanged( const QString &newTitel );
};

Q_DECLARE_OPERATORS_FOR_FLAGS( KAbstractDocument::SynchronizationStates )

inline bool KAbstractDocument::hasLocalChanges() const { return synchronizationStates().testFlag(LocalHasChanges) ; }

inline KAbstractDocument::~KAbstractDocument() {}

#endif
