/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLOAT64EDITOR_HPP
#define KASTEN_FLOAT64EDITOR_HPP

// lib
#include "../types/float64.hpp"
// Qt
#include <QLineEdit>

class Float64Editor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Float64 data READ data WRITE setData USER true)

public:
    explicit Float64Editor(QWidget* parent);

    ~Float64Editor() override;

public:
    void setData(Float64 data);
    Float64 data() const;
};

#endif
