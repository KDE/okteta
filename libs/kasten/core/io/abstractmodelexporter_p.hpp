/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELEXPORTER_P_HPP
#define KASTEN_ABSTRACTMODELEXPORTER_P_HPP

#include "abstractmodelexporter.hpp"

namespace Kasten {

class AbstractModelExporterPrivate
{
public:
    AbstractModelExporterPrivate(AbstractModelExporter* parent,
                                 const QString& remoteTypeName, const QString& remoteMimeType);

    virtual ~AbstractModelExporterPrivate();

public:
    const QString& remoteTypeName() const;
    const QString& remoteMimeType() const;

protected:
    AbstractModelExporter* const q_ptr;

    const QString mRemoteTypeName;
    const QString mRemoteMimeType;
};

inline AbstractModelExporterPrivate::AbstractModelExporterPrivate(AbstractModelExporter* parent,
                                                                  const QString& remoteTypeName, const QString& remoteMimeType)
    : q_ptr(parent)
    , mRemoteTypeName(remoteTypeName)
    , mRemoteMimeType(remoteMimeType)
{}

inline AbstractModelExporterPrivate::~AbstractModelExporterPrivate() = default;

inline const QString& AbstractModelExporterPrivate::remoteTypeName() const { return mRemoteTypeName; }
inline const QString& AbstractModelExporterPrivate::remoteMimeType() const { return mRemoteMimeType; }

}

#endif
