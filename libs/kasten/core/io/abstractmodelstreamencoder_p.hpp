/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELSTREAMENCODER_P_HPP
#define KASTEN_ABSTRACTMODELSTREAMENCODER_P_HPP

#include "abstractmodelstreamencoder.hpp"

namespace Kasten {

class AbstractModelStreamEncoderPrivate
{
public:
    explicit AbstractModelStreamEncoderPrivate(AbstractModelStreamEncoder* parent,
                                               const QString& remoteTypeName, const QString& remoteMimeType,
                                               const QString& remoteClipboardMimeType);

    virtual ~AbstractModelStreamEncoderPrivate();

public:
    const QString& remoteTypeName() const;
    const QString& remoteMimeType() const;
    const QString& remoteClipboardMimeType() const;

protected:
    AbstractModelStreamEncoder* const q_ptr;

    const QString mRemoteTypeName;
    const QString mRemoteMimeType;
    const QString mRemoteClipboardMimeType;
};

inline AbstractModelStreamEncoderPrivate::AbstractModelStreamEncoderPrivate(AbstractModelStreamEncoder* parent,
                                                                            const QString& remoteTypeName, const QString& remoteMimeType, const QString& remoteClipboardMimeType)
    : q_ptr(parent)
    , mRemoteTypeName(remoteTypeName)
    , mRemoteMimeType(remoteMimeType)
    , mRemoteClipboardMimeType(remoteClipboardMimeType.isEmpty() ? remoteMimeType : remoteClipboardMimeType)
{}

inline AbstractModelStreamEncoderPrivate::~AbstractModelStreamEncoderPrivate() = default;

inline const QString& AbstractModelStreamEncoderPrivate::remoteTypeName() const { return mRemoteTypeName; }
inline const QString& AbstractModelStreamEncoderPrivate::remoteMimeType() const { return mRemoteMimeType; }
inline const QString& AbstractModelStreamEncoderPrivate::remoteClipboardMimeType() const
{ return mRemoteClipboardMimeType; }

}

#endif
