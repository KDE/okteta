/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERTOOL_HPP
#define KASTEN_FILTERTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Qt
#include <QVector>

class AbstractByteArrayFilterParameterSet;
class AbstractByteArrayFilter;

namespace Okteta {
class AbstractByteArrayModel;
}
class QString;

namespace Kasten {

class ByteArrayView;

class FilterTool : public AbstractTool
{
    Q_OBJECT

private:
    static constexpr char ConfigGroupId[] = "FilterTool";
    static constexpr char OperationConfigKey[] = "Operation";

public:
    FilterTool();
    ~FilterTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public:
    int filterId() const;
    bool hasWriteable() const;
    QVector<AbstractByteArrayFilter*> filterList() const;

public:
    QString charCodecName() const;
    AbstractByteArrayFilterParameterSet* parameterSet(int filterId);
    void saveParameterSet(int filterId);

public Q_SLOTS:
    void filter() const;
    void setFilter(int filterId);

Q_SIGNALS:
    void filterChanged(int filterId);
    void hasWriteableChanged(bool hasWriteable);
    void charCodecChanged(const QString& charCodecName);

private Q_SLOTS:
    void onApplyableChanged();

private:
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
    bool mHasWritable = false;

    QVector<AbstractByteArrayFilter*> mFilterList;

    int mFilterId = 0;
};

inline int FilterTool::filterId() const { return mFilterId; }

}

#endif
