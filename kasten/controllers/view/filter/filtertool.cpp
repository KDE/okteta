/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filtertool.hpp"

// tool
#include "filterjob.hpp"
// filter
#include <bytearrayfilterfactory.hpp>
#include <abstractbytearrayfilter.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserNotification>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ChangesDescribable>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QByteArray>


namespace Kasten {

FilterTool::FilterTool(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
    setObjectName(QStringLiteral("BinaryFilter"));

    mFilterList = ByteArrayFilterFactory::createFilters();

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    for (const auto& filter : mFilterList) {
        filter->loadConfig(configGroup);
    }

    mFilterId = 0;
    const QString operationId = configGroup.readEntry(OperationConfigKey);
    if (!operationId.isEmpty()) {
        for (std::size_t i = 0; i < mFilterList.size(); ++i) {
            if (mFilterList[i]->id() == operationId) {
                mFilterId = i;
                break;
            }
        }
    }
}

FilterTool::~FilterTool() = default;

QString FilterTool::title() const { return i18nc("@title:window", "Binary Filter"); }
const std::vector<std::unique_ptr<AbstractByteArrayFilter>>& FilterTool::filterList() const { return mFilterList; }
QString FilterTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->charCodingName() : QString();
}

bool FilterTool::hasWriteable() const { return mHasWritable; }

AbstractByteArrayFilterParameterSet* FilterTool::parameterSet(int filterId)
{
    if ((filterId < 0) || (static_cast<int>(mFilterList.size()) <= filterId)) {
        return nullptr;
    }

    return mFilterList[filterId]->parameterSet();
}

void FilterTool::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = byteArrayView;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    const bool hasByteArray = (mByteArrayModel && mByteArrayView);
    QString newCharCodecName;
    if (hasByteArray) {
        newCharCodecName = mByteArrayView->charCodingName();
        connect(mByteArrayView, &ByteArrayView::hasSelectedDataChanged, this, &FilterTool::onApplyableChanged);
        connect(mByteArrayView, &ByteArrayView::readOnlyChanged,        this, &FilterTool::onApplyableChanged);
        connect(mByteArrayView, &ByteArrayView::charCodecChanged,
                this, &FilterTool::charCodecChanged);
    }

    onApplyableChanged();
    Q_EMIT charCodecChanged(newCharCodecName);
}

void FilterTool::saveParameterSet(int filterId)
{
    if ((filterId < 0) || (static_cast<int>(mFilterList.size()) <= filterId)) {
        return;;
    }

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mFilterList[filterId]->saveConfig(configGroup);
}

void FilterTool::setFilter(int filterId)
{
    if (mFilterId == filterId) {
        return;
    }

    if ((filterId < 0) || (static_cast<int>(mFilterList.size()) <= filterId)) {
        return;;
    }

    mFilterId = filterId;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(OperationConfigKey, mFilterList[mFilterId]->id());

    Q_EMIT filterChanged(mFilterId);
}

void FilterTool::filter() const
{
    if ((mFilterId < 0) || (static_cast<int>(mFilterList.size()) <= mFilterId)) {
        return;;
    }

    const auto& byteArrayFilter = mFilterList[mFilterId];

    const Okteta::AddressRange filteredSection = mByteArrayView->selection();

    QByteArray filterResult;
    filterResult.resize(filteredSection.width());

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* filterJob = new FilterJob(byteArrayFilter.get(), reinterpret_cast<Okteta::Byte*>(filterResult.data()), mByteArrayModel, filteredSection);
    const bool success = filterJob->exec();

    QApplication::restoreOverrideCursor();

    if (success) {
        auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

        if (changesDescribable) {
            changesDescribable->openGroupedChange(byteArrayFilter->name());
        }
        std::ignore = mByteArrayModel->replace(filteredSection, filterResult);
        if (changesDescribable) {
            changesDescribable->closeGroupedChange();
        }
    }

    const QString notificationTitle = title();
    const QString notificationText = i18ncp("@info", "1 byte filtered with %2.", "%1 bytes filtered with %2.", filterResult.size(), byteArrayFilter->name());
    auto message = std::make_unique<Kasten::UserNotification>(mByteArrayView, notificationText, notificationTitle);
    m_userMessagesHandler->postNotification(std::move(message));

    mByteArrayView->setFocus();
}

void FilterTool::onApplyableChanged()
{
    const bool newHasWriteable = (mByteArrayModel && mByteArrayView
                                  && !mByteArrayView->isReadOnly() && mByteArrayView->hasSelectedData());
    if (newHasWriteable != mHasWritable) {
        mHasWritable = newHasWriteable;
        Q_EMIT hasWriteableChanged(newHasWriteable);
    }
}

}

#include "moc_filtertool.cpp"
