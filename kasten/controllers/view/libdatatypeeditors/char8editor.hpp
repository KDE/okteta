/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHAR8EDITOR_HPP
#define OKTETA_CHAR8EDITOR_HPP

// lib
#include "char8stringparser.hpp"
// libdatatypes
#include <char8.hpp>
// Qt
#include <QLineEdit>

namespace Okteta {
class CharCodec;

class Char8Editor : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(Char8 data READ data WRITE setData USER true)

public:
    Char8Editor(const CharCodec* charCodec, QWidget* parent);

    ~Char8Editor() override;

public:
    void setData(Char8 data);
    Char8 data() const;

private:
    Char8StringParser m_parser;
};

}

#endif
