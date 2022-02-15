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

static constexpr char FilterConfigGroupId[] = "FilterTool";
static constexpr char OperationConfigKey[] = "Operation";

namespace Kasten {

FilterTool::FilterTool()
{
    setObjectName(QStringLiteral("BinaryFilter"));

    mFilterList = ByteArrayFilterFactory::createFilters();

    const KConfigGroup configGroup(KSharedConfig::openConfig(), FilterConfigGroupId);
    for (auto* filter : qAsConst(mFilterList)) {
        filter->loadConfig(configGroup);
    }

    mFilterId = 0;
    const QString operationId = configGroup.readEntry(OperationConfigKey);
    if (!operationId.isEmpty()) {
        for (int i = 0; i < mFilterList.size(); ++i) {
            if (mFilterList[i]->id() == operationId) {
                mFilterId = i;
                break;
            }
        }
    }
}

FilterTool::~FilterTool()
{
    qDeleteAll(mFilterList);
}

QString FilterTool::title() const { return i18nc("@title:window", "Binary Filter"); }
QVector<AbstractByteArrayFilter*> FilterTool::filterList() const { return mFilterList; }
QString FilterTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->charCodingName() : QString();
}

bool FilterTool::hasWriteable() const { return mHasWritable; }

AbstractByteArrayFilterParameterSet* FilterTool::parameterSet(int filterId)
{
    AbstractByteArrayFilter* byteArrayFilter = mFilterList.at(filterId);

    return byteArrayFilter ? byteArrayFilter->parameterSet() : nullptr;
}

void FilterTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

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
    emit charCodecChanged(newCharCodecName);
}

void FilterTool::saveParameterSet(int filterId)
{
    AbstractByteArrayFilter* byteArrayFilter = mFilterList.at(filterId);

    if (!byteArrayFilter) {
        return;
    }

    KConfigGroup configGroup(KSharedConfig::openConfig(), FilterConfigGroupId);
    byteArrayFilter->saveConfig(configGroup);
}

void FilterTool::setFilter(int filterId)
{
    if (mFilterId == filterId) {
        return;
    }

    mFilterId = filterId;

    AbstractByteArrayFilter* byteArrayFilter = mFilterList.at(mFilterId);
    if (byteArrayFilter) {
        KConfigGroup configGroup(KSharedConfig::openConfig(), FilterConfigGroupId);
        configGroup.writeEntry(OperationConfigKey, byteArrayFilter->id());
    }

    emit filterChanged(mFilterId);
}

void FilterTool::filter() const
{
    AbstractByteArrayFilter* byteArrayFilter = mFilterList.at(mFilterId);

    if (byteArrayFilter) {
        const Okteta::AddressRange filteredSection = mByteArrayView->selection();

        QByteArray filterResult;
        filterResult.resize(filteredSection.width());

        QApplication::setOverrideCursor(Qt::WaitCursor);

        auto* filterJob = new FilterJob(byteArrayFilter, reinterpret_cast<Okteta::Byte*>(filterResult.data()), mByteArrayModel, filteredSection);
        const bool success = filterJob->exec();

        QApplication::restoreOverrideCursor();

        if (success) {
            Okteta::ChangesDescribable* changesDescribable =
                qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

            if (changesDescribable) {
                changesDescribable->openGroupedChange(byteArrayFilter->name());
            }
            mByteArrayModel->replace(filteredSection, filterResult);
            if (changesDescribable) {
                changesDescribable->closeGroupedChange();
            }
        }
    }

    mByteArrayView->setFocus();
}

void FilterTool::onApplyableChanged()
{
    const bool newHasWriteable = (mByteArrayModel && mByteArrayView
                                  && !mByteArrayView->isReadOnly() && mByteArrayView->hasSelectedData());
    if (newHasWriteable != mHasWritable) {
        mHasWritable = newHasWriteable;
        emit hasWriteableChanged(newHasWriteable);
    }
}

}
