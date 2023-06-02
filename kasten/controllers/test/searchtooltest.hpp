/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef SEARCHTOOLTEST_HPP
#define SEARCHTOOLTEST_HPP

// Okteta
#include <Okteta/AddressRange>
// Qt
#include <QObject>
#include <QByteArray>

struct ViewData
{
    Okteta::Address cursorPosition = -1;
    Okteta::AddressRange selection;
    QByteArray data;
    Okteta::AddressRange match;
};

class SearchToolTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testParseToViewData_data();
    void testParseToViewData();

    void testSearch_data();
    void testSearch();

private:
    ViewData parseToViewData(const QByteArray& viewMarkup) const;
};

#endif
