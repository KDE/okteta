/*
    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumtool.hpp"

// lib
#include "checksumcalculatejob.hpp"
#include "checksumlogging.hpp"
//
#include <bytearraychecksumalgorithmfactory.hpp>
#include <abstractbytearraychecksumalgorithm.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QApplication>


namespace Kasten {

ChecksumTool::ChecksumTool()
    : mChecksumUptodate(false)
    , mSourceByteArrayModelUptodate(false)
{
    setObjectName(QStringLiteral("Checksum"));

    mAlgorithmList = ByteArrayChecksumAlgorithmFactory::createAlgorithms();

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    for (auto& algorithm : mAlgorithmList) {
        algorithm->loadConfig(configGroup);
    }

    mAlgorithmId = 0;
    const QString algorithmId = configGroup.readEntry(AlgorithmConfigKey);
    if (!algorithmId.isEmpty()) {
        for (std::size_t i = 0; i < mAlgorithmList.size(); ++i) {
            if (mAlgorithmList[i]->id() == algorithmId) {
                mAlgorithmId = i;
                break;
            }
        }
    }
}

ChecksumTool::~ChecksumTool() = default;

const std::vector<std::unique_ptr<AbstractByteArrayChecksumAlgorithm>>& ChecksumTool::algorithmList() const
{
    return mAlgorithmList;
}

bool ChecksumTool::isApplyable() const
{
    return (mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData());
}

QString ChecksumTool::title() const { return i18nc("@title:window of the tool to calculate checksums", "Checksum"); }

AbstractByteArrayChecksumParameterSet* ChecksumTool::parameterSet()
{
    if ((mAlgorithmId < 0) || (static_cast<int>(mAlgorithmList.size()) <= mAlgorithmId)) {
        return nullptr;
    }

    return mAlgorithmList[mAlgorithmId]->parameterSet();
}

void ChecksumTool::setTargetModel(AbstractModel* model)
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

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                this, &ChecksumTool::onSelectionChanged);
    }

    // TODO: if there is no view, there is nothing calculate a checksum from
    // or this could be the view where we did the checksum from and it did not change
    checkUptoDate();
    Q_EMIT uptodateChanged(mChecksumUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

void ChecksumTool::checkUptoDate()
{
    mChecksumUptodate =
        (mSourceByteArrayModel == mByteArrayModel
         && mByteArrayView && mSourceSelection == mByteArrayView->selection()
         && mSourceAlgorithmId == mAlgorithmId
         && mSourceByteArrayModelUptodate);
}

void ChecksumTool::calculateChecksum()
{
    if ((mAlgorithmId < 0) || (static_cast<int>(mAlgorithmList.size()) <= mAlgorithmId)) {
        return;
    }

    // forget old string source
    if (mSourceByteArrayModel) {
        mSourceByteArrayModel->disconnect(this);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* checksumCalculateJob =
        new ChecksumCalculateJob(&mCheckSum, mAlgorithmList[mAlgorithmId].get(), mByteArrayModel, mByteArrayView->selection());
    checksumCalculateJob->exec();

    QApplication::restoreOverrideCursor();

    // remember checksum source
    mSourceAlgorithmId = mAlgorithmId;
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = mByteArrayView->selection();
    connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::contentsChanged,
            this, &ChecksumTool::onSourceChanged);
    connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::destroyed,
            this, &ChecksumTool::onSourceDestroyed);

    mChecksumUptodate = true;
    mSourceByteArrayModelUptodate = true;
    Q_EMIT checksumChanged(mCheckSum);
    Q_EMIT uptodateChanged(true);
}

void ChecksumTool::setAlgorithm(int algorithmId)
{
    if ((algorithmId < 0) || (static_cast<int>(mAlgorithmList.size()) <= algorithmId)) {
        return;
    }

    if (mAlgorithmId == algorithmId) {
        return;
    }

    mAlgorithmId = algorithmId;
    checkUptoDate();

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(AlgorithmConfigKey, mAlgorithmList[mAlgorithmId]->id());

    Q_EMIT algorithmChanged(mAlgorithmId);
    Q_EMIT uptodateChanged(mChecksumUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

// TODO: hack!
// better would be to store the parameter set used for the source and compare if equal
// this hack does the same, except for that the source will never be up-to-date
void ChecksumTool::resetSourceTool()
{
    mSourceAlgorithmId = -1;

    if ((0 <= mAlgorithmId) && (mAlgorithmId < static_cast<int>(mAlgorithmList.size()))) {
        KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
        mAlgorithmList[mAlgorithmId]->saveConfig(configGroup);
    }

    checkUptoDate();
    Q_EMIT uptodateChanged(mChecksumUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

void ChecksumTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    checkUptoDate();
    Q_EMIT uptodateChanged(mChecksumUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

void ChecksumTool::onSourceChanged()
{
    mChecksumUptodate = false;
    mSourceByteArrayModelUptodate = false;
    Q_EMIT uptodateChanged(false);
}

void ChecksumTool::onSourceDestroyed()
{
    mSourceByteArrayModel = nullptr;
    onSourceChanged();
}

}

#include "moc_checksumtool.cpp"
