/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

// QCA
// need to have this first, as QCA needs QT_NO_CAST_FROM_ASCII disabled when included
#include <config-qca2.hpp> // krazy:excludeall=includes
#if HAVE_QCA2
// disable QT_NO_CAST_FROM_ASCII
#ifdef QT_NO_CAST_FROM_ASCII
#undef QT_NO_CAST_FROM_ASCII
#endif
#include <QtCrypto>
#endif

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

static constexpr char ChecksumConfigGroupId[] = "ChecksumTool";
static constexpr char AlgorithmConfigKey[] = "Algorithm";

namespace Kasten {

ChecksumTool::ChecksumTool()
    : mChecksumUptodate(false)
    , mSourceByteArrayModelUptodate(false)
{
    setObjectName(QStringLiteral("Checksum"));

// TODO: find a better place to do and store the initialization
#if HAVE_QCA2
    mQcaInitializer = new QCA::Initializer(QCA::Practical, 64);
    qCDebug(LOG_OKTETA_KASTEN_CONTROLLER_CHECKSUM) << QCA::supportedFeatures();// Hash::supportedTypes();
#endif

    mAlgorithmList = ByteArrayChecksumAlgorithmFactory::createAlgorithms();

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ChecksumConfigGroupId);
    for (auto *algorithm : std::as_const(mAlgorithmList)) {
        algorithm->loadConfig(configGroup);
    }

    mAlgorithmId = 0;
    const QString algorithmId = configGroup.readEntry(AlgorithmConfigKey);
    if (!algorithmId.isEmpty()) {
        for (int i = 0; i < mAlgorithmList.size(); ++i) {
            if (mAlgorithmList[i]->id() == algorithmId) {
                mAlgorithmId = i;
                break;
            }
        }
    }
}

ChecksumTool::~ChecksumTool()
{
    qDeleteAll(mAlgorithmList);
#if HAVE_QCA2
    delete mQcaInitializer;
#endif
}

QVector<AbstractByteArrayChecksumAlgorithm*> ChecksumTool::algorithmList() const { return mAlgorithmList; }

bool ChecksumTool::isApplyable() const
{
    return (mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData());
}

QString ChecksumTool::title() const { return i18nc("@title:window of the tool to calculate checksums", "Checksum"); }

AbstractByteArrayChecksumParameterSet* ChecksumTool::parameterSet()
{
    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at(mAlgorithmId);

    return algorithm ? algorithm->parameterSet() : nullptr;
}

void ChecksumTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

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
    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at(mAlgorithmId);

    if (algorithm) {
        // forget old string source
        if (mSourceByteArrayModel) {
            mSourceByteArrayModel->disconnect(this);
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);

        auto* checksumCalculateJob =
            new ChecksumCalculateJob(&mCheckSum, algorithm, mByteArrayModel, mByteArrayView->selection());
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
}

void ChecksumTool::setAlgorithm(int algorithmId)
{
    if (mAlgorithmId == algorithmId) {
        return;
    }

    mAlgorithmId = algorithmId;
    checkUptoDate();

    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at(mAlgorithmId);
    if (algorithm) {
        KConfigGroup configGroup(KSharedConfig::openConfig(), ChecksumConfigGroupId);
        configGroup.writeEntry(AlgorithmConfigKey, algorithm->id());
    }

    Q_EMIT algorithmChanged(mAlgorithmId);
    Q_EMIT uptodateChanged(mChecksumUptodate);
    Q_EMIT isApplyableChanged(isApplyable());
}

// TODO: hack!
// better would be to store the parameter set used for the source and compare if equal
// this hack does the same, except for that the source will never be uptodate
void ChecksumTool::resetSourceTool()
{
    mSourceAlgorithmId = -1;

    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at(mAlgorithmId);
    if (algorithm) {
        KConfigGroup configGroup(KSharedConfig::openConfig(), ChecksumConfigGroupId);
        algorithm->saveConfig(configGroup);
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
