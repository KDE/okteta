/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "cropcontroller.hpp"

// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayView>
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ChangesDescribable>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
// Qt
#include <QAction>


namespace Kasten {

// TODO: look into making in generic, remove everything but selected
CropController::CropController(KXMLGUIClient* guiClient)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    m_cropAction = new QAction(QIcon::fromTheme(QStringLiteral("transform-crop")),
                               i18nc("@action:inmenu", "Crop"), this);
    m_cropAction->setObjectName(QStringLiteral("edit_crop"));
    connect(m_cropAction, &QAction::triggered, this, &CropController::crop);
    actionCollection->setDefaultShortcut(m_cropAction, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_C));
    m_cropAction->setEnabled(false);

    actionCollection->addAction(m_cropAction->objectName(), m_cropAction);
}

void CropController::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView =  model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == m_byteArrayView) {
        return;
    }

    if (m_byteArrayView) {
        m_byteArrayView->disconnect(this);
    }
    if (m_byteArrayModel) {
        m_byteArrayModel->disconnect(this);
    }

    m_byteArrayView = byteArrayView;

    // TODO; check if already same document used before
    ByteArrayDocument* document =
        m_byteArrayView ? qobject_cast<ByteArrayDocument*>(m_byteArrayView->baseModel()) : nullptr;
    m_byteArrayModel = document ? document->content() : nullptr;

    if (m_byteArrayView && m_byteArrayModel) {
        connect(m_byteArrayView, &ByteArrayView::readOnlyChanged,
                this, &CropController::updateActionEnabled);
        connect(m_byteArrayView, &ByteArrayView::overwriteModeChanged,
                this, &CropController::updateActionEnabled);
        connect(m_byteArrayView, &ByteArrayView::hasSelectedDataChanged,
                this, &CropController::updateActionEnabled);
    }

    updateActionEnabled();
}

void CropController::updateActionEnabled()
{
    const bool isApplyable = (
        m_byteArrayModel &&
        m_byteArrayView &&
        !m_byteArrayView->isReadOnly() &&
        !m_byteArrayView->isOverwriteMode() &&
        m_byteArrayView->hasSelectedData());

    m_cropAction->setEnabled(isApplyable);
}

void CropController::crop()
{
    auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(m_byteArrayModel);

    const Okteta::AddressRange keptSection = m_byteArrayView->selection();
    const Okteta::AddressRange beginSection(0, keptSection.nextBeforeStart());
    const Okteta::AddressRange endSection(keptSection.nextBehindEnd(), m_byteArrayModel->size() - 1);

    if (changesDescribable) {
        const QString changeDescription = i18nc("@item name of the change", "Crop");

        changesDescribable->openGroupedChange(changeDescription);
    }

    // first remove stuff at the end, so its values would not be messed up on removing the begin first
    if (endSection.isValid()) {
        std::ignore = m_byteArrayModel->removeBytes(endSection);
    }
    if (beginSection.isValid()) {
        std::ignore = m_byteArrayModel->removeBytes(beginSection);
    }

    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
}

}

#include "moc_cropcontroller.cpp"
