/*
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
#include <QFontDatabase>

namespace Kasten {

StatisticTableModel::StatisticTableModel(int* byteCount, QObject* parent)
    : QAbstractTableModel(parent)
    , mByteCount(byteCount)
    , mValueCodec(Okteta::ValueCodec::createCodec(mValueCoding))
    , mCharCodec(Okteta::CharCodec::createCodecForLocale())
    , mFixedFont(QFontDatabase::systemFont(QFontDatabase::FixedFont))
{
}

StatisticTableModel::~StatisticTableModel() = default;

void StatisticTableModel::update(int size)
{
    mSize = size;
    Q_EMIT dataChanged(index(0, CountId), index(ByteSetSize - 1, PercentId));
    Q_EMIT sizeChanged(mSize);
}

void StatisticTableModel::setSubstituteChar(QChar substituteChar)
{
    if (substituteChar.isNull()) {
        substituteChar = DefaultSubstituteChar;
    }

    if (mSubstituteChar == substituteChar) {
        return;
    }

    mSubstituteChar = substituteChar;

    Q_EMIT dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

void StatisticTableModel::setUndefinedChar(QChar undefinedChar)
{
    if (undefinedChar.isNull()) {
        undefinedChar = DefaultUndefinedChar;
    }

    if (mUndefinedChar == undefinedChar) {
        return;
    }

    mUndefinedChar = undefinedChar;

    Q_EMIT dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

void StatisticTableModel::setValueCoding(int valueCoding)
{
    // no changes?
    if (mValueCoding == valueCoding) {
        return;
    }

    mValueCoding = (Okteta::ValueCoding)valueCoding;
    mValueCodec = Okteta::ValueCodec::createCodec(mValueCoding);
//     CodedByte.resize( ByteCodec->encodingWidth() );

    Q_EMIT dataChanged(index(0, ValueId), index(ByteSetSize - 1, ValueId));
    Q_EMIT headerChanged();
}

void StatisticTableModel::setCharCodec(const QString& codecName)
{
    if (codecName == mCharCodec->name()) {
        return;
    }

    mCharCodec = Okteta::CharCodec::createCodec(codecName);

    Q_EMIT dataChanged(index(0, CharacterId), index(ByteSetSize - 1, CharacterId));
}

int StatisticTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? ByteSetSize : 0;
}

int StatisticTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfIds : 0;
}

QVariant StatisticTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if ((role == Qt::DisplayRole) ||
        (role == SortRole)) {
        const unsigned char byte = index.row();
        const int column = index.column();
        switch (column)
        {
        case CharacterId:
        {
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            result =
                decodedChar.isUndefined() ?
                    QString(mUndefinedChar) :
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
            if (role == SortRole) {
                result =  (mSize == -1) ?
                        QVariant() :
                        QVariant(mByteCount[byte]);
            } else {
                result = (mSize > 0) ?
                        QVariant(100.0 * (double)mByteCount[byte] / mSize) :
                        QVariant();
            }
            break;
        default:
            ;
        }
    } else if (role == Qt::ToolTipRole) {
        const int column = index.column();
        if (column == CharacterId) {
            const unsigned char byte = index.row();
            const Okteta::Character decodedChar = mCharCodec->decode(byte);
            if (decodedChar.isUndefined()) {
                result = i18nc("@info:tooltip", "Character not defined.");
            }
        }
    } else if (role == Qt::TextAlignmentRole) {
        result = static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
    } else if (role == Qt::FontRole) {
        result = mFixedFont;
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
        const QString title =
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
        result = title;
    } else if (role == Qt::ToolTipRole) {
        const QString title =
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
        result = title;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

}

#include "moc_statistictablemodel.cpp"
