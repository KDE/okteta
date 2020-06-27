/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytetablemodel.hpp"

// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
#include <Okteta/ValueCodec>
// KF
#include <KLocalizedString>
#include <KColorScheme>
// Qt
#include <QApplication>

namespace Kasten {

static constexpr QChar ByteTableDefaultSubstituteChar =  QLatin1Char('.');
static constexpr QChar ByteTableDefaultUndefinedChar =   QChar(QChar::ReplacementCharacter);
static constexpr int ByteSetSize = 256;

ByteTableModel::ByteTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mCharCodec(Okteta::CharCodec::createCodec(Okteta::LocalEncoding))
    , mSubstituteChar(ByteTableDefaultSubstituteChar)
    , mUndefinedChar(ByteTableDefaultUndefinedChar)
{
    constexpr Okteta::ValueCoding CodingIds[NofOfValueCodings] = {
        Okteta::DecimalCoding,
        Okteta::HexadecimalCoding,
        Okteta::OctalCoding,
        Okteta::BinaryCoding
    };
    for (int i = 0; i < NofOfValueCodings; ++i) {
        mValueCodec[i] = Okteta::ValueCodec::createCodec(CodingIds[i]);
    }
}

ByteTableModel::~ByteTableModel()
{
    for (auto* codec : qAsConst(mValueCodec)) {
        delete codec;
    }

    delete mCharCodec;
}

void ByteTableModel::setSubstituteChar(QChar substituteChar)
{
    if (substituteChar.isNull()) {
        substituteChar = ByteTableDefaultSubstituteChar;
    }

    if (mSubstituteChar == substituteChar) {
        return;
    }

    mSubstituteChar = substituteChar;

    emit dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

void ByteTableModel::setUndefinedChar(QChar undefinedChar)
{
    if (undefinedChar.isNull()) {
        undefinedChar = ByteTableDefaultUndefinedChar;
    }

    if (mUndefinedChar == undefinedChar) {
        return;
    }

    mUndefinedChar = undefinedChar;

    emit dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

void ByteTableModel::setCharCodec(const QString& codeName)
{
    if (codeName == mCharCodec->name()) {
        return;
    }

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec(codeName);

    emit dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

int ByteTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? ByteSetSize : 0;
}

int ByteTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfIds : 0;
}

QVariant ByteTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole) {
        QString content;

        const unsigned char byte = index.row();
        const int column = index.column();
        if (column == CharacterId) {
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            content =
                decodedChar.isUndefined() ?
                    i18nc("@item:intable character is not defined", "undef.") :
                !decodedChar.isPrint() ?
                    QString(mSubstituteChar) :
                    QString(static_cast<QChar>(decodedChar));
            // TODO: show proper descriptions for all control values, incl. space and delete
            // cmp. KCharSelect
        } else if (column < CharacterId) {
            mValueCodec[column]->encode(&content, 0, byte);
        }

        result = content;
    } else if (role == Qt::ForegroundRole) {
        const int column = index.column();
        if (column == CharacterId) {
            const unsigned char byte = index.row();
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            if (decodedChar.isUndefined() || !decodedChar.isPrint()) {
                const QPalette& palette = QApplication::palette();
                const KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
                result = colorScheme.foreground(KColorScheme::InactiveText);
            }
        }
    } else if (role == Qt::TextAlignmentRole) {
        result = Qt::AlignRight;
    }

    return result;
}

QVariant ByteTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
            section == DecimalId ?     i18nc("@title:column short for Decimal",     "Dec") :
            section == HexadecimalId ? i18nc("@title:column short for Hexadecimal", "Hex") :
            section == OctalId ?       i18nc("@title:column short for Octal",       "Oct") :
            section == BinaryId ?      i18nc("@title:column short for Binary",      "Bin") :
            section == CharacterId ?   i18nc("@title:column short for Character",   "Char") :
            QString();
        result = titel;
    } else if (role == Qt::ToolTipRole) {
        const QString titel =
            section == DecimalId ?
                i18nc("@info:tooltip column contains the value in decimal format",     "Decimal") :
            section == HexadecimalId ?
                i18nc("@info:tooltip column contains the value in hexadecimal format", "Hexadecimal") :
            section == OctalId ?
                i18nc("@info:tooltip column contains the value in octal format",       "Octal") :
            section == BinaryId ?
                i18nc("@info:tooltip column contains the value in binary format",      "Binary") :
            section == CharacterId ?
                i18nc("@info:tooltip column contains the character with the value",    "Character") :
                QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

}
