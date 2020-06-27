/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf8editor.hpp"

// Qt
#include <QValidator>

class Utf8CharValidator : public QValidator
{
    Q_OBJECT

public:
    explicit Utf8CharValidator(QObject* parent = nullptr);

    ~Utf8CharValidator() override;

public: // QValidator API
    QValidator::State validate(QString& input, int& pos) const override;
};

Utf8CharValidator::~Utf8CharValidator() = default;

inline Utf8CharValidator::Utf8CharValidator(QObject* parent) : QValidator(parent) {}

QValidator::State Utf8CharValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)

    const int stringLength = input.length();
    return (stringLength == 0) ? QValidator::Intermediate : QValidator::Acceptable;
}


Utf8Editor::Utf8Editor(QWidget* parent)
    : QLineEdit(parent)
{
    setValidator(new Utf8CharValidator(this));
    setMaxLength(1);
    setClearButtonEnabled(true);
}

Utf8Editor::~Utf8Editor() = default;

void Utf8Editor::setData(Utf8 data)
{
    setText(data.value);
}

Utf8 Utf8Editor::data() const
{
    const QString t = text();
    return {t.isEmpty() ? QChar(0) : t[0]};
}

#include "utf8editor.moc"
