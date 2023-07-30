/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf16editor.hpp"

// Qt
#include <QValidator>

namespace Okteta {

class Utf16CharValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Utf16CharValidator(QObject* parent = nullptr);

    ~Utf16CharValidator() override;

public: // QValidator API
    QValidator::State validate(QString& input, int& pos) const override;
};

Utf16CharValidator::~Utf16CharValidator() = default;

inline Utf16CharValidator::Utf16CharValidator(QObject* parent) : QValidator(parent) {}

QValidator::State Utf16CharValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)

    const int stringLength = input.length();
    return (stringLength == 0) ? QValidator::Intermediate : QValidator::Acceptable;
}


Utf16Editor::Utf16Editor(QWidget* parent)
    : QLineEdit(parent)
{
    setValidator(new Utf16CharValidator(this));
    setMaxLength(1);
    setClearButtonEnabled(true);
}

Utf16Editor::~Utf16Editor() = default;

void Utf16Editor::setData(Utf16 data)
{
    setText(data.value);
}

Utf16 Utf16Editor::data() const
{
    const QString t = text();
    return {t.isEmpty() ? QChar(0) : t[0]};
}

}

#include "utf16editor.moc"
#include "moc_utf16editor.cpp"
