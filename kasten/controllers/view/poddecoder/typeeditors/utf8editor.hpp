/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF8EDITOR_HPP
#define KASTEN_UTF8EDITOR_HPP

// lib
#include "../types/utf8.hpp"
// Qt
#include <QLineEdit>

class Utf8Editor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Utf8 data READ data WRITE setData USER true)

public:
    explicit Utf8Editor(QWidget* parent);

    ~Utf8Editor() override;

public:
    void setData(Utf8 data);
    Utf8 data() const;
};

#endif
