/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODDISPLAYMODEL_HPP
#define KASTEN_PODDISPLAYMODEL_HPP

// libdatatypedisplaystringifiers
#include <datatypedisplaystringifiermanager.hpp>
// Qt
#include <QIdentityProxyModel>

class QWidget;

namespace Kasten {

class PODDecoderTool;

class PODDisplayModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    explicit PODDisplayModel(QWidget* uiWidget, PODDecoderTool* tool, QObject* parent = nullptr);

    ~PODDisplayModel() override;

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
    PODDecoderTool* const m_tool;

    const Okteta::DataTypeDisplayStringifierManager m_dataTypeDisplayStringifierManager;
};

}

#endif
