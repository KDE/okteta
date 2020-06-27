/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocument.hpp"

namespace Kasten {

TestDocument::TestDocument() = default;
TestDocument::TestDocument(const QByteArray& data)
    : mData(data)
{
}

TestDocument::~TestDocument() = default;

QString TestDocument::mimeType() const { return QStringLiteral("TestDocument"); }
QString TestDocument::typeName() const { return QStringLiteral("Test Document"); }
QString TestDocument::title() const { return mTitle; }
ContentFlags TestDocument::contentFlags() const { return mContentFlags; }
const QByteArray* TestDocument::data() const { return &mData; }

void TestDocument::setData(const QByteArray& data)
{
    const ContentFlags oldContentFlags = mContentFlags;

    mData = data;

    mContentFlags = mContentFlags | ContentHasUnstoredChanges;

    if (oldContentFlags != mContentFlags) {
        emit contentFlagsChanged(mContentFlags);
    }
}

void TestDocument::setTitle(const QString& title)
{
    if (mTitle != title) {
        mTitle = title;
        emit titleChanged(title);
    }
}

void TestDocument::setContentFlags(ContentFlags contentFlags)
{
    if (mContentFlags != contentFlags) {
        mContentFlags = contentFlags;

        emit contentFlagsChanged(contentFlags);
    }
}

}
