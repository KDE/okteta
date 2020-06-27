/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INFOTOOL_HPP
#define KASTEN_INFOTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Okteta core
#include <Okteta/AddressRange>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

class StatisticTableModel;

class ByteArrayView;

/**
 */
class InfoTool : public AbstractTool
{
    Q_OBJECT

public:
    InfoTool();
    ~InfoTool() override;

public:
    StatisticTableModel* statisticTableModel() const;
    int size() const;
    bool isApplyable() const;
    bool isStatisticUptodate() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public Q_SLOTS:
    void updateStatistic();

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);
    void statisticDirty(bool dirty);

private Q_SLOTS:
    void onSelectionChanged();
    void onSourceChanged();
    void onSourceDestroyed();

private:
    int mByteCount[256]; // TODO: here or in statistic model?

    StatisticTableModel* mStatisticTableModel;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    //
    bool mSourceByteArrayModelUptodate = false;
    // selection source
    Okteta::AddressRange mSourceSelection;
    // source of strings
    Okteta::AbstractByteArrayModel* mSourceByteArrayModel = nullptr;
};

}

#endif
