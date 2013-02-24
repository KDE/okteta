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

#include "structtool.h"
#include "structuredefinitionfile.h"
#include "structuresmanager.h"
// lib
#include <bytearraydocument.h>
#include <bytearrayview.h>
// Okteta core
#include <character.h>
#include <charcodec.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
#include <KDebug>
#include <KGlobal>
#include <KStandardDirs>
//Qt
#include <QModelIndex>


#include "script/scripthandler.h"
#include "datatypes/datainformation.h"
#include "structviewpreferences.h"


namespace Kasten2
{

class StructToolPrivate
{
public:
    StructToolPrivate()
            : mByteArrayView(0), mByteArrayModel(0), mCursorIndex(0), mByteOrder(
                    (QSysInfo::Endian)StructViewPreferences::byteOrder()), mManager(new StructuresManager()),
                    mWritingData(false), mCurrentItemDataChanged(false)
    {
    }

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

StructTool::StructTool()
        : d(new StructToolPrivate())
{
    //leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName(QLatin1String("StructTool"));
    d->mManager->reloadPaths();
    setSelectedStructuresInView();
    //	mUtf8Codec = QTextCodec::codecForName("UTF-8");

    connect(this, SIGNAL(byteOrderChanged()), this, SLOT(onByteOrderChanged()));
}

void StructTool::onByteOrderChanged()
{
    updateData(Okteta::ArrayChangeMetricsList());
}

StructTool::~StructTool()
{
}

void StructTool::setByteOrder(QSysInfo::Endian order)
{
    if (order != (QSysInfo::Endian)StructViewPreferences::byteOrder() || order != d->mByteOrder)
    {
        emit byteOrderChanged();
        StructViewPreferences::setByteOrder((StructViewPreferences::EnumByteOrder::type)order);
        d->mByteOrder = order;
        updateData(Okteta::ArrayChangeMetricsList());
    }
}

void StructTool::setByteOrder(StructViewPreferences::EnumByteOrder::type order)
{
    setByteOrder(static_cast<QSysInfo::Endian>(order));
}

QString StructTool::title() const
{
    return i18nc("@title:window", "Structures");
}

void StructTool::setTargetModel(AbstractModel* model)
{
    //just a copy of the code in poddecodertool.h
    if (d->mByteArrayView)
        d->mByteArrayView->disconnect(this);
    if (d->mByteArrayModel)
        d->mByteArrayModel->disconnect(this);

    d->mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;
    ByteArrayDocument* document = d->mByteArrayView ?
            qobject_cast<ByteArrayDocument*>(d->mByteArrayView->baseModel()) : 0;
    d->mByteArrayModel = document ? document->content() : 0;

    if (d->mByteArrayModel && d->mByteArrayView)
    {
        d->mCursorIndex = d->mByteArrayView->cursorPosition();
        connect(d->mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)),
                SLOT(onCursorPositionChange(Okteta::Address)));
        connect(d->mByteArrayModel, SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                SLOT(onContentsChange(Okteta::ArrayChangeMetricsList)));
    }
    emit byteArrayModelChanged(d->mByteArrayModel);
    updateData(Okteta::ArrayChangeMetricsList());
}

void StructTool::onCursorPositionChange(Okteta::Address pos)
{
    if (d->mCursorIndex != pos)
    {
        d->mCursorIndex = pos;
        updateData(Okteta::ArrayChangeMetricsList());
        emit cursorIndexChanged();
    }
}

void StructTool::setByteOrder(int order)
{
    if (order == QSysInfo::LittleEndian)
        setByteOrder(QSysInfo::LittleEndian);
    else if (order == QSysInfo::BigEndian)
        setByteOrder(QSysInfo::BigEndian);
    else
    {
        kWarning() << "invalid byte order set:" << order;
    }
}

void StructTool::onContentsChange(const Okteta::ArrayChangeMetricsList& list)
{
    kDebug() << "contents changed";
    for (int i = 0; i < list.size(); ++i) {
        const Okteta::ArrayChangeMetrics& acm = list.at(i);
        kDebug() << "change: t=" << acm.type() << "o=" << acm.offset() << "a2=" << acm.removeLength() << "a3=" << acm.insertLength();
    }
    updateData(list);
}

bool StructTool::setData(const QVariant& value, int role, DataInformation* item, uint row)
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
    ret = item->setData(value, d->mByteArrayModel, position / 8, remainingBits, bitOffs);
    d->mWritingData = false; //finished
    //XXX: this is inefficient, best would be to only update the changed value
    updateData(Okteta::ArrayChangeMetricsList()); //update once after writing
    return ret;
}

