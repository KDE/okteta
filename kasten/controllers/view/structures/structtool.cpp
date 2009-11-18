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
#include <QDir>
#include <QFile>
//data types
#include "datatypes/structuredatainformation.h"
#include "datatypes/uniondatainformation.h"
#include "datatypes/primitivedatainformation.h"
#include "datatypes/staticlengtharraydatainformation.h"

namespace Kasten
{
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
    qDeleteAll(this->mData);
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
    ByteArrayDocument *document = mByteArrayView ? qobject_cast<ByteArrayDocument*> (
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
        kWarning() << "§invalid byte order set:" << order;
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
        if (mData[i]->setData(value, item, mByteArrayModel, mByteOrder, mCursorIndex,
                remaining))
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
    //just empty list, don't bother with rest of method
    qDeleteAll(mData);
    mData.clear();
    emit
    dataChanged();
    emit
    dataCleared();
    QString defaultDir = KGlobal::dirs()->locateLocal("data", "okteta/structures/", true);
    kDebug() << defaultDir;
    //FIXME already loaded files behavour is not correct
    QStringList includedFiles = mLoadedFiles;
    foreach(QString fileName,filelist)
        {
            QFileInfo fileInfo(fileName);
            if (!fileInfo.isAbsolute())
                fileInfo = QFileInfo(defaultDir + fileName);

            DataInformationGroup* loadedData = loadXML(fileInfo, includedFiles);
            if (loadedData)
                addChildItem(loadedData);
        }
    //	emit dataChanged();
    updateData();
    mLoadedFiles = filelist;
}

DataInformationGroup* StructTool::loadXML(QFileInfo& fileInfo, QStringList& includedFiles)
{
    QString absoluteFilePath = fileInfo.absoluteFilePath();
    if (includedFiles.contains(absoluteFilePath))
        includedFiles.append(absoluteFilePath);
    QFile file(absoluteFilePath);
    QDomDocument doc;
    if (!file.open(QIODevice::ReadOnly))
        return NULL;
    int errorLine, errorColumn;
    QString errorMsg;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
    {
        file.close();
        kWarning() << "DataInformation::loadFromXML(): error reading file:\n" << errorMsg
                << "\n error line=" << errorLine << " error column=" << errorColumn;
        return NULL;
    }
    file.close();
    DataInformationGroup* group = new DataInformationGroup(fileInfo.fileName(), NULL);
    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    kDebug() << "root elem tag: " << docElem.tagName();
    QDomNode n = docElem.firstChild();
    while (!n.isNull())
    {
        QDomElement elem = n.toElement(); // try to convert the node to an element.
        DataInformation* data = NULL;
        if (!elem.isNull())
        {
            //			kDebug() << "element tag: " << elem.tagName();
            //e is element
            if (elem.tagName() == "include")
            {
                parseIncludeNode(elem, fileInfo, includedFiles);
            }
            else if (elem.tagName() == "struct")
                data = StructureDataInformation::fromXML(elem);
            else if (elem.tagName() == "array")
                data = StaticLengthArrayDataInformation::fromXML(elem);
            else if (elem.tagName() == "primitive")
                data = PrimitiveDataInformation::fromXML(elem);
            else if (elem.tagName() == "union")
                data = UnionDataInformation::fromXML(elem);
            //			kDebug() << "end of tag: " << elem.tagName();
        }

        if (data)
            *group << data;
        n = n.nextSibling();
    }
    return group;
}

void StructTool::parseIncludeNode(const QDomElement& elem, const QFileInfo& fileInfo,
        QStringList& includedFiles)
{
    QString includeFile = elem.attribute("file", QString::null);
    if (!includeFile.isNull())
    {
        kDebug() << "including file: " << includeFile;
        QFileInfo includeFileInfo(includeFile);
        if (includeFileInfo.isRelative())
            includeFileInfo = QFileInfo(fileInfo.absolutePath() + '/' + includeFile);

        //if contains this file, file was already included
        if (!includedFiles.contains(includeFile))
        {
            includedFiles.append(includeFile);
            DataInformationGroup* loadedIncludeData = loadXML(includeFileInfo,
                    includedFiles);
            if (loadedIncludeData)
                addChildItem(loadedIncludeData);
        }
        else
        {
            kDebug() << "included file is already loaded";
        }
    }
}
void StructTool::mark(const QModelIndex& idx)
{
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
