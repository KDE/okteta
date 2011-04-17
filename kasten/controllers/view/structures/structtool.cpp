/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "script/scripthandler.h"

#include "datatypes/topleveldatainformation.h"
#include "datatypes/datainformation.h"

namespace Kasten
{

StructTool::StructTool() :
    mByteArrayView(0), mByteArrayModel(0), mCursorIndex(0), mByteOrder(
            StructViewPreferences::byteOrder()), mManager(new StructuresManager()),
            mWritingData(false), mCurrentItemDataChanged(false)
{
    //leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName( QLatin1String("StructTool" ));
    mManager->reloadPaths();
    setSelectedStructuresInView();
    //	mUtf8Codec = QTextCodec::codecForName("UTF-8");

    connect(this, SIGNAL(byteOrderChanged()), this, SLOT(updateData()));
}

StructTool::~StructTool()
{
    //	delete mCharCodec;
    delete mManager;
}
void StructTool::setByteOrder(StructViewPreferences::EnumByteOrder::type order)
{
    if (order != StructViewPreferences::byteOrder() || order != mByteOrder)
    {
        emit byteOrderChanged();
        StructViewPreferences::setByteOrder(order);
        mByteOrder = order;
        updateData();
    }
}
QString StructTool::title() const
{
    return i18nc("@title:window", "Structures");
}

void StructTool::setTargetModel(AbstractModel* model)
{
    //just a copy of the code in poddecodertool.h
    if (mByteArrayView)
        mByteArrayView->disconnect(this);
    if (mByteArrayModel)
        mByteArrayModel->disconnect(this);

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*> () : 0;
    ByteArrayDocument *document =
            mByteArrayView ? qobject_cast<ByteArrayDocument*> (
                    mByteArrayView->baseModel()) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if (mByteArrayModel && mByteArrayView)
    {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect(mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)),
                SLOT(onCursorPositionChange(Okteta::Address)));
        connect(mByteArrayModel,
                SIGNAL(contentsChanged(const Okteta::ArrayChangeMetricsList&)),
                SLOT(onContentsChange(const Okteta::ArrayChangeMetricsList&)));
        //		onCharCodecChange(mByteArrayView->charCodingName());
        //         connect(mByteArrayView, SIGNAL(charCodecChanged( const QString& )),
        //                 SLOT(onCharCodecChange( const QString& )));
    }emit
    byteArrayModelChanged(mByteArrayModel != NULL);
    updateData();
}
//void StructTool::onCharCodecChange(const QString& codecName)
//{
//	if (codecName == mCharCodec->name())
//		return;
//
//	delete mCharCodec;
//	mCharCodec = Okteta::CharCodec::createCodec(codecName);
//	updateData();
//}
void StructTool::onCursorPositionChange(Okteta::Address pos)
{
    if (mCursorIndex != pos)
    {
        mCursorIndex = pos;
        updateData();
        emit cursorIndexChanged();
    }
}

void StructTool::setByteOrder(int order)
{
    if (order == StructViewPreferences::EnumByteOrder::LittleEndian)
        setByteOrder(StructViewPreferences::EnumByteOrder::LittleEndian);
    else if (order == StructViewPreferences::EnumByteOrder::BigEndian)
        setByteOrder(StructViewPreferences::EnumByteOrder::BigEndian);
    else
    {
        kWarning() << "invalid byte order set:" << order;
    }
}

void StructTool::onContentsChange(const Okteta::ArrayChangeMetricsList& list)
{
    updateData(list);
}

bool StructTool::setData(const QVariant& value, int role, DataInformation* item)
{
    if (!mByteArrayModel)
        return false;
    if (role != Qt::EditRole)
        return false;
    mWritingData = true;
    bool found = false;
    QScopedPointer<quint8> bitOffset(new quint8(0));
    int remaining = qMax(mByteArrayModel->size() - mCursorIndex, 0);
    for (int i = 0; i < mData.size(); ++i)
    {
        if (mData[i]->actualDataInformation()->setData(value, item, mByteArrayModel,
                mCursorIndex, remaining, bitOffset.data()))
        {
            found = true;
            break;
        }
    }
    mWritingData = false; //finished
    //XXX: this is inefficient, best would be to only update the changed value
    updateData(); //update once after writing
    return found;
}

