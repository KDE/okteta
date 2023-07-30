/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_OCTAL8EDITOR_HPP
#define OKTETA_OCTAL8EDITOR_HPP

// lib
#include "abstractvalue8editor.hpp"
// libdatatypes
#include <octal8.hpp>

namespace Okteta {

class Octal8Editor : public AbstractValue8Editor
{
    Q_OBJECT
    Q_PROPERTY(Octal8 data READ data WRITE setData USER true)

public:
    explicit Octal8Editor(QWidget* parent);

    ~Octal8Editor() override;

public:
    void setData(Octal8 data);
    Octal8 data() const;
};

}

#endif
