/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ABSTRACTVALUE8EDITOR_H
#define ABSTRACTVALUE8EDITOR_H

// lib
#include "../types/binary8.h"
// OKteta core
#include <okteta/oktetacore.h>
// Qt
#include <QSpinBox>

namespace Okteta {
class ValueCodec;
}

class AbstractValue8Editor : public QSpinBox
{
    Q_OBJECT

public:
    explicit AbstractValue8Editor(Okteta::ValueCoding valueCoding, QWidget* parent = nullptr);

    ~AbstractValue8Editor() override;

protected: // QSpinBox API
    QString textFromValue(int value) const override;
    int valueFromText(const QString& text) const override;

    QValidator::State validate(QString& text, int& pos) const override;

protected:
    Okteta::ValueCodec* mValueCodec;
};

#endif
