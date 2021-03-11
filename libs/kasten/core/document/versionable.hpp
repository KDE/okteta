/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_VERSIONABLE_HPP
#define KASTEN_IF_VERSIONABLE_HPP

// lib
// #include "documentversionid.hpp"
#include "documentversiondata.hpp"
// Qt
#include <QtPlugin>

/*
   This interface for now just deals with the working copy of the model,
   versions in the storing (like revision control systems) are not supported/integrated,
   needs a more general approach to input, synchronizer, changesets and such.

   A model/document can be versioned if the changes to it are traced and reversable.

   Addressing:
   While we don't do a distributed model and concurrent changes we use simple
   increasing indizes to address the different version, starting with 0.
   The initial version as created/loaded in memory gets the index 0. All following
   versions get a indexOfPrevious+1. This approach might be reusable for local
   addressing later.

   Wandering in the version "tree":
   The model can be reset to a previous version, or be set
   again to a newer version it had been advanced to before.
   If the model was reset to a previous version and is changed, currently no branch is
   created but all the versions that are newer are simply skipped.

   Version
   ^ Changes
   Version
   ^ Changes
   Version
   ^ Changed
   Version
   ^ Changes
   InitialVersion : Index = 0
 */

namespace Kasten {
namespace If {

class Versionable
{
public:
    virtual ~Versionable();

public: // get
//     virtual KDocumentVersionId versionId() const = 0;
    virtual int versionIndex() const = 0;
    virtual DocumentVersionData versionData(int versionIndex) const = 0;
//     virtual KDocumentVersionIterator currentVersion() const = 0;
//     virtual KDocumentVersionIterator rootVersion() const = 0;
//     virtual KDocumentVersionIterator headVersion() const = 0;
// for those with multiple branches:
//     virtual KDocumentVersionIterator headVersion( const KDocumentVersionBranchId& id) const = 0;
//     virtual QList<KDocumentVersionBranchId> heads() const = 0;
    virtual int versionCount() const = 0;

public: // set/action
//     virtual void setVersion( KDocumentVersionId id ) = 0;
    virtual void revertToVersionByIndex(int versionIndex) = 0;

public: // signal
//     virtual void versionChanged( KDocumentVersionId id ) = 0;
    //
    virtual void revertedToVersionIndex(int versionIndex) = 0;
    //
    virtual void headVersionDataChanged(const DocumentVersionData& versionData) = 0;
    virtual void headVersionChanged(int newHeadVersionIndex) = 0;
};

inline Versionable::~Versionable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::Versionable, "org.kde.kasten.if.versionable/1.0")

#endif
