/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

namespace Kasten
{
const QDir StructTool::defsDir = KGlobal::dirs()->locateLocal("data",
        "okteta/structures/", true);
StructTool::StructTool() :
            mByteArrayView(0),
            mByteArrayModel(0),
            mCursorIndex(0),
            //			mCharCodec(Okteta::CharCodec::createCodec(Okteta::LocalEncoding)),
            mByteOrder(
                    (StructViewPreferences::EnumByteOrder::type) StructViewPreferences::byteOrder())
{
    //leave mLoadedFiles empty for now, since otherwise loading slot will not work
    setObjectName("StructTool");
    loadStructDefFiles();
    //	mUtf8Codec = QTextCodec::codecForName("UTF-8");

    connect(this, SIGNAL(byteOrderChanged()), this, SLOT(updateData()));
}

StructTool::~StructTool()
{
    qDeleteAll(mData);
    qDeleteAll(mLoadedDefs);
    //	delete mCharCodec;
    StructViewPreferences::self()->writeConfig();
}
void StructTool::setByteOrder(StructViewPreferences::EnumByteOrder::type order)
{
    if (order != StructViewPreferences::byteOrder() || order != mByteOrder)
    {
        emit byteOrderChanged();
        StructViewPreferences::setByteOrder(order);
        mByteOrder = order;
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
        connect(mByteArrayView, SIGNAL(cursorPositionChanged( Okteta::Address )),
                SLOT(onCursorPositionChange( Okteta::Address )));
        connect(mByteArrayModel,
                SIGNAL(contentsChanged( const Okteta::ArrayChangeMetricsList& )),
                SLOT(onContentsChange()));
        //		onCharCodecChange(mByteArrayView->charCodingName());
        connect(mByteArrayView, SIGNAL(charCodecChanged( const QString& )),
                SLOT(onCharCodecChange( const QString& )));
    }

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
        kDebug("cursor pos changed -> updating data");
        updateData();
    }
}

void StructTool::setByteOrder(int order)
{
    if (order == StructViewPreferences::EnumByteOrder::LittleEndian)
        setByteOrder(StructViewPreferences::EnumByteOrder::LittleEndian);
    else if (order == StructViewPreferences::EnumByteOrder::BigEndian)
        setByteOrder(StructViewPreferences::EnumByteOrder::BigEndian);
        else
        kWarning() << "invalid byte order set:" << order;
}

void StructTool::onContentsChange()
{
    // TODO: only update if affected
    updateData();
}

bool StructTool::setData(const QVariant& value, int role, DataInformation* item)
{
    if (!mByteArrayModel)
        return false;
    if (role != Qt::EditRole)
        return false;

    int remaining = qMax(mByteArrayModel->size() - mCursorIndex, 0);
    for (int i = 0; i < mData.size(); ++i)
    {
        if (mData[i]->setData(value, item, mByteArrayModel, mByteOrder,
                mCursorIndex, remaining))
            return true;
    }
    return false;
}

void StructTool::updateData()
{
    kDebug() << "updateData()";
    int remaining;
    if (mByteArrayModel)
    {
        remaining = qMax(mByteArrayModel->size() - mCursorIndex, 0);
    }
    else
        remaining = 0;

    if (remaining != 0)
    {
        for (int i = 0; i < mData.size(); i++)
        {
            DataInformation* dat = mData.at(i);
            dat->readData(mByteArrayModel, mByteOrder, mCursorIndex, remaining);
        }
    }
}

//model interface:
QVariant StructTool::headerData(int column, int role)
{
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return i18nc("name of a data structure", "Name");
        case 1:
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
    return mData[idx];
}

void StructTool::addChildItem(DataInformation* child)
{
    if (child)
    {
        child->setIndex(mData.size());
        child->setParent(NULL);
        mData.append(child);
        connect(child, SIGNAL(dataChanged()), this, SLOT(onChildItemDataChanged()));

    }
}
void StructTool::loadStructDefFiles()
{
    QStringList filelist = StructViewPreferences::structureDefFiles();
    if (filelist == mLoadedFiles)
        return;
    //FIXME already loaded files behaviour is not correct
    QStringList includedFiles = mLoadedFiles;
    foreach(QString fileName,filelist)
        {
            fileName = QDir::cleanPath(fileName);
            QString relPath = defsDir.relativeFilePath(fileName);
            if (!mLoadedDefs.contains(relPath))
            {
                addStructDef(relPath);
            }
        }
    updateData();
    mLoadedFiles = filelist;
    setSelectedStructuresInView();
}

void StructTool::addStructDef(const QString& relPath)
{
    kDebug() << "add struct def " << relPath;
    QFileInfo fileInfo(defsDir, relPath);
    StructureDefinitionFile* def = new StructureDefinitionFile(fileInfo);
    def->parse(); //TODO lazy loading
    mLoadedDefs.insert(relPath, def);
    manageIncludes(def);
}

void StructTool::manageIncludes(const StructureDefinitionFile* def)
{
    QStringList includedFiles = def->includedFiles();
    if (includedFiles.length() == 0)
        return;
    kDebug() << "included files: " << def->includedFiles();
    QDir defDir = def->path().dir();
    //add included files structures
    for (int i = 0; i < includedFiles.length(); ++i)
    {
        QString inclPath = includedFiles.at(i);
        kDebug() << "include path: " << inclPath;
        //XXX maybe more lazy loading
        //check if included file is already loaded
        QString relPath =
                defsDir.relativeFilePath(defDir.absoluteFilePath(inclPath));
        kDebug() << "rel path = " << relPath;
        relPath = QDir::cleanPath(relPath);
        kDebug() << "rel path = " << relPath;
        if (mLoadedDefs.contains(relPath))
        {
            kDebug() << "included file already loaded: " << relPath;
        }
        else
        {
            QFileInfo test(defsDir, relPath);

            if (test.exists()) // only load if file exists
                addStructDef(relPath);
            else
            {
                kDebug() << "included file does not exist: " << relPath;
            }
        }
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
    kDebug() << "loadedStructs " << loadedStructs;
    foreach(QString s,loadedStructs)
        {
            int pos = regex.indexIn(s);
            if (pos > -1)
            {
                QString path = regex.cap(1);
                QString name = regex.cap(2);
//                kDebug() << "path=" << path << " name=" << name;
                StructureDefinitionFile* def = mLoadedDefs.value(path);
                if (!def)
                    continue;
                DataInformation* data = def->getStructure(name);
                if (data)
                    addChildItem(data);
            }
        } emit
    dataChanged();
    updateData();

}

void StructTool::mark(const QModelIndex& idx)
{
    if (!mByteArrayModel || !mByteArrayView)
    {
        kDebug() << "model or view == NULL";
        return;
    }
    DataInformation* data = static_cast<DataInformation*> (idx.internalPointer());
    if (!data)
        return;
    int length = data->getSize() / 8;
    int maxLen = mByteArrayModel->size() - mCursorIndex;
    length = qMin(length, maxLen);
    Okteta::Address startOffset = mCursorIndex + data->positionRelativeToParent();
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth(
            startOffset, length);
    mByteArrayView->setMarking(markingRange, true);
}
void StructTool::unmark(/*const QModelIndex& idx*/)
{
    if (mByteArrayView)
        mByteArrayView->setMarking(Okteta::AddressRange());
}

}
