/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STATISTICTABLEMODEL_HPP
#define KASTEN_STATISTICTABLEMODEL_HPP

// Okteta core
#include <Okteta/OktetaCore>
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

class StatisticTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        ValueId = 0,
        CharacterId = 1,
        CountId = 2,
        PercentId = 3,
        NoOfIds = 4 // TODO: what pattern is usually used to mark number of ids?
    };

    enum Roles {
        SortRole = Qt::UserRole,
    };

public:
    explicit StatisticTableModel(int* byteCount, QObject* parent = nullptr);
    ~StatisticTableModel() override;

public: // QAbstractTableModel API
    [[nodiscard]]
    int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public:
    void update(int size);

public Q_SLOTS:
    void setCharCodec(const QString& codecName);
    void setValueCoding(int valueCoding);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);

Q_SIGNALS:
    void headerChanged();
    void sizeChanged(int size);

public:
    [[nodiscard]]
    const QFont &fixedFont() const;

private:
    int mSize = -1;
    int* mByteCount;

    Okteta::ValueCoding mValueCoding;
    std::unique_ptr<const Okteta::ValueCodec> mValueCodec;
    std::unique_ptr<const Okteta::CharCodec> mCharCodec;
    QChar mSubstituteChar;
    QChar mUndefinedChar;
    QFont mFixedFont;
};

inline const QFont &StatisticTableModel::fixedFont() const
{
    return mFixedFont;
}

}

#endif
