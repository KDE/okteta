/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef CHAR8STRINGVALIDATORTEST_HPP
#define CHAR8STRINGVALIDATORTEST_HPP

// Qt
#include <QObject>

class Char8StringValidatorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testValidate_data();
    void testValidate();
};

#endif
