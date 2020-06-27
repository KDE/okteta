/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "statistictablemodel.hpp"

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

static constexpr QChar StatisticsDefaultSubstituteChar =  QLatin1Char('.');
static constexpr QChar StatisticsDefaultUndefinedChar =   QChar(QChar::ReplacementCharacter);
static constexpr Okteta::ValueCoding DefaultValueCoding =  Okteta::HexadecimalCoding;
static constexpr int StatisticsByteSetSize = 256;

StatisticTableModel::StatisticTableModel(int* byteCount, QObject* parent)
    : QAbstractTableModel(parent)
    , mByteCount(byteCount)
    , mValueCoding(DefaultValueCoding)
    , mValueCodec(Okteta::ValueCodec::createCodec(DefaultValueCoding))
    , mCharCodec(Okteta::CharCodec::createCodec(Okteta::LocalEncoding))
    , mSubstituteChar(StatisticsDefaultSubstituteChar)
    , mUndefinedChar(StatisticsDefaultUndefinedChar)
{
}

StatisticTableModel::~StatisticTableModel()
{
    delete mValueCodec;
    delete mCharCodec;
}

void StatisticTableModel::update(int size)
{
    mSize = size;
    emit dataChanged(index(0, CountId), index(StatisticsByteSetSize - 1, PercentId));
    emit sizeChanged(mSize);
}

void StatisticTableModel::setSubstituteChar(QChar substituteChar)
{
    if (substituteChar.isNull()) {
        substituteChar = StatisticsDefaultSubstituteChar;
    }

    if (mSubstituteChar == substituteChar) {
        return;
    }

    mSubstituteChar = substituteChar;

    emit dataChanged(index(0, CharacterId), index(StatisticsByteSetSize - 1, CharacterId));
}

void StatisticTableModel::setUndefinedChar(QChar undefinedChar)
{
    if (undefinedChar.isNull()) {
        undefinedChar = StatisticsDefaultUndefinedChar;
    }

    if (mUndefinedChar == undefinedChar) {
        return;
    }

    mUndefinedChar = undefinedChar;

    emit dataChanged(index(0, CharacterId), index(StatisticsByteSetSize - 1, CharacterId));
}

void StatisticTableModel::setValueCoding(int valueCoding)
{
    // no changes?
    if (mValueCoding == valueCoding) {
        return;
    }

    delete mValueCodec;

    mValueCoding = (Okteta::ValueCoding)valueCoding;
    mValueCodec = Okteta::ValueCodec::createCodec(mValueCoding);
//     CodedByte.resize( ByteCodec->encodingWidth() );

    emit dataChanged(index(0, ValueId), index(StatisticsByteSetSize - 1, ValueId));
    emit headerChanged();
}

void StatisticTableModel::setCharCodec(const QString& codeName)
{
    if (codeName == mCharCodec->name()) {
        return;
    }

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec(codeName);

    emit dataChanged(index(0, CharacterId), index(StatisticsByteSetSize - 1, CharacterId));
}

int StatisticTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? StatisticsByteSetSize : 0;
}

int StatisticTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfIds : 0;
}

QVariant StatisticTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole) {
        const unsigned char byte = index.row();
        const int column = index.column();
        switch (column)
        {
        case CharacterId:
        {
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            result =
                decodedChar.isUndefined() ?
                    i18nc("@item:intable character is not defined", "undef.") :
                !decodedChar.isPrint() ?
                    QString(mSubstituteChar) :
                    QString(static_cast<QChar>(decodedChar));
            // TODO: show proper descriptions for all control values, incl. space and delete
            // cmp. KCharSelect
            break;
        }
        case ValueId:
        {
            QString value;
            mValueCodec->encode(&value, 0, byte);
            result = value;
            break;
        }
        case CountId:
            result =  (mSize == -1) ?
                     QVariant(QStringLiteral("-")) :
                     QVariant(mByteCount[byte]);
            break;
        case PercentId:
            result = (mSize > 0) ?
                     // TODO: before we printed only a string (which killed sorting) with QString::number( x, 'f', 6 )
                     // Qt now cuts trailing 0s, results in unaligned numbers, not so beautiful.
                     QVariant(100.0 * (double)mByteCount[byte] / mSize) :
                     QVariant(QStringLiteral("-"));
            break;
        default:
            ;
        }
    } else if (role == Qt::TextAlignmentRole) {
        result = Qt::AlignRight;
    } else if (role == Qt::ForegroundRole) {
        const int column = index.column();
        bool isInactive = false;
        switch (column)
        {
        case CharacterId:
        {
            const unsigned char byte = index.row();
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            isInactive = decodedChar.isUndefined() || !decodedChar.isPrint();
            break;
        }
        case CountId:
            isInactive = (mSize == -1);
            break;
        case PercentId:
            isInactive = (mSize <= 0);
            break;
        default:
            ;
        }
        if (isInactive) {
            const QPalette& palette = QApplication::palette();
            const KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
            result = colorScheme.foreground(KColorScheme::InactiveText);
        }
    }

    return result;
}

QVariant StatisticTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
            section == ValueId ? (
                mValueCoding == Okteta::HexadecimalCoding ? i18nc("@title:column short for Hexadecimal", "Hex") :
                mValueCoding == Okteta::DecimalCoding ?     i18nc("@title:column short for Decimal",    "Dec") :
                mValueCoding == Okteta::OctalCoding ?       i18nc("@title:column short for Octal",      "Oct") :
                mValueCoding == Okteta::BinaryCoding ?      i18nc("@title:column short for Binary",     "Bin") :
                                                            QString()) :
            section == CharacterId ?   i18nc("@title:column short for Character",      "Char") :
            section == CountId ?       i18nc("@title:column count of characters",      "Count") :
            section == PercentId ?     i18nc("@title:column Percent of byte in total", "Percent") :
                                       QString();
        result = titel;
    } else if (role == Qt::ToolTipRole) {
        const QString titel =
            section == ValueId ? (
                mValueCoding == Okteta::HexadecimalCoding ?
                    i18nc("@info:tooltip column contains the value in hexadecimal format", "Hexadecimal") :
                mValueCoding == Okteta::DecimalCoding ?
                    i18nc("@info:tooltip column contains the value in decimal format",    "Decimal") :
                mValueCoding == Okteta::OctalCoding ?
                    i18nc("@info:tooltip column contains the value in octal format",      "Octal") :
                mValueCoding == Okteta::BinaryCoding ?
                    i18nc("@info:tooltip column contains the value in binary format",     "Binary") :
                // else
                    QString()) :
            section == CharacterId ?
                i18nc("@info:tooltip column contains the character with the value",   "Character") :
//             section == CountId ?       i18nc("@info:tooltip count of characters",      "Count") :
//             section == PercentId ?     i18nc("@info:tooltip Percent of byte in total", "Percent") :
                QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

}
