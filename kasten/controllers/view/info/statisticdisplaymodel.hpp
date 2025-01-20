/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STATISTICDISPLAYMODEL_HPP
#define KASTEN_STATISTICDISPLAYMODEL_HPP

// Qt
#include <QIdentityProxyModel>

class QWidget;

namespace Kasten {

class StatisticDisplayModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    explicit StatisticDisplayModel(QWidget* uiWidget, QObject* parent = nullptr);

    ~StatisticDisplayModel() override;

public: // QAbstractItemModel API
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]]
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]]
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    [[nodiscard]]
    QStringList mimeTypes() const override;

private:
    QWidget* const m_uiWidget;
};

}

#endif
