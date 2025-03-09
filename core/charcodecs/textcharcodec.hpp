/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TEXTCHARCODEC_HPP
#define OKTETA_TEXTCHARCODEC_HPP

// lib
#include <charcodec.hpp>
// Qt
#include <QString>
// Std
#include <memory>

class QTextCodec;
class QTextDecoder;
class QTextEncoder;

namespace Okteta {

// used by all codecs with full char coping, i.e. there are no undefined chars
class TextCharCodec : public CharCodec
{
    struct ConstructorTag
    {
        ConstructorTag() = default;
        ConstructorTag(const ConstructorTag&) = default;
    };

public:
    [[nodiscard]]
    static std::unique_ptr<TextCharCodec> createCodec(const QString& codecName);
    [[nodiscard]]
    static std::unique_ptr<TextCharCodec> createLocalCodec();

    [[nodiscard]]
    static const QStringList& codecNames();

public:
    explicit TextCharCodec(QTextCodec* textCodec, ConstructorTag);
    TextCharCodec(const TextCharCodec&) = delete;
    TextCharCodec(TextCharCodec&&) = delete;
    ~TextCharCodec() override;

    TextCharCodec& operator=(const TextCharCodec&) = delete;
    TextCharCodec& operator=(TextCharCodec&&) = delete;

public: // CharCodec API
    [[nodiscard]]
    Character decode(Byte byte) const override;
    [[nodiscard]]
    bool encode(Byte* byte, QChar _char) const override;
    [[nodiscard]]
    bool canEncode(QChar _char) const override;
    [[nodiscard]]
    QString name() const override;

private:
    // Qt owns all QTextCodec instances
    QTextCodec* const mCodec;
    /** decodes the chars to unicode */
    const std::unique_ptr<QTextDecoder> mDecoder;
    /** encodes the chars from unicode */
    const std::unique_ptr<QTextEncoder> mEncoder;
    /** */
    mutable QString mName;
};

}

#endif
