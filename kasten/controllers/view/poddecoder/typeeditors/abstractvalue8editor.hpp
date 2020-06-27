/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTVALUE8EDITOR_HPP
#define KASTEN_ABSTRACTVALUE8EDITOR_HPP

// lib
#include "../types/binary8.hpp"
// OKteta core
#include <Okteta/OktetaCore>
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
