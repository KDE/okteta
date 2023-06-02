/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacetool.hpp"

// controller
#include "replacejob.hpp"
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

// TODO: move to helper interface lib?
template <>
inline Qt::CaseSensitivity KConfigGroup::readEntry(const char *key, const Qt::CaseSensitivity &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Qt::CaseSensitivity caseSensitivity =
        (entry == QLatin1String("Sensitive")) ?   Qt::CaseSensitive :
        (entry == QLatin1String("Insensitive")) ? Qt::CaseInsensitive :
        /* else */                                defaultValue;
    return caseSensitivity;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Qt::CaseSensitivity &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Qt::CaseSensitive) ? QLatin1String("Sensitive") : QLatin1String("Insensitive");
    writeEntry(key, valueString, flags);
}

static constexpr Qt::CaseSensitivity DefaultCaseSensitivity = Qt::CaseInsensitive;
static constexpr bool DefaultDoPrompt = false;

static constexpr char ReplaceConfigGroupId[] = "ReplaceTool";

static constexpr char CaseSensitivityConfigKey[] = "CaseSensitivity";
static constexpr char PromptConfigKey[] = "Prompt";

namespace Kasten {

ReplaceTool::ReplaceTool()
{
    setObjectName(QStringLiteral("Replace"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ReplaceConfigGroupId);
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
        emit isApplyableChanged(newIsApplyable);
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
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setReplaceData(const QByteArray& replaceData)
{
//     const bool oldIsApplyable = isApplyable();

    mReplaceData = replaceData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
    if (mCaseSensitivity == caseSensitivity) {
        return;
    }

//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ReplaceConfigGroupId);
    configGroup.writeEntry(CaseSensitivityConfigKey, mCaseSensitivity);

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setDoPrompt(bool doPrompt)
{
    if (mDoPrompt == doPrompt) {
        return;
    }

    mDoPrompt = doPrompt;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ReplaceConfigGroupId);
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
        mReplaceJob = nullptr;
        emit finished(false, 0);
        return;
    }

    mReplaceJob = new ReplaceJob(mByteArrayView, mByteArrayModel, mUserQueryAgent, this);
    mReplaceJob->setSearchData(mSearchData);
    mReplaceJob->setReplaceData(mReplaceData);
    mReplaceJob->setCaseSensitivity(mCaseSensitivity);
    mReplaceJob->setDoPrompt(mDoPrompt);
    mReplaceJob->setRange(replaceRangeStartIndex, replaceRangeEndIndex, direction);
    connect(mReplaceJob, &ReplaceJob::finished, this, &ReplaceTool::onJobFinished);

    emit isApplyableChanged(isApplyable());

    mReplaceJob->start();
}

void ReplaceTool::onReadOnlyChanged(bool isReadOnly)
{
    Q_UNUSED(isReadOnly)

    // TODO: find out if isApplyable really changed, perhaps by caching the readonly state?
    emit isApplyableChanged(isApplyable());
}

void ReplaceTool::onJobFinished(bool previousFound, int noOfReplacements)
{
    delete mReplaceJob;
    mReplaceJob = nullptr;

    emit finished(previousFound, noOfReplacements);
    emit isApplyableChanged(isApplyable());
}

}
