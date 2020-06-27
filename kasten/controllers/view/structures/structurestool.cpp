/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestool.hpp"
#include "structuredefinitionfile.hpp"
#include "structuresmanager.hpp"
#include "structlogging.hpp"
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QModelIndex>

#include "script/scripthandler.hpp"
#include "datatypes/datainformation.hpp"
#include "structureviewpreferences.h"

namespace Kasten {

StructuresTool::StructuresTool()
    : mByteOrder(StructureViewPreferences::byteOrder())
    , mManager(new StructuresManager(this))
    , mWritingData(false)
    , mCurrentItemDataChanged(false)
{
    // leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName(QStringLiteral("StructuresTool"));
    mManager->reloadPaths();
    setSelectedStructuresInView();
    //	mUtf8Codec = QTextCodec::codecForName("UTF-8");

    connect(this, &StructuresTool::byteOrderChanged, this, &StructuresTool::onByteOrderChanged);
}

StructuresTool::~StructuresTool() = default;

void StructuresTool::onByteOrderChanged()
{
    updateData(Okteta::ArrayChangeMetricsList());
}

void StructuresTool::setByteOrder(QSysInfo::Endian order)
{
    if (order != StructureViewPreferences::byteOrder() || order != mByteOrder) {
        emit byteOrderChanged();
        StructureViewPreferences::setByteOrder(order);
        mByteOrder = order;
        updateData(Okteta::ArrayChangeMetricsList());
    }
}

QString StructuresTool::title() const
{
    return i18nc("@title:window", "Structures");
}

void StructuresTool::setTargetModel(AbstractModel* model)
{
    // just a copy of the code in poddecodertool.h
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    ByteArrayDocument* document = mByteArrayView ?
                                  qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    if (mByteArrayModel && mByteArrayView) {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect(mByteArrayView, &ByteArrayView::cursorPositionChanged,
                this, &StructuresTool::onCursorPositionChange);
        connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &StructuresTool::onContentsChange);
    }
    emit byteArrayModelChanged(mByteArrayModel);
    updateData(Okteta::ArrayChangeMetricsList());
}

void StructuresTool::onCursorPositionChange(Okteta::Address pos)
{
    if (mCursorIndex != pos) {
        mCursorIndex = pos;
        updateData(Okteta::ArrayChangeMetricsList());
        emit cursorIndexChanged();
    }
}

void StructuresTool::setByteOrder(int order)
{
    if (order == QSysInfo::LittleEndian) {
        setByteOrder(QSysInfo::LittleEndian);
    } else if (order == QSysInfo::BigEndian) {
        setByteOrder(QSysInfo::BigEndian);
    } else {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "invalid byte order set:" << order;
    }
}

void StructuresTool::onContentsChange(const Okteta::ArrayChangeMetricsList& list)
{
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "contents changed";
    for (int i = 0; i < list.size(); ++i) {
        const Okteta::ArrayChangeMetrics& acm = list.at(i);
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "change: t=" << acm.type() << "o=" << acm.offset() << "a2=" << acm.removeLength() << "a3=" << acm.insertLength();
    }

    updateData(list);
}

bool StructuresTool::setData(const QVariant& value, int role, DataInformation* item, uint row)
{
    Q_UNUSED(row)
    if (!mByteArrayModel) {
        return false;
    }
    if (role != Qt::EditRole) {
        return false;
    }
    Q_CHECK_PTR(item);

    TopLevelDataInformation* topLevel = item->topLevelDataInformation();
    const Okteta::Address structureStart = startAddress(topLevel);
    mWritingData = true;
    bool ret = false;
    BitCount64 position = item->positionInFile(structureStart);
    const quint64 remainingBits = qMax(mByteArrayModel->size() * 8 - qint64(position), qint64(0));
    quint8 bitOffs = position % 8;
    ret = item->setData(value, mByteArrayModel, Okteta::Address(position / 8), remainingBits, bitOffs);
    mWritingData = false; // finished
    // XXX: this is inefficient, best would be to only update the changed value
    updateData(Okteta::ArrayChangeMetricsList()); // update once after writing
    return ret;
}

