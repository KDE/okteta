/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDOCUMENTTEST_HPP
#define KASTEN_BYTEARRAYDOCUMENTTEST_HPP

// Qt
#include <QObject>

namespace Kasten {

class ByteArrayDocumentTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testCreateNew();
    void testSetTitle();
};

}

#endif
