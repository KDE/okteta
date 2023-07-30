/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BINARY8EDITOR_HPP
#define OKTETA_BINARY8EDITOR_HPP

// lib
#include "abstractvalue8editor.hpp"
// libdatatypes
#include <binary8.hpp>

namespace Okteta {

class Binary8Editor : public AbstractValue8Editor
{
    Q_OBJECT
    Q_PROPERTY(Binary8 data READ data WRITE setData USER true)

public:
    explicit Binary8Editor(QWidget* parent);

    ~Binary8Editor() override;

public:
    void setData(Binary8 data);
    Binary8 data() const;
};

}

#endif