void StructuresTool::updateData(const Okteta::ArrayChangeMetricsList& list)
{
    if (mWritingData) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "currently writing data, won't update";
        return;
    }
    if (!mByteArrayModel) {
        return;
    }

    for (int i = 0; i < mData.size(); i++) {
        const TopLevelDataInformation::Ptr& dat = mData.at(i);
        dat->read(mByteArrayModel, mCursorIndex, list, false);
        if (mCurrentItemDataChanged) {
            emit dataChanged(i, mData.at(i)->actualDataInformation());
        }
        mCurrentItemDataChanged = false;
    }
}

// model interface:
QVariant StructuresTool::headerData(int column, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (column)
        {
        case DataInformation::ColumnName:
            return i18nc("name of a data structure", "Name");
        case DataInformation::ColumnType:
            return i18nc("type of a data structure", "Type");
        case DataInformation::ColumnValue:
            return i18nc("value of a data structure (primitive type)", "Value");
        default:
            return {};
        }
    }
    return {};
}

int StructuresTool::childCount() const
{
    return mData.size();
}

DataInformation* StructuresTool::childAt(int idx) const
{
    if (idx >= mData.size() || idx < 0) {
        return nullptr;
    }
    // don't expose the topLevelDataInformation, since this may cause crashes
    return mData.at(idx)->actualDataInformation();
}

void StructuresTool::addChildItem(TopLevelDataInformation* child)
{
    Q_CHECK_PTR(child);
    child->setParent(this);
    if (child->isValid()) {
        child->setIndex(mData.size());
        connect(child, &TopLevelDataInformation::dataChanged, this, &StructuresTool::onChildItemDataChanged);
        connect(child, &TopLevelDataInformation::childrenAboutToBeInserted,
                this, &StructuresTool::childrenAboutToBeInserted);
        connect(child, &TopLevelDataInformation::childrenInserted,
                this, &StructuresTool::childrenInserted);
        connect(child, &TopLevelDataInformation::childrenAboutToBeRemoved,
                this, &StructuresTool::childrenAboutToBeRemoved);
        connect(child, &TopLevelDataInformation::childrenRemoved,
                this, &StructuresTool::childrenRemoved);
        connect(this, &StructuresTool::byteArrayModelChanged,
                child, &TopLevelDataInformation::newModelActivated);
        mData.append(QSharedPointer<TopLevelDataInformation>(child));
        // ensure that locking gets set up properly
        if (mByteArrayModel) {
            child->newModelActivated(mByteArrayModel);
        }
    } else {
        child->setIndex(mInvalidData.size());
        mInvalidData.append(QSharedPointer<TopLevelDataInformation>(child));
    }
}

void StructuresTool::setSelectedStructuresInView()
{
    mData.clear();
    mInvalidData.clear();
    emit dataCleared();

    QRegExp regex(QStringLiteral("'(.+)':'(.+)'"));
    QStringList loadedStructs = StructureViewPreferences::loadedStructures();
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "loadedStructs " << loadedStructs;
    for (int i = 0; i < loadedStructs.size(); ++i) {
        const QString& s = loadedStructs.at(i);
        int pos = regex.indexIn(s);
        if (pos > -1) {
            QString pluginName = regex.cap(1);
            QString name = regex.cap(2);
            // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "pluginName=" << path << " structureName=" << name;
            StructureDefinitionFile* def = mManager->definition(pluginName);
            if (!def) {
                continue;
            }
            if (!def->isValid()) {
                continue;
            }
            // should be valid now
            if (name == QLatin1String("*")) {
                // add all of them
                const QVector<TopLevelDataInformation*> structs = def->structures();
                for (auto* structure : structs) {
                    addChildItem(structure);
                }
            } else {
                TopLevelDataInformation* data = def->structure(name);
                if (data) {
                    addChildItem(data);
                } else {
                    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Could not find structure with name" << name << "in" << pluginName;
                }
            }
        }
    }

    for (int i = 0; i < mData.count(); ++i) {
        emit dataChanged(i, mData.at(i)->actualDataInformation());
    }

    updateData(Okteta::ArrayChangeMetricsList());

}

