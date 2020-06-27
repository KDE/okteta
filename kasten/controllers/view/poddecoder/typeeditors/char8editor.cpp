/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8editor.hpp"

// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
// Qt
#include <QValidator>

class Char8Validator : public QValidator
{
    Q_OBJECT

public:
    Char8Validator(Okteta::CharCodec* charCodec, QObject* parent = nullptr);

    ~Char8Validator() override;

public: // QValidator API
    QValidator::State validate(QString& input, int& pos) const override;

private:
    Okteta::CharCodec* mCharCodec;
};

inline Char8Validator::Char8Validator(Okteta::CharCodec* charCodec, QObject* parent)
    : QValidator(parent)
    , mCharCodec(charCodec)
{}

Char8Validator::~Char8Validator() = default;

QValidator::State Char8Validator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)

    State result;

    const int stringLength = input.length();
    if (stringLength == 0) {
        result = QValidator::Intermediate;
    } else {
        const QChar c = input.at(0);

        result = mCharCodec->canEncode(c) ? QValidator::Acceptable : QValidator::Invalid;
    }

    return result;
}


Char8Editor::Char8Editor(Okteta::CharCodec* charCodec, QWidget* parent)
    : QLineEdit(parent)
{
    setValidator(new Char8Validator(charCodec, this));
    setClearButtonEnabled(true);
    setMaxLength(1);
}

Char8Editor::~Char8Editor() = default;

void Char8Editor::setData(Char8 data)
{
    setText(data.character.isUndefined() ? QString() : QString(data.character));
}

Char8 Char8Editor::data() const
{
    const QString t = text();
    return {t.isEmpty() ? QChar(0) : t[0]};
}

#include "char8editor.moc"
