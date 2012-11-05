/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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



#include "stringdatainformation.h"
#include "../dummydatainformation.h"
#include "../topleveldatainformation.h"
#include "utf32stringdata.h"
#include "utf16stringdata.h"
#include "utf8stringdata.h"
#include "asciistringdata.h"
#include "latin1stringdata.h"
#include "../../script/classes/stringscriptclass.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/scriptlogger.h"

#include <QScriptEngine>
#include <QBrush>
#include <QString>

#include <KLocale>

StringDataInformation::StringDataInformation(const QString& name, StringType encoding, DataInformationBase* parent)
    : DataInformationWithDummyChildren(name, parent), mDummy(new DummyDataInformation(this)), mData(0), mEncoding(InvalidEncoding)
{
    setEncoding(encoding); //sets mData
}

StringDataInformation::StringDataInformation(const StringDataInformation& d)
    : DataInformationWithDummyChildren(d), mDummy(new DummyDataInformation(this)), mData(0), mEncoding(InvalidEncoding)
{
    setEncoding(d.mEncoding); //sets mData
    mData->copyTerminationFrom(d.mData.data());
}

StringDataInformation::~StringDataInformation()
{
}

DataInformation* StringDataInformation::childAt(unsigned int index) const
{
	Q_ASSERT(index < childCount());
	mDummy->setDummyIndex(index);
    return mDummy.data();
}

bool StringDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "StringDataInformation::setData()", "this should never be called!");
    return false;
}

bool StringDataInformation::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    logWarn() << "setChildData not implemented yet!";
    return false;
}


qint64 StringDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    if (*bitOffset != 0)
    {
        logWarn() << "while reading string bit offset was: " << *bitOffset
                << ", adding padding and continuing at next byte (address=" << address << ")";
        bitsRemaining -= 8 - *bitOffset;
        *bitOffset = 0;
        address += 1;
        Q_ASSERT((bitsRemaining % 8) == 0); //must be mod 8
    }
    qint64 ret = mData->read(input, address, bitsRemaining);
    mWasAbleToRead = ret >= 0;
    return ret;
}

BitCount32 StringDataInformation::size() const
{
    return mData->size();
}

void StringDataInformation::setWidgetData(QWidget*) const
{
    //TODO
}

QVariant StringDataInformation::dataFromWidget(const QWidget*) const
{
    //TODO
    Q_ASSERT(false);
    return QVariant();
}

QWidget* StringDataInformation::createEditWidget(QWidget*) const
{
    //TODO
    Q_ASSERT(false);
    return 0;
}

QString StringDataInformation::typeName() const
{
    return mData->typeName();
}

unsigned int StringDataInformation::childCount() const
{
    return mData->count();
}

Qt::ItemFlags StringDataInformation::flags(int column, bool fileLoaded) const
{
    return DataInformation::flags(column, fileLoaded);
}

QVariant StringDataInformation::childData(int row, int column, int role) const
{
    Q_ASSERT(row >= 0 && (unsigned)row < childCount());
    Q_ASSERT(column < COLUMN_COUNT);
    if (role == Qt::DisplayRole)
    {
        if (column == ColumnName)
        {
            //TODO termination char
            return QString(QLatin1Char('[') + QString::number(row) + QLatin1Char(']'));
        }
        else if (column == ColumnType)
        {
            return mData->charType();
        }
        else if (column == ColumnValue)
        {
            return mData->stringValue(row);
        }
    }
    //TODO mark eof reached, don't add extra item. i.e. add icon or colour
    return QVariant();
}

QString StringDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    return mData->completeString();
}

Qt::ItemFlags StringDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    Q_UNUSED(fileLoaded);
    Q_UNUSED(row);
    Q_UNUSED(column);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled; //not editable atm
    //TODO make editable
}

void StringDataInformation::setEncoding(StringDataInformation::StringType encoding)
{
    if (mData && mEncoding == encoding)
        return;
    if (mData && ((mEncoding == UTF16_LE && encoding == UTF16_BE) || (mEncoding == UTF16_BE || encoding == UTF16_LE)))
    {
        //only set endianess, since is already utf 16
        mData->setLittleEndian(encoding == UTF16_LE);
    }
    else if (mData && ((mEncoding == UTF32_LE && encoding == UTF32_BE) || (mEncoding == UTF32_BE && encoding == UTF32_LE)))
    {
        //only set endianess, since is already utf 32
        mData->setLittleEndian(encoding == UTF32_LE);
    }
    else
    {
        StringData* data = 0;
        switch (encoding) {
            case ASCII:
                data = new AsciiStringData(this);
                break;
            case Latin1:
                data = new Latin1StringData(this);
                break;
            case UTF8:
                data = new Utf8StringData(this);
                break;
            case UTF16_LE:
                data = new Utf16StringData(this);
                data->setLittleEndian(true);
                break;
            case UTF16_BE:
                data = new Utf16StringData(this);
                data->setLittleEndian(false);
                break;
            case UTF32_LE:
                data = new Utf32StringData(this);
                data->setLittleEndian(true);
                break;
            case UTF32_BE:
                data = new Utf32StringData(this);
                data->setLittleEndian(false);
                break;
            default:
                data = new AsciiStringData(this); //TODO add the other classes
                break;
        }
        if (mData)
            data->copyTerminationFrom(mData.data());
        mData.reset(data);
    }
    mEncoding = encoding;
}
//TODO implement string editing

BitCount32 StringDataInformation::childSize(uint index) const
{
    return mData->sizeAt(index);
}

QString StringDataInformation::childTypeName(uint index) const
{
    Q_UNUSED(index)
    return QString(); //XXX should there be something here?
}

void StringDataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT(false);
    Q_ASSERT(index < mData->count());
}

QVariant StringDataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT(index < mData->count());
    Q_ASSERT(false);
    return QVariant();
}

QWidget* StringDataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT(false);
    return 0;
}

QScriptValue StringDataInformation::childToScriptValue(uint index, QScriptEngine*, ScriptHandlerInfo*) const
{
    //just return as a string
    return mData->stringValue(index);
}

BitCount64 StringDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    Q_UNUSED(start)
    Q_ASSERT(child->isDummy());
    Q_ASSERT(child->parent() == this);
    Q_ASSERT(child == mDummy.data());
    uint index = mDummy->dummyIndex();
    Q_ASSERT(index < mData->count());
    BitCount32 offs = 0;
    for (uint i = 0; i < index; ++i)
    {
        offs += mData->sizeAt(i);
    }
    return offs;
}

QVariant StringDataInformation::data(int column, int role) const
{
    if (mData->wasEof())
    {
        if (role == Qt::BackgroundRole)
            return QBrush(Qt::yellow);
        else if (role == Qt::ToolTipRole)
            return i18n("End of file reached prematurely");
    }
    return DataInformation::data(column, role);
}

bool StringDataInformation::isString() const
{
    return true;
}

void StringDataInformation::unsetTerminationMode(StringData::TerminationMode mode)
{
    //clear the mode and set to null terminated of none is left
    mData->setTerminationMode(StringData::TerminationMode(mData->terminationMode() & ~mode));
    if (mData->terminationMode() == StringData::None)
        mData->setTerminationCodePoint(0);
}

QScriptClass* StringDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mStringClass.data();
}
