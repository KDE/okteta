/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include "utf16stringdata.h"
#include "asciistringdata.h"

StringDataInformation::StringDataInformation(const QString& name, StringType encoding, DataInformationBase* parent)
    : DataInformation(name, parent), mDummy(new DummyDataInformation(this)), mData(0), mEncoding(InvalidEncoding)
{
    setEncoding(encoding); //sets mData
}

StringDataInformation::StringDataInformation(const QString& name, QString encoding, DataInformationBase* parent)
    : DataInformation(name, parent), mDummy(new DummyDataInformation(this)), mData(0), mEncoding(InvalidEncoding)
{
    setEncoding(encoding); //sets mData
}

StringDataInformation::StringDataInformation(const StringDataInformation& d)
    : DataInformation(d), mDummy(new DummyDataInformation(this)), mData(0), mEncoding(InvalidEncoding)
{
    setEncoding(d.mEncoding); //sets mData
    mData->copyTerminationFrom(d.mData);
}

StringDataInformation::~StringDataInformation()
{
    delete mData;
    delete mDummy;
}

DataInformation* StringDataInformation::childAt(unsigned int ) const
{
    return mDummy;
}

quint64 StringDataInformation::offset(unsigned int index) const
{
    Q_ASSERT(index < mData->count());
    quint64 offs = 0;
    for (uint i = 0; i < index; ++i)
    {
        offs += mData->sizeAt(i);
    }
    return offs;
}

bool StringDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    //TODO
    Q_ASSERT(false);
    return false;
}

qint64 StringDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        quint64 bitsRemaining, quint8* bitOffset)
{
    if (*bitOffset != 0)
    {
        kDebug() << "while reading string bit offset was: " << *bitOffset
                << ", adding padding and continuing at next byte (address=" << address << ")";
        bitsRemaining -= 8 - *bitOffset;
        *bitOffset = 0;
        address += 1;
        Q_ASSERT((bitsRemaining % 8) == 0); //must be mod 8
    }
    return mData->read(input, address, bitsRemaining);
}

int StringDataInformation::size() const
{
    return mData->size();
}

void StringDataInformation::setWidgetData(QWidget* w) const
{
    //TODO
}

QVariant StringDataInformation::dataFromWidget(const QWidget* w) const
{
    //TODO
    Q_ASSERT(false);
    return QVariant();
}

QWidget* StringDataInformation::createEditWidget(QWidget* parent) const
{
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
            return '[' + QString::number(row) + ']';
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
    return DataInformation::valueString();
}

QString StringDataInformation::name() const
{
    return DataInformation::name();
}

Qt::ItemFlags StringDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    Q_UNUSED(fileLoaded);
    Q_UNUSED(row);
    Q_UNUSED(column);
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled; //not editable atm
    //TODO make editable
}

void StringDataInformation::setEncoding(QString encodingStr)
{
    QString enc = encodingStr.toLower();
    StringType encoding;
    if (enc == QLatin1String("ascii"))
        encoding = ASCII;
    else if (enc.startsWith(QLatin1String("latin"))) //allow latin-1 and latin1
        encoding = Latin1;
    else if (enc.startsWith(QLatin1String("utf-16")))
    {
        if (enc.endsWith(QLatin1String("be")))
            encoding = UTF16_BE;
        else
            encoding = UTF16_LE;
    }
    else if (enc.startsWith(QLatin1String("utf-32")))
    {
        if (enc.endsWith(QLatin1String("be")))
            encoding = UTF32_BE;
        else
            encoding = UTF32_LE;
    }
    else if (enc.startsWith(QLatin1String("utf-8")))
        encoding = UTF8;
    else
    {
        kDebug() << "invalid encoding set: " << encodingStr << "defaulting to ASCII";
        encoding = ASCII;
    }
    setEncoding(encoding);
}

void StringDataInformation::setEncoding(StringDataInformation::StringType encoding)
{
    if (mEncoding == encoding)
        return;
    if ((encoding == UTF16_LE || encoding == UTF16_BE) && (mEncoding == UTF16_LE || mEncoding == UTF16_BE))
    {
        //only set endianess
        mData->setEndianess(encoding == UTF16_LE);
    }
    else if ((encoding == UTF32_LE || encoding == UTF32_BE) && (mEncoding == UTF32_LE || mEncoding == UTF32_BE))
    {
        //only set endianess
        mData->setEndianess(encoding == UTF32_LE);
    }
    else
    {
        delete mData;
        switch (encoding) {
            case ASCII:
                mData = new AsciiStringData(this);
                break;
            case UTF16_LE:
                mData = new Utf16StringData(this);
                mData->setEndianess(true);
                break;
            case UTF16_BE:
                mData = new Utf16StringData(this);
                mData->setEndianess(false);
                break;
            default:
                mData = new AsciiStringData(this); //TODO add the other classes
                break;
        }
    }
    mEncoding = encoding;
}

QScriptValue StringDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    //TODO
    return QScriptValue();
}

int StringDataInformation::childSize(int index) const
{
    return mData->sizeAt(index);
}
