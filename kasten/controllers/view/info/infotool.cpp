/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infotool.hpp"

// controller
#include "statistictablemodel.hpp"
#include "createstatisticjob.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>

namespace Kasten {

InfoTool::InfoTool()
    : mStatisticTableModel(new StatisticTableModel(mByteCount, this))
{
    setObjectName(QStringLiteral("Info"));
    updateStatistic();
}

InfoTool::~InfoTool() = default;

QString InfoTool::title() const { return i18nc("@title:window", "Statistics"); }
StatisticTableModel* InfoTool::statisticTableModel() const { return mStatisticTableModel; }
int InfoTool::size() const { return (mByteArrayModel != nullptr) ? mByteArrayModel->size() : -1; }
bool InfoTool::isApplyable() const
{
    return (mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData() && !isStatisticUptodate());
}
bool InfoTool::isStatisticUptodate() const
{
    return (mSourceByteArrayModelUptodate
            && mSourceByteArrayModel == mByteArrayModel
            && mByteArrayView && mSourceSelection == mByteArrayView->selection());
}

void InfoTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(mStatisticTableModel);
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayView && mByteArrayModel) {
        mStatisticTableModel->setCharCodec(mByteArrayView->charCodingName());
        mStatisticTableModel->setValueCoding(mByteArrayView->valueCoding());
        mStatisticTableModel->setSubstituteChar(mByteArrayView->substituteChar());
        mStatisticTableModel->setUndefinedChar(mByteArrayView->undefinedChar());
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                mStatisticTableModel, &StatisticTableModel::setCharCodec);
        connect(mByteArrayView,  &ByteArrayView::valueCodingChanged,
                mStatisticTableModel, &StatisticTableModel::setValueCoding);
        connect(mByteArrayView,  &ByteArrayView::substituteCharChanged,
                mStatisticTableModel, &StatisticTableModel::setSubstituteChar);
        connect(mByteArrayView,  &ByteArrayView::undefinedCharChanged,
                mStatisticTableModel, &StatisticTableModel::setUndefinedChar);

        connect(mByteArrayView,  &ByteArrayView::selectedDataChanged,
                this, &InfoTool::onSelectionChanged);
    } else {
        // TODO: set based on default view profile, also char codec
        mStatisticTableModel->setSubstituteChar(QChar());
        mStatisticTableModel->setUndefinedChar(QChar());
    }

    emit statisticDirty(!isStatisticUptodate());
    emit isApplyableChanged(isApplyable());
}

void InfoTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    emit statisticDirty(!isStatisticUptodate());
    emit isApplyableChanged(isApplyable());
}

void InfoTool::onSourceChanged()
{
    mSourceByteArrayModelUptodate = false;
    emit statisticDirty(true);
    emit isApplyableChanged(isApplyable());
}

void InfoTool::onSourceDestroyed()
{
    mSourceByteArrayModel = nullptr;
    onSourceChanged();
}

void InfoTool::updateStatistic()
{
    // forget old string source
    if (mSourceByteArrayModel) {
        mSourceByteArrayModel->disconnect(this);
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    const Okteta::AddressRange selection = (mByteArrayView ? mByteArrayView->selection() : Okteta::AddressRange());
    auto* createStatisticJob = new CreateStatisticJob(mByteArrayModel, selection, mByteCount);
    const int selectionSize = createStatisticJob->exec();

    QApplication::restoreOverrideCursor();

    mStatisticTableModel->update(selectionSize);

    // remember new string source
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = selection;
    if (mSourceByteArrayModel) {
        connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &InfoTool::onSourceChanged);
        connect(mSourceByteArrayModel,  &Okteta::AbstractByteArrayModel::destroyed,
                this, &InfoTool::onSourceDestroyed);
    }

    mSourceByteArrayModelUptodate = true;
    emit statisticDirty(false);
    emit isApplyableChanged(false);

    if (mByteArrayView) {
        mByteArrayView->setFocus();
    }
}

}
