/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_ABSTRACTDOCUMENT_HPP
#define KASTEN_ABSTRACTDOCUMENT_HPP

// lib
#include <kasten/kastencore_export.hpp>
#include <kasten/kastencore.hpp>
// lib
#include <kasten/abstractmodel.hpp>

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocumentPrivate;

// TODO: store creation time? And time of last modification or access?
// last both might be too much overhead, unless modification and access are grained enough
// in multiuser environment also author/creator and group/identity
// we would end with a in-memory file/document system, why not?
class KASTENCORE_EXPORT AbstractDocument : public AbstractModel
{
    Q_OBJECT

    friend class AbstractModelSynchronizer;
    friend class DocumentManager;

protected:
    AbstractDocument();

public:
    ~AbstractDocument() override;

public: // API to be implemented
    // TODO: what about plurals?
    virtual QString typeName() const = 0;
    virtual QString mimeType() const = 0;
    virtual ContentFlags contentFlags() const = 0;

public:
    void setSynchronizer(AbstractModelSynchronizer* synchronizer);

public: // helper or basic?
    AbstractModelSynchronizer* synchronizer() const;
    QString id() const;

Q_SIGNALS:
    void synchronizerChanged(Kasten::AbstractModelSynchronizer* newSynchronizer);
    void contentFlagsChanged(Kasten::ContentFlags contentFlags);

protected:
    void setId(const QString& id);

protected:
    Q_DECLARE_PRIVATE(AbstractDocument)
};

}

#endif
