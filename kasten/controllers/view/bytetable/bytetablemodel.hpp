/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTETABLEMODEL_HPP
#define KASTEN_BYTETABLEMODEL_HPP

// Qt
#include <QAbstractTableModel>
#include <QFont>
// Std
#include <memory>

namespace Okteta {
class CharCodec;
class ValueCodec;
}

namespace Kasten {

class ByteTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        DecimalId = 0,
        HexadecimalId = 1,
        OctalId = 2,
        BinaryId = 3,
        CharacterId = 4,
        NoOfIds = 5 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    explicit ByteTableModel(QObject* parent = nullptr);
    ~ByteTableModel() override;

public: // QAbstractTableModel API
    [[nodiscard]]
    int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public Q_SLOTS:
    void setCharCodec(const QString& codecName);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);

public:
    [[nodiscard]]
    const QFont &fixedFont() const;

private:
    static constexpr int NofOfValueCodings = 4;

private:
    std::unique_ptr<const Okteta::ValueCodec> mValueCodec[NofOfValueCodings];
    std::unique_ptr<const Okteta::CharCodec> mCharCodec;
    QChar mSubstituteChar;
    QChar mUndefinedChar;
    QFont mFixedFont;
};

inline const QFont &ByteTableModel::fixedFont() const
{
    return mFixedFont;
}

}

#endif
