/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchtool.hpp"

// libbytearraysearch
#include <bytearraysearchjob.hpp>
#include <bytearraysearchutils.hpp>
// controller
#include "searchuserqueryable.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QApplication>

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

static constexpr char SearchConfigGroupId[] = "SearchTool";

static constexpr char CaseSensitivityConfigKey[] = "CaseSensitivity";

namespace Kasten {

SearchTool::SearchTool()
{
    setObjectName(QStringLiteral("Search"));

    const KConfigGroup configGroup(KSharedConfig::openConfig(), SearchConfigGroupId);
    mCaseSensitivity = configGroup.readEntry(CaseSensitivityConfigKey, DefaultCaseSensitivity);
}

SearchTool::~SearchTool() = default;

bool SearchTool::isApplyable() const
{
    return (mByteArrayView && mByteArrayModel);
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayView && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString SearchTool::title() const { return i18nc("@title", "Search"); }

bool SearchTool::hasSelectedData()   const { return mByteArrayView->hasSelectedData(); }
QString SearchTool::charCodingName() const { return mByteArrayView->charCodingName(); }

void SearchTool::setTargetModel(AbstractModel* model)
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
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                this, &SearchTool::charCodecChanged);
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if (oldIsApplyable != newIsApplyable) {
        emit isApplyableChanged(newIsApplyable);
    }
}

void SearchTool::setUserQueryAgent(If::SearchUserQueryable* userQueryAgent)
{
    mUserQueryAgent = userQueryAgent;
}

void SearchTool::setSearchData(const QByteArray& searchData)
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
{
    if (mCaseSensitivity == caseSensitivity) {
        return;
    }

//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

    KConfigGroup configGroup(KSharedConfig::openConfig(), SearchConfigGroupId);
    configGroup.writeEntry(CaseSensitivityConfigKey, mCaseSensitivity);

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::search(FindDirection direction, bool fromCursor, bool inSelection)
{
    mPreviousFound = false;

    if (!ByteArraySearchUtils::getSearchIndexes(&mSearchFirstIndex, &mSearchLastIndex,
                                                mByteArrayModel,
                                                mByteArrayView->selection(),
                                                mByteArrayView->cursorPosition(),
                                                mSearchData,
                                                direction,
                                                fromCursor, inSelection)) {
        // no search doable, so skip any search and finish now
        emit dataNotFound();
        return;
    }

    doSearch(direction);
}

void SearchTool::doSearch(FindDirection direction)
{
    // TODO: should start at last
    Okteta::Address startIndex = (direction == FindForward) ? mSearchFirstIndex : mSearchLastIndex /*-mSearchData.size()*/;
    bool wrapEnabled = (direction == FindForward) ? (mSearchLastIndex < startIndex) : (startIndex < mSearchFirstIndex);

    while (true) {
        const bool isWithin = !wrapEnabled || (direction == FindForward) ? (startIndex <= mByteArrayModel->size() - 1) : (startIndex >= 0);

        if (isWithin) {
            QApplication::setOverrideCursor(Qt::WaitCursor);

            Okteta::Address endIndex = wrapEnabled ?
                                    ((direction == FindForward) ? mByteArrayModel->size() - 1 : 0) :
                                    ((direction == FindForward) ? mSearchLastIndex : mSearchFirstIndex);

            auto* searchJob =
                new ByteArraySearchJob(mByteArrayModel, mSearchData, startIndex, endIndex, mCaseSensitivity, mByteArrayView->charCodingName());
            const Okteta::AddressRange matchRange = searchJob->exec();

            QApplication::restoreOverrideCursor();

            if (matchRange.isValid()) {
                mPreviousFound = true;
                mByteArrayView->setSelection(matchRange.start(), matchRange.end());
                break;
            }
        }

        if (wrapEnabled) {
            const bool wrapping = mUserQueryAgent ? mUserQueryAgent->queryContinue(direction) : true;

            if (!wrapping) {
                break;
            }
            startIndex = (direction == FindForward) ? 0 : mByteArrayModel->size() - 1;
            wrapEnabled = false;
        } else {
            if (!mPreviousFound) {
                emit dataNotFound();
            }
            break;
        }
    }
    mByteArrayView->setFocus();
}

}
