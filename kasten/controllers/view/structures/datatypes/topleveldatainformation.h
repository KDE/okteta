/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef TOPLEVELDATAINFORMATION_H_
#define TOPLEVELDATAINFORMATION_H_

#include "datainformation.h"

#include <QtCore/QFileInfo>
#include <QtCore/QExplicitlySharedDataPointer>

class QScriptEngine;
class ScriptHandler;

class TopLevelDataInformation: public DataInformation
{
Q_OBJECT
public:
    /** creates a new TopLevelDataInformation object wrapping the existing DataInformation @p data.
     *  @param data the object to wrap
     *  @param scriptFile the file which contains the file this object was initialized from
     *  @param dynamic whether the wrapped object is a dynamic structure definition (i.e. one using QtScript)
     */
    TopLevelDataInformation(const DataInformation& data, QFileInfo structureFile,
            bool dynamic = false, QString name = QString());
    TopLevelDataInformation(const TopLevelDataInformation& d);
    virtual ~TopLevelDataInformation();
    DATAINFORMATION_CLONE(TopLevel)

public:
    //reimplement all methods from DataInformation (forward to mData)
    virtual bool hasChildren() const;
    virtual unsigned int childCount() const;
    virtual DataInformation* childAt(unsigned int) const;
    virtual void setIndex(int newIndex);
    virtual quint64 positionRelativeToParent() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    virtual QVariant data(int column, int role) const;
    virtual QString typeName() const;
    virtual QString valueString() const;
    virtual QString sizeString() const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual qint64
    readData(Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);
    virtual int size() const;
    virtual bool wasAbleToRead() const;
    virtual bool isDynamicArray() const;
    virtual TopLevelDataInformation* topLevelDataInformation();
    void validate();
    QScriptEngine* scriptEngine() const;

    DataInformation* actualDataInformation();
    bool wasAbleToParse() const;
protected:
    virtual quint64 offset(unsigned int index) const;

public:
    //the new methods of topLevelDataInformation

private:
    DataInformation* mData;
    QExplicitlySharedDataPointer<ScriptHandler> mScriptHandler;
    QFileInfo mStructureFile;
    bool mWasAbleToParse :1;
};

//just forward the call to mData:
inline int TopLevelDataInformation::size() const
{
    return mData->size();
}
inline bool TopLevelDataInformation::wasAbleToRead() const
{
    return mData->wasAbleToRead();
}
inline bool TopLevelDataInformation::hasChildren() const
{
    return mData->hasChildren();
}
inline unsigned int TopLevelDataInformation::childCount() const
{
    return mData->childCount();
}
inline DataInformation* TopLevelDataInformation::childAt(unsigned int idx) const
{
    return mData->childAt(idx);
}
inline void TopLevelDataInformation::setIndex(int newIndex)
{
    mData->setIndex(newIndex);
    mIndex = newIndex;
}
inline quint64 TopLevelDataInformation::positionRelativeToParent() const
{
    return 0; // highest level
}
inline Qt::ItemFlags TopLevelDataInformation::flags(int column, bool fileLoaded) const
{
    return mData->flags(column, fileLoaded);
}
inline QVariant TopLevelDataInformation::data(int column, int role) const
{
    return mData->data(column, role);
}
inline QString TopLevelDataInformation::typeName() const
{
    return mData->typeName();
}
inline QString TopLevelDataInformation::valueString() const
{
    return mData->valueString();
}
inline QString TopLevelDataInformation::sizeString() const
{
    return mData->sizeString();
}
inline QWidget* TopLevelDataInformation::createEditWidget(QWidget* parent) const
{
    return mData->createEditWidget(parent);
}
inline QVariant TopLevelDataInformation::dataFromWidget(const QWidget* w) const
{
    return mData->dataFromWidget(w);
}
inline void TopLevelDataInformation::setWidgetData(QWidget* w) const
{
    return mData->setWidgetData(w);
}
inline quint64 TopLevelDataInformation::offset(unsigned int idx) const
{
    Q_UNUSED(idx)
    return 0; //this is the first element, offset must always be 0
}
inline DataInformation* TopLevelDataInformation::actualDataInformation()
{
    return mData;
}
inline bool TopLevelDataInformation::isDynamicArray() const
{
    return mData->isDynamicArray();
}
inline bool TopLevelDataInformation::wasAbleToParse() const
{
    return mWasAbleToParse;
}


#endif /* TOPLEVELDATAINFORMATION_H_ */
