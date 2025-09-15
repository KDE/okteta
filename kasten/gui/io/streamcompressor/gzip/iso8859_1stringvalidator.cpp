/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "iso8859_1stringvalidator.hpp"

// Qt
#include <QTextCodec>

namespace Okteta {

Iso8859_1StringValidator::Iso8859_1StringValidator(QObject* parent)
    : QValidator(parent)
    , m_codec(QTextCodec::codecForName("ISO-8859-1"))
{}


QValidator::State Iso8859_1StringValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)

    return m_codec->canEncode(input) ? QValidator::Acceptable : QValidator::Invalid;
}

}

#include "moc_iso8859_1stringvalidator.cpp"