void StructTool::updateData(const Okteta::ArrayChangeMetricsList& list)
{
    if (d->mWritingData)
    {
        kWarning() << "currently writing data, won't update";
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
QVariant StructTool::headerData(int column, int role)
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

int StructTool::childCount() const
{
    return d->mData.size();
}

DataInformation* StructTool::childAt(int idx) const
{
    if (idx >= d->mData.size() || idx < 0)
    {
        return 0;
    }
    //don't expose the topLevelDataInformation, since this may cause crashes
    return d->mData.at(idx)->actualDataInformation();
}

void StructTool::addChildItem(TopLevelDataInformation* child)
{
    Q_CHECK_PTR(child);
    child->setParent(this);
    if (child->isValid())
    {
        child->setIndex(d->mData.size());
        connect(child, SIGNAL(dataChanged()), this, SLOT(onChildItemDataChanged()));
        connect(child, SIGNAL(childrenAboutToBeInserted(DataInformation*,uint,uint)),
                this, SIGNAL(childrenAboutToBeInserted(DataInformation*,uint,uint)));
        connect(child, SIGNAL(childrenInserted(const DataInformation*,uint,uint)),
                this, SIGNAL(childrenInserted(const DataInformation*,uint,uint)));
        connect(child, SIGNAL(childrenAboutToBeRemoved(DataInformation*,uint,uint)),
                this, SIGNAL(childrenAboutToBeRemoved(DataInformation*,uint,uint)));
        connect(child, SIGNAL(childrenRemoved(const DataInformation*,uint,uint)),
                this, SIGNAL(childrenRemoved(const DataInformation*,uint,uint)));
        connect(this, SIGNAL(byteArrayModelChanged(Okteta::AbstractByteArrayModel*)),
                child, SLOT(newModelActivated(Okteta::AbstractByteArrayModel*)));
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

void StructTool::setSelectedStructuresInView()
{
    d->mData.clear();
    d->mInvalidData.clear();
    emit dataCleared();

    QRegExp regex(QLatin1String("'(.+)':'(.+)'"));
    QStringList loadedStructs = StructViewPreferences::loadedStructures();
    kDebug() << "loadedStructs " << loadedStructs;
    for (int i = 0; i < loadedStructs.size(); ++i)
    {
        const QString& s = loadedStructs.at(i);
        int pos = regex.indexIn(s);
        if (pos > -1)
        {
            QString pluginName = regex.cap(1);
            QString name = regex.cap(2);
            //kDebug() << "pluginName=" << path << " structureName=" << name;
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
            }
        }
    }
    for (int i = 0; i < d->mData.count(); ++i)
    {
        emit dataChanged(i, d->mData.at(i)->actualDataInformation());
    }
    updateData(Okteta::ArrayChangeMetricsList());

}

Okteta::Address StructTool::startAddress(const TopLevelDataInformation* data)
{
    if (data->isLockedFor(d->mByteArrayModel))
        return data->lockPositionFor(d->mByteArrayModel);
    else
        return d->mCursorIndex;
}

void StructTool::mark(const QModelIndex& idx)
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
    const Okteta::Address startOffset = data->positionInFile(baseAddress) / 8;
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(startOffset, length);
    d->mByteArrayView->setMarking(markingRange, true);
}

void StructTool::unmark(/*const QModelIndex& idx*/)
{
    if (d->mByteArrayView)
        d->mByteArrayView->setMarking(Okteta::AddressRange());
}

void StructTool::validateAllStructures()
{
    if (!d->mByteArrayModel)
        return; //no point validating
    const int size = d->mData.size();
    for (int i = 0; i < size; ++i)
    {
        d->mData.at(i)->validate();
    }
}

int StructTool::columnCount() const
{
    return DataInformation::COLUMN_COUNT;
}

bool StructTool::isFileLoaded() const
{
    return d->mByteArrayModel != NULL;
}

void StructTool::lockStructure(const QModelIndex& idx)
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

void StructTool::unlockStructure(const QModelIndex& idx)
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

bool StructTool::isStructureLocked(const QModelIndex& idx) const
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

bool StructTool::canStructureBeLocked(const QModelIndex& idx) const
{
    //we need a valid model and a valid index
    return d->mByteArrayModel && idx.isValid() && idx.internalPointer();
}

void StructTool::onChildItemDataChanged()
{
    d->mCurrentItemDataChanged = true;
}

Okteta::AbstractByteArrayModel* StructTool::byteArrayModel() const
{
    return d->mByteArrayModel;
}

StructuresManager* StructTool::manager() const
{
    return d->mManager.data();
}

QSysInfo::Endian StructTool::byteOrder() const
{
    return d->mByteOrder;
}

TopLevelDataInformation::List StructTool::allData() const
{
    TopLevelDataInformation::List ret;
    ret << d->mData << d->mInvalidData;
    return ret;
}

}
