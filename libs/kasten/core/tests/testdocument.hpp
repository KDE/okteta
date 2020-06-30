/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENT_HPP
#define KASTEN_TESTDOCUMENT_HPP

// lib
#include "abstractdocument.hpp"
// Qt
#include <QByteArray>
#include <QString>

namespace Kasten {

class TestDocument : public AbstractDocument
{
    Q_OBJECT

public:
    TestDocument();
    explicit TestDocument(const QByteArray& data);
    ~TestDocument() override;

public: // API to be implemented
    QString title() const override;
    QString typeName() const override;
    QString mimeType() const override;
    ContentFlags contentFlags() const override;

public:
    const QByteArray* data() const;
    void setData(const QByteArray& data);

public: // instruction functions
    void setTitle(const QString& title);
    void setContentFlags(ContentFlags contentFlags);

private:
    QString mTitle;
    QByteArray mData;
    ContentFlags mContentFlags = ContentStateNormal;
};

}

#endif
