/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestool.hpp"

// lib
#include "structuredefinitionfile.hpp"
#include "structuresmanager.hpp"
#include <structureslogging.hpp>
#include "structuretreemodel.hpp"
#include "script/scripthandler.hpp"
#include "datatypes/datainformation.hpp"
#include "structureviewpreferences.hpp"
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
#include <Okteta/OffsetFormat>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QModelIndex>
#include <QRegularExpression>
#include <QByteArray>

namespace Kasten {

StructuresTool::StructuresTool()
    : mByteOrder(StructureViewPreferences::byteOrder())
    , mManager(new StructuresManager(this))
    , mWritingData(false)
    , mCurrentItemDataChanged(false)
    , mIsStructureMarked(false)
{
    // leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName(QStringLiteral("StructuresTool"));
    mManager->reloadPaths();
    setEnabledStructuresInView();
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
        Q_EMIT byteOrderChanged();
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
    ByteArrayView* const byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    // just a copy of the code in poddecodertool.h
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
        if (mIsStructureMarked) {
            unmark();
        }
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mByteArrayView = byteArrayView;
    // TODO; check if already same document used before
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
    Q_EMIT byteArrayModelChanged(mByteArrayModel);
    updateData(Okteta::ArrayChangeMetricsList());
}

bool StructuresTool::isStructureListEmpty() const
{
    return mData.isEmpty() && mInvalidData.isEmpty();
}

void StructuresTool::onCursorPositionChange(Okteta::Address pos)
{
    if (mCursorIndex != pos) {
        mCursorIndex = pos;
        updateData(Okteta::ArrayChangeMetricsList());
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
    for (const Okteta::ArrayChangeMetrics& acm : list) {
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
    // block immediate data updating
    mWritingData = true;

    bool ret = false;
    BitCount64 position = item->positionInFile(structureStart);
    const quint64 remainingBits = qMax(mByteArrayModel->size() * 8 - qint64(position), qint64(0));
    quint8 bitOffs = position % 8;
    ret = item->setData(value, mByteArrayModel, Okteta::Address(position / 8), remainingBits, bitOffs);

    // unblock updating and catch up
    mWritingData = false;
    updateData(mArrayChangesWhileWriting);
    mArrayChangesWhileWriting.clear();

    return ret;
}

void StructuresTool::updateData(const Okteta::ArrayChangeMetricsList& list)
{
    if (mWritingData) {
        mArrayChangesWhileWriting.append(list);
        return;
    }
    if (!mByteArrayModel) {
        return;
    }

    for (int i = 0; i < mData.size(); i++) {
        const TopLevelDataInformation::Ptr& dat = mData.at(i);
        dat->read(mByteArrayModel, mCursorIndex, list, false);
        if (mCurrentItemDataChanged) {
            Q_EMIT dataChanged(i, mData.at(i)->actualDataInformation());
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
        case DataInformation::ColumnSize:
            return i18nc("size of a data structure", "Size");
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

void StructuresTool::setEnabledStructuresInView()
{
    const bool wasStructureListEmpty = isStructureListEmpty();

    mData.clear();
    mInvalidData.clear();
    Q_EMIT dataCleared();

    QRegularExpression regex(QStringLiteral("'(.+)':'(.+)'"));
    const QStringList loadedStructs = StructureViewPreferences::loadedStructures();
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "loadedStructs " << loadedStructs;
    for (const QString& s : loadedStructs) {
        QRegularExpressionMatch match = regex.match(s);
        if (match.hasMatch()) {
            const QString structureId = match.captured(1);
            const QString name = match.captured(2);
            // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "structureId=" << path << " structureName=" << name;
            StructureDefinitionFile* def = mManager->definition(structureId);
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
                    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Could not find structure with name" << name << "in" << structureId;
                }
            }
        }
    }

    for (int i = 0; i < mData.size(); ++i) {
        Q_EMIT dataChanged(i, mData.at(i)->actualDataInformation());
    }

    updateData(Okteta::ArrayChangeMetricsList());

    const bool isNowStructureListEmpty = isStructureListEmpty();
    if (isNowStructureListEmpty != wasStructureListEmpty) {
        Q_EMIT isStructureListEmptyChanged(isNowStructureListEmpty);
    }
}

Okteta::Address StructuresTool::startAddress(const TopLevelDataInformation* data) const
{
    if (data->isLockedFor(mByteArrayModel)) {
        return data->lockPositionFor(mByteArrayModel);
    }

    return mCursorIndex;
}

QString StructuresTool::dataAddressAsString(const DataInformation* data) const
{
    Q_CHECK_PTR(data->topLevelDataInformation());
    const Okteta::Address baseAddress = startAddress(data->topLevelDataInformation());
    const Okteta::Address dataOffset = Okteta::Address(data->positionInFile(baseAddress) / 8);

    const auto codingId = static_cast<Okteta::OffsetFormat::Format>(mByteArrayView->offsetCoding());

    struct ValueCoding {
        int base;
        int width;
    };
    const ValueCoding coding =
        (codingId == Okteta::OffsetFormat::Hexadecimal) ? ValueCoding{16,  8} :
        (codingId == Okteta::OffsetFormat::Decimal) ?     ValueCoding{10, 10} :
        /* else */                                        ValueCoding{ 8, 11};

    const QString offsetText = QStringLiteral("%1").arg(dataOffset, coding.width, coding.base, QLatin1Char('0')).toUpper();
    return offsetText;
}

Okteta::AddressRange StructuresTool::dataRange(const DataInformation* data) const
{
    Q_CHECK_PTR(data->topLevelDataInformation());
    const Okteta::Address baseAddress = startAddress(data->topLevelDataInformation());
    // FIXME support range of partial bytes
    int length = data->size() / 8;
    const int maxLen = mByteArrayModel->size() - baseAddress;
    length = qMin(length, maxLen);
    const Okteta::Address startOffset = Okteta::Address(data->positionInFile(baseAddress) / 8);
    return Okteta::AddressRange::fromWidth(startOffset, length);
}

void StructuresTool::selectBytesInView(const QModelIndex& idx)
{
    if (!mByteArrayModel || !mByteArrayView || !idx.isValid()) {
        return;
    }
    const auto* data = idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    const Okteta::AddressRange selection = dataRange(data);
    mByteArrayView->setSelection(selection.start(), selection.end());
    mByteArrayView->setFocus();
}

void StructuresTool::mark(const QModelIndex& idx)
{
    if (!mByteArrayModel || !mByteArrayView || !idx.isValid()) {
        return;
    }
    const auto* data = idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    const Okteta::AddressRange markingRange = dataRange(data);

    mByteArrayView->setMarking(markingRange, true);
    mIsStructureMarked = true;
}

void StructuresTool::unmark(/*const QModelIndex& idx*/)
{
    if (mByteArrayView) {
        mByteArrayView->setMarking(Okteta::AddressRange());
        mIsStructureMarked = false;
    }
}

QByteArray StructuresTool::bytes(const DataInformation* data) const
{
    const Okteta::AddressRange range = dataRange(data);
    QByteArray bytes;
    bytes.resize(range.width());
    std::ignore = mByteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(bytes.data()), range.start(), range.width());
    return bytes;
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
    if (!mByteArrayModel || !idx.isValid()) {
        return;
    }
    auto* data = idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top) {
        top->lockPositionToOffset(mCursorIndex, mByteArrayModel);
    }
}

void StructuresTool::unlockStructure(const QModelIndex& idx)
{
    if (!mByteArrayModel || !idx.isValid()) {
        return;
    }
    auto* data = idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);

    const bool wasMarked = mIsStructureMarked;
    if (wasMarked) {
        unmark();
    }
    top->unlockPosition(mByteArrayModel);
    // now read from the current position:
    top->read(mByteArrayModel, mCursorIndex, Okteta::ArrayChangeMetricsList(), true);
    if (wasMarked) {
        mark(idx); // we have to change the marked range, otherwise it stays at the previous locked offset
    }
}

bool StructuresTool::isStructureLocked(const QModelIndex& idx) const
{
    if (!mByteArrayModel || !idx.isValid()) {
        return false;
    }
    auto* data = idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return false;
    }
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
    return mByteArrayModel && idx.isValid() && idx.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
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
    return mManager.get();
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

#include "moc_structurestool.cpp"
