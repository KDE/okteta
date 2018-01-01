/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *   Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "structurestool.h"
#include "structuredefinitionfile.h"
#include "structuresmanager.h"
#include "structlogging.h"
// lib
#include <kasten/okteta/bytearraydocument.h>
#include <kasten/okteta/bytearrayview.h>
// Okteta core
#include <okteta/character.h>
#include <okteta/charcodec.h>
#include <okteta/abstractbytearraymodel.h>
// KF5
#include <KLocalizedString>
//Qt
#include <QModelIndex>


#include "script/scripthandler.h"
#include "datatypes/datainformation.h"
#include "structureviewpreferences.h"


namespace Kasten
{

class StructuresToolPrivate
{
public:
    StructuresToolPrivate() : mByteArrayView(nullptr), mByteArrayModel(nullptr), mCursorIndex(0),
            mByteOrder(StructureViewPreferences::byteOrder()), mManager(new StructuresManager()),
            mWritingData(false), mCurrentItemDataChanged(false) {}

    // source
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
    Okteta::Address mCursorIndex;

    // settings
    QSysInfo::Endian mByteOrder;
    QScopedPointer<StructuresManager> mManager;
    TopLevelDataInformation::List mData;
    TopLevelDataInformation::List mInvalidData;
    bool mWritingData :1;
    bool mCurrentItemDataChanged :1;
};

StructuresTool::StructuresTool()
        : d(new StructuresToolPrivate())
{
    //leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName(QStringLiteral("StructuresTool"));
    d->mManager->reloadPaths();
    setSelectedStructuresInView();
    //	mUtf8Codec = QTextCodec::codecForName("UTF-8");

    connect(this, &StructuresTool::byteOrderChanged, this, &StructuresTool::onByteOrderChanged);
}

void StructuresTool::onByteOrderChanged()
{
    updateData(Okteta::ArrayChangeMetricsList());
}

StructuresTool::~StructuresTool()
{
}

void StructuresTool::setByteOrder(QSysInfo::Endian order)
{
    if (order != StructureViewPreferences::byteOrder() || order != d->mByteOrder)
    {
        emit byteOrderChanged();
        StructureViewPreferences::setByteOrder(order);
        d->mByteOrder = order;
        updateData(Okteta::ArrayChangeMetricsList());
    }
}

QString StructuresTool::title() const
{
    return i18nc("@title:window", "Structures");
}

void StructuresTool::setTargetModel(AbstractModel* model)
{
    //just a copy of the code in poddecodertool.h
    if (d->mByteArrayView)
        d->mByteArrayView->disconnect(this);
    if (d->mByteArrayModel)
        d->mByteArrayModel->disconnect(this);

    d->mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    ByteArrayDocument* document = d->mByteArrayView ?
            qobject_cast<ByteArrayDocument*>(d->mByteArrayView->baseModel()) : nullptr;
    d->mByteArrayModel = document ? document->content() : nullptr;

    if (d->mByteArrayModel && d->mByteArrayView)
    {
        d->mCursorIndex = d->mByteArrayView->cursorPosition();
        connect(d->mByteArrayView, &ByteArrayView::cursorPositionChanged,
                this, &StructuresTool::onCursorPositionChange);
        connect(d->mByteArrayModel, &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &StructuresTool::onContentsChange);
    }
    emit byteArrayModelChanged(d->mByteArrayModel);
    updateData(Okteta::ArrayChangeMetricsList());
}

void StructuresTool::onCursorPositionChange(Okteta::Address pos)
{
    if (d->mCursorIndex != pos)
    {
        d->mCursorIndex = pos;
        updateData(Okteta::ArrayChangeMetricsList());
        emit cursorIndexChanged();
    }
}

void StructuresTool::setByteOrder(int order)
{
    if (order == QSysInfo::LittleEndian)
        setByteOrder(QSysInfo::LittleEndian);
    else if (order == QSysInfo::BigEndian)
        setByteOrder(QSysInfo::BigEndian);
    else
    {
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
    if (!d->mByteArrayModel)
        return false;
    if (role != Qt::EditRole)
        return false;
    Q_CHECK_PTR(item);

    TopLevelDataInformation* topLevel = item->topLevelDataInformation();
    const Okteta::Address structureStart = startAddress(topLevel);
    d->mWritingData = true;
    bool ret = false;
    BitCount64 position = item->positionInFile(structureStart);
    const quint64 remainingBits = qMax(d->mByteArrayModel->size() * 8 - qint64(position), qint64(0));
    quint8 bitOffs = position % 8;
    ret = item->setData(value, d->mByteArrayModel, Okteta::Address(position / 8), remainingBits, bitOffs);
    d->mWritingData = false; //finished
    //XXX: this is inefficient, best would be to only update the changed value
    updateData(Okteta::ArrayChangeMetricsList()); //update once after writing
    return ret;
}

void StructuresTool::updateData(const Okteta::ArrayChangeMetricsList& list)
{
    if (d->mWritingData)
    {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "currently writing data, won't update";
        return;
    }
    if (!d->mByteArrayModel)
        return;

    for (int i = 0; i < d->mData.size(); i++)
    {
        const TopLevelDataInformation::Ptr& dat = d->mData.at(i);
        dat->read(d->mByteArrayModel, d->mCursorIndex, list, false);
        if (d->mCurrentItemDataChanged)
            emit dataChanged(i, d->mData.at(i)->actualDataInformation());
        d->mCurrentItemDataChanged = false;
    }
}

//model interface:
QVariant StructuresTool::headerData(int column, int role)
{
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case DataInformation::ColumnName:
            return i18nc("name of a data structure", "Name");
        case DataInformation::ColumnType:
            return i18nc("type of a data structure", "Type");
        case DataInformation::ColumnValue:
            return i18nc("value of a data structure (primitive type)", "Value");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

int StructuresTool::childCount() const
{
    return d->mData.size();
}

DataInformation* StructuresTool::childAt(int idx) const
{
    if (idx >= d->mData.size() || idx < 0)
    {
        return nullptr;
    }
    //don't expose the topLevelDataInformation, since this may cause crashes
    return d->mData.at(idx)->actualDataInformation();
}

void StructuresTool::addChildItem(TopLevelDataInformation* child)
{
    Q_CHECK_PTR(child);
    child->setParent(this);
    if (child->isValid())
    {
        child->setIndex(d->mData.size());
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
        d->mData.append(QSharedPointer<TopLevelDataInformation>(child));
        //ensure that locking gets set up properly
        if (d->mByteArrayModel)
            child->newModelActivated(d->mByteArrayModel);
    }
    else
    {
        child->setIndex(d->mInvalidData.size());
        d->mInvalidData.append(QSharedPointer<TopLevelDataInformation>(child));
    }
}

void StructuresTool::setSelectedStructuresInView()
{
    d->mData.clear();
    d->mInvalidData.clear();
    emit dataCleared();

    QRegExp regex(QStringLiteral("'(.+)':'(.+)'"));
    QStringList loadedStructs = StructureViewPreferences::loadedStructures();
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "loadedStructs " << loadedStructs;
    for (int i = 0; i < loadedStructs.size(); ++i)
    {
        const QString& s = loadedStructs.at(i);
        int pos = regex.indexIn(s);
        if (pos > -1)
        {
            QString pluginName = regex.cap(1);
            QString name = regex.cap(2);
            //qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "pluginName=" << path << " structureName=" << name;
            StructureDefinitionFile* def = d->mManager->definition(pluginName);
            if (!def)
                continue;
            if (!def->isValid())
                continue;
            //should be valid now
            if (name == QLatin1String("*"))
            {
                //add all of them
                QVector<TopLevelDataInformation*> structs = def->structures();
                for (int i = 0; i < structs.size(); ++i)
                    addChildItem(structs.at(i));
            }
            else
            {
                TopLevelDataInformation* data = def->structure(name);
                if (data)
                    addChildItem(data);
                else
                    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Could not find structure with name" << name << "in" << pluginName;
            }
        }
    }
    for (int i = 0; i < d->mData.count(); ++i)
    {
        emit dataChanged(i, d->mData.at(i)->actualDataInformation());
    }
    updateData(Okteta::ArrayChangeMetricsList());

}

Okteta::Address StructuresTool::startAddress(const TopLevelDataInformation* data)
{
    if (data->isLockedFor(d->mByteArrayModel))
        return Okteta::Address(data->lockPositionFor(d->mByteArrayModel));
    else
        return d->mCursorIndex;
}

void StructuresTool::mark(const QModelIndex& idx)
{
    if (!d->mByteArrayModel || !d->mByteArrayView || !idx.isValid())
        return;
    const DataInformation* data = static_cast<const DataInformation*>(idx.internalPointer());
    if (!data)
        return;
    Q_CHECK_PTR(data->topLevelDataInformation());
    const Okteta::Address baseAddress = startAddress(data->topLevelDataInformation());
    //FIXME support marking of partial bytes
    int length = data->size() / 8;
    const int maxLen = d->mByteArrayModel->size() - baseAddress;
    length = qMin(length, maxLen);
    const Okteta::Address startOffset = Okteta::Address(data->positionInFile(baseAddress) / 8);
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(startOffset, length);
    d->mByteArrayView->setMarking(markingRange, true);
}

void StructuresTool::unmark(/*const QModelIndex& idx*/)
{
    if (d->mByteArrayView)
        d->mByteArrayView->setMarking(Okteta::AddressRange());
}

void StructuresTool::validateAllStructures()
{
    if (!d->mByteArrayModel)
        return; //no point validating
    const int size = d->mData.size();
    for (int i = 0; i < size; ++i)
    {
        d->mData.at(i)->validate();
    }
}

bool StructuresTool::isFileLoaded() const
{
    return ( d->mByteArrayModel != nullptr );
}

void StructuresTool::lockStructure(const QModelIndex& idx)
{
    if (!d->mByteArrayModel) //no point without ByteArrayModel
        return;
    if (!idx.isValid() || !idx.internalPointer())
        return;
    DataInformation* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
        top->lockPositionToOffset(d->mCursorIndex, d->mByteArrayModel);
}

void StructuresTool::unlockStructure(const QModelIndex& idx)
{
    if (!d->mByteArrayModel) //no point without ByteArrayModel
        return;
    if (!idx.isValid() || !idx.internalPointer())
        return;
    DataInformation* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);

    unmark();
    top->unlockPosition(d->mByteArrayModel);
    //now read from the current position:
    top->read(d->mByteArrayModel, d->mCursorIndex, Okteta::ArrayChangeMetricsList(), true);
    mark(idx); //we have to change the marked range, otherwise it stays at the previous locked offset
}

bool StructuresTool::isStructureLocked(const QModelIndex& idx) const
{
    if (!d->mByteArrayModel) //no point without ByteArrayModel
        return false;
    if (!idx.isValid() || !idx.internalPointer())
        return false;
    DataInformation* data = static_cast<DataInformation*>(idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
        return top->isLockedFor(d->mByteArrayModel);
    return false;
}

bool StructuresTool::canStructureBeLocked(const QModelIndex& idx) const
{
    //we need a valid model and a valid index
    return d->mByteArrayModel && idx.isValid() && idx.internalPointer();
}

void StructuresTool::onChildItemDataChanged()
{
    d->mCurrentItemDataChanged = true;
}

Okteta::AbstractByteArrayModel* StructuresTool::byteArrayModel() const
{
    return d->mByteArrayModel;
}

StructuresManager* StructuresTool::manager() const
{
    return d->mManager.data();
}

QSysInfo::Endian StructuresTool::byteOrder() const
{
    return d->mByteOrder;
}

TopLevelDataInformation::List StructuresTool::allData() const
{
    TopLevelDataInformation::List ret;
    ret << d->mData << d->mInvalidData;
    return ret;
}

}
