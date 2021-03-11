/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTDOCUMENT_HPP
#define KASTEN_ABSTRACTDOCUMENT_HPP

// lib
#include "kastencore_export.hpp"
#include "kastencore.hpp"
#include "abstractmodel.hpp"

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
    friend class DocumentManagerPrivate;

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

private:
    Q_DECLARE_PRIVATE(AbstractDocument)
};

}

#endif
