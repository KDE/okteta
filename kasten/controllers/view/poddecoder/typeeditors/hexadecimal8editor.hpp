/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_HEXADECIMAL8EDITOR_HPP
#define KASTEN_HEXADECIMAL8EDITOR_HPP

// lib
#include "../types/hexadecimal8.hpp"
#include "abstractvalue8editor.hpp"

class Hexadecimal8Editor : public AbstractValue8Editor
{
    Q_OBJECT
    Q_PROPERTY(Hexadecimal8 data READ data WRITE setData USER true)

public:
    explicit Hexadecimal8Editor(QWidget* parent);

    ~Hexadecimal8Editor() override;

public:
    void setData(Hexadecimal8 data);
    Hexadecimal8 data() const;
};

#endif