void StructTool::updateData(const Okteta::ArrayChangeMetricsList& list)
{
    if (mWritingData)
    {
        kWarning() << "currently writing data, won't update";
        return;
    }
    if (!mByteArrayModel)
        return;

    for (int i = 0; i < mData.size(); i++)
    {
        TopLevelDataInformation* dat = mData.at(i);
        dat->read(mByteArrayModel, mCursorIndex, list);
        if (mCurrentItemDataChanged)
            emit dataChanged(i, mData.at(i)->actualDataInformation());
        mCurrentItemDataChanged = false;
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
    return mData.size();
}

DataInformation* StructTool::childAt(int idx) const
{
    if (idx >= mData.size() || idx < 0)
    {
        return NULL;
    }
    //don't expose the topLevelDataInformation, since this may cause crashes
    return mData.at(idx)->actualDataInformation();
}

void StructTool::addChildItem(TopLevelDataInformation* child)
{
    if (child)
    {
        child->setIndex(mData.size());
        child->setParent(this);
        mData.append(child);
        connect(child, SIGNAL(dataChanged()), this, SLOT(onChildItemDataChanged()));
    }
}

void StructTool::setSelectedStructuresInView()
{
    qDeleteAll(mData);
    mData.clear();
    emit
    dataCleared();

    QRegExp regex("'(.+)':'(.+)'");
    QStringList loadedStructs = StructViewPreferences::loadedStructures();
    kDebug()
        << "loadedStructs " << loadedStructs;
    foreach(const QString& s,loadedStructs)
        {
            int pos = regex.indexIn(s);
            if (pos > -1)
            {
                QString pluginName = regex.cap(1);
                QString name = regex.cap(2);
                //kDebug() << "pluginName=" << path << " structureName=" << name;
                StructureDefinitionFile* def = mManager->definition(pluginName);
                if (!def)
                    continue;
                if (!def->isValid())
                    continue;
                //should be valid now
                TopLevelDataInformation* data = def->structure(name);
                if (data)
                    addChildItem(data);
            }
        }
    for (int i = 0; i < mData.count(); ++i) {
        emit dataChanged(i, mData.at(i)->actualDataInformation());
    }
    updateData();

}

void StructTool::mark(const QModelIndex& idx)
{
    if (!mByteArrayModel || !mByteArrayView)
    {
        kDebug()
            << "model or view == NULL";
        return;
    }
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    if (!data)
        return;
    int length = data->size() / 8;
    int maxLen = mByteArrayModel->size() - mCursorIndex;
    length = qMin(length, maxLen);
    Q_CHECK_PTR(data->topLevelDataInformation());
    Okteta::Address baseAddress;
    if (data->topLevelDataInformation()->isLockedFor(mByteArrayModel))
    {
        baseAddress = data->topLevelDataInformation()->lockPositionFor(
                mByteArrayModel);
    }
    else
    {
        //not locked
        baseAddress = mCursorIndex;
    }

    //FIXME support marking of partial bytes
    Okteta::Address startOffset = baseAddress + data->positionRelativeToParent()
            / 8;
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(
            startOffset, length);
    mByteArrayView->setMarking(markingRange, true);
    //    kDebug()
    //        << "marking range " << markingRange.start() << " to  " << markingRange.end();
}

void StructTool::unmark(/*const QModelIndex& idx*/)
{
    if (mByteArrayView)
        mByteArrayView->setMarking(Okteta::AddressRange());
}

void StructTool::validateAllStructures()
{
    if (!mByteArrayModel)
        return; //no point validating
    //TODO it would be nicer if the button was grayed out while no model exists
    foreach(TopLevelDataInformation* data, mData)
        {
            data->validate();
        }
}

int StructTool::columnCount() const
{
    return DataInformation::COLUMN_COUNT;
}

Qt::ItemFlags StructTool::flags(int column, DataInformation* data) const
{
    if (!data)
        return 0; // just return something in case data is null
    return data->flags(column, mByteArrayModel != NULL);
}

void StructTool::lockStructure(QModelIndex idx)
{
    if (!mByteArrayModel) //no point without ByteArrayModel
        return;
    if (!idx.isValid() || !idx.internalPointer())
        return;
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
        top->lockPositionToOffset(mCursorIndex, mByteArrayModel);
}
void StructTool::unlockStructure(QModelIndex idx)
{
    if (!mByteArrayModel) //no point without ByteArrayModel
        return;
    if (!idx.isValid() || !idx.internalPointer())
        return;
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
    {
        top->unlockPosition(mByteArrayModel);
        //now read from the current position:
        top->read(mByteArrayModel, mCursorIndex);
    }
}

bool StructTool::isStructureLocked(QModelIndex idx) const
{
    if (!mByteArrayModel) //no point without ByteArrayModel
        return false;
    if (!idx.isValid() || !idx.internalPointer())
        return false;
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
        return top->isLockedFor(mByteArrayModel);
    return false;
}

bool StructTool::canStructureBeLocked(QModelIndex idx) const
{
    if (!mByteArrayModel) //no point without ByteArrayModel
        return false;
    if (!idx.isValid() || !idx.internalPointer())
        return false;
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_ASSERT(top);
    if (top)
        return true;
    return false;
}

void StructTool::onChildItemDataChanged()
{
    mCurrentItemDataChanged = true;
}


}
