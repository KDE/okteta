/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacetool.hpp"

// controller
#include "replacejob.hpp"
// libconfigentries
#include <casesensitivityconfigentry.hpp>
// libbytearraysearch
#include <bytearraysearchutils.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>


namespace Kasten {

ReplaceTool::ReplaceTool()
{
    setObjectName(QStringLiteral("Replace"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mCaseSensitivity = configGroup.readEntry(CaseSensitivityConfigKey, DefaultCaseSensitivity);
    mDoPrompt = configGroup.readEntry(PromptConfigKey, DefaultDoPrompt);
}

ReplaceTool::~ReplaceTool() = default;

bool ReplaceTool::isApplyable() const
{
    return (mByteArrayView && mByteArrayModel && !mByteArrayView->isReadOnly()) && !mReplaceJob;
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayView && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString ReplaceTool::title() const { return i18nc("@title", "Replace"); }

AbstractModel* ReplaceTool::targetModel() const
{
    return mByteArrayView;
}

bool ReplaceTool::hasSelectedData()   const { return mByteArrayView->hasSelectedData(); }
QString ReplaceTool::charCodingName() const { return mByteArrayView->charCodingName(); }

void ReplaceTool::setTargetModel(AbstractModel* model)
{
    const bool oldIsApplyable = isApplyable();

    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        connect(mByteArrayView, &ByteArrayView::readOnlyChanged, this, &ReplaceTool::onReadOnlyChanged);
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        Q_EMIT isApplyableChanged(newIsApplyable);
    }
}

void ReplaceTool::setUserQueryAgent(QObject* userQueryAgent)
{
    mUserQueryAgent = userQueryAgent;
}

void ReplaceTool::setSearchData(const QByteArray& searchData)
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         Q_EMIT isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setReplaceData(const QByteArray& replaceData)
{
//     const bool oldIsApplyable = isApplyable();

    mReplaceData = replaceData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         Q_EMIT isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
    if (mCaseSensitivity == caseSensitivity) {
        return;
    }

//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(CaseSensitivityConfigKey, mCaseSensitivity);

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         Q_EMIT isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setDoPrompt(bool doPrompt)
{
    if (mDoPrompt == doPrompt) {
        return;
    }

    mDoPrompt = doPrompt;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(PromptConfigKey, mDoPrompt);
}

void ReplaceTool::replace(FindDirection direction, bool fromCursor, bool inSelection)
{
    Okteta::Address replaceRangeStartIndex;
    Okteta::Address replaceRangeEndIndex;

    if (!ByteArraySearchUtils::getSearchIndexes(&replaceRangeStartIndex, &replaceRangeEndIndex,
                                                mByteArrayModel,
                                                mByteArrayView->selection(),
                                                mByteArrayView->cursorPosition(),
                                                mSearchData,
                                                direction,
                                                fromCursor, inSelection)) {
        // no search doable, so skip any search and finish now
        Q_EMIT finished(false, 0);
        return;
    }

    mReplaceJob = std::make_unique<ReplaceJob>(mByteArrayView, mByteArrayModel, mUserQueryAgent, this);
    mReplaceJob->setSearchData(mSearchData);
    mReplaceJob->setReplaceData(mReplaceData);
    mReplaceJob->setCaseSensitivity(mCaseSensitivity);
    mReplaceJob->setDoPrompt(mDoPrompt);
    mReplaceJob->setRange(replaceRangeStartIndex, replaceRangeEndIndex, direction);
    connect(mReplaceJob.get(), &ReplaceJob::finished, this, &ReplaceTool::onJobFinished);

    Q_EMIT isApplyableChanged(isApplyable());

    mReplaceJob->start();
}

void ReplaceTool::onReadOnlyChanged(bool isReadOnly)
{
    Q_UNUSED(isReadOnly)

    // TODO: find out if isApplyable really changed, perhaps by caching the readonly state?
    Q_EMIT isApplyableChanged(isApplyable());
}

void ReplaceTool::onJobFinished(bool previousFound, int noOfReplacements)
{
    mReplaceJob.reset();

    Q_EMIT finished(previousFound, noOfReplacements);
    Q_EMIT isApplyableChanged(isApplyable());
}

}

#include "moc_replacetool.cpp"
