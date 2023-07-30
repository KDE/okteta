/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF16EDITOR_HPP
#define KASTEN_UTF16EDITOR_HPP

// libdatatypes
#include <utf16.hpp>
// Qt
#include <QLineEdit>

class Utf16Editor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Utf16 data READ data WRITE setData USER true)

public:
    explicit Utf16Editor(QWidget* parent);

    ~Utf16Editor() override;

public:
    void setData(Utf16 data);
    Utf16 data() const;
};

#endif