Okteta::Address StructuresTool::startAddress(const TopLevelDataInformation* data) const
{
    if (data->isLockedFor(mByteArrayModel)) {
        return Okteta::Address(data->lockPositionFor(mByteArrayModel));
    }

    return mCursorIndex;
}

void StructuresTool::mark(const QModelIndex& idx)
{
    if (!mByteArrayModel || !mByteArrayView || !idx.isValid()) {
        return;
    }
    const auto* data = static_cast<const DataInformation*>(idx.internalPointer());
    if (!data) {
        return;
    }
    Q_CHECK_PTR(data->topLevelDataInformation());
    const Okteta::Address baseAddress = startAddress(data->topLevelDataInformation());
    // FIXME support marking of partial bytes
    int length = data->size() / 8;
    const int maxLen = mByteArrayModel->size() - baseAddress;
    length = qMin(length, maxLen);
    const Okteta::Address startOffset = Okteta::Address(data->positionInFile(baseAddress) / 8);
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(startOffset, length);
    mByteArrayView->setMarking(markingRange, true);
}

void StructuresTool::unmark(/*const QModelIndex& idx*/)
{
    if (mByteArrayView) {
        mByteArrayView->setMarking(Okteta::AddressRange());
    }
}

void StructuresTool::validateAllStructures()
{
    if (!mByteArrayModel) {
        return; // no point validating
    }
    const int size = mData.size();
    for (int i = 0; i < size; ++i) {
        mData.at(i)->validate();
    }
}

bool StructuresTool::isFileLoaded() const
{
    return (mByteArrayModel != nullptr);
}

void StructuresTool::lockStructure(const QModelIndex& idx)
{
    if (!mByteArrayModel) { // no point without ByteArrayModel
        return;
    }
    if (!idx.isValid() || !idx.internalPointer()) {
        return;
    }
    auto* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top) {
        top->lockPositionToOffset(mCursorIndex, mByteArrayModel);
    }
}

void StructuresTool::unlockStructure(const QModelIndex& idx)
{
    if (!mByteArrayModel) { // no point without ByteArrayModel
        return;
    }
    if (!idx.isValid() || !idx.internalPointer()) {
        return;
    }
    auto* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);

    unmark();
    top->unlockPosition(mByteArrayModel);
    // now read from the current position:
    top->read(mByteArrayModel, mCursorIndex, Okteta::ArrayChangeMetricsList(), true);
    mark(idx); // we have to change the marked range, otherwise it stays at the previous locked offset
}

bool StructuresTool::isStructureLocked(const QModelIndex& idx) const
{
    if (!mByteArrayModel) { // no point without ByteArrayModel
        return false;
    }
    if (!idx.isValid() || !idx.internalPointer()) {
        return false;
    }
    auto* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top) {
        return top->isLockedFor(mByteArrayModel);
    }
    return false;
}

bool StructuresTool::canStructureBeLocked(const QModelIndex& idx) const
{
    // we need a valid model and a valid index
    return mByteArrayModel && idx.isValid() && idx.internalPointer();
}

void StructuresTool::onChildItemDataChanged()
{
    mCurrentItemDataChanged = true;
}

Okteta::AbstractByteArrayModel* StructuresTool::byteArrayModel() const
{
    return mByteArrayModel;
}

StructuresManager* StructuresTool::manager() const
{
    return mManager.data();
}

QSysInfo::Endian StructuresTool::byteOrder() const
{
    return mByteOrder;
}

TopLevelDataInformation::List StructuresTool::allData() const
{
    TopLevelDataInformation::List ret;
    ret << mData << mInvalidData;
    return ret;
}

}
