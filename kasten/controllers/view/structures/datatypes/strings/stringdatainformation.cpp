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
#include "../topleveldatainformation.h"
#include "utf32stringdata.h"
#include "utf16stringdata.h"
#include "utf8stringdata.h"
#include "asciistringdata.h"
#include "latin1stringdata.h"
#include "../../script/classes/stringscriptclass.h"
#include "../../script/scripthandlerinfo.h"

#include <QtScript/QScriptEngine>
#include <QtGui/QBrush>

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

BitCount32 StringDataInformation::offset(unsigned int index) const
{
    Q_ASSERT(index < (unsigned)mData->count());
    BitCount32 offs = 0;
    for (uint i = 0; i < index; ++i)
    {
        offs += mData->sizeAt(i);
    }
    return offs;
}


bool StringDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "StringDataInformation::setData()", "this should never be called!");
    return false;
}

bool StringDataInformation::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    kDebug() << "not implemented yet!";
    return false;
}


qint64 StringDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
        BitCount64 bitsRemaining, quint8* bitOffset)
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

    topLevelDataInformation()->updateElement(this); //update before reading!

    return mData->read(input, address, bitsRemaining);
}

BitCount32 StringDataInformation::size() const
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
    return mData->completeString();
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
    if ((mEncoding == UTF16_LE && encoding == UTF16_BE) || (mEncoding == UTF16_BE || encoding == UTF16_LE))
    {
        //only set endianess, since is already utf 16
        mData->setLittleEndian(encoding == UTF16_LE);
    }
    else if ((mEncoding == UTF32_LE && encoding == UTF32_BE) || (mEncoding == UTF32_BE && encoding == UTF32_LE))
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
            data->copyTerminationFrom(mData);
        delete mData;
        mData = data;
    }
    mEncoding = encoding;
}

QScriptValue StringDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mStringScriptClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

BitCount32 StringDataInformation::childSize(int index) const
{
    return mData->sizeAt(index);
}

void StringDataInformation::setMaxByteCount(const QScriptValue& value, QScriptEngine* engine)
{
    if (value.isNull())
    {
        //clear the mode and set to null terminated of none is left
        mData->setTerminationMode(mData->terminationMode() & ~StringData::ByteCount);
        if (mData->terminationMode() == StringData::None)
            mData->setTerminationCodePoint(0);
    }
    else if (value.isNumber())
    {
        mData->setMaxByteCount(value.toUInt32());
    }
    else
    {
        engine->currentContext()->throwError(QScriptContext::TypeError,
            QLatin1String("Setting max byte count: value was not a number."));
    }
}

void StringDataInformation::setMaxCharCount(const QScriptValue& value, QScriptEngine* engine)
{
    if (value.isNull())
    {
        //clear the mode and set to null terminated of none is left
        mData->setTerminationMode(mData->terminationMode() & ~StringData::CharCount);
        if (mData->terminationMode() == StringData::None)
            mData->setTerminationCodePoint(0);
    }
    else if (value.isNumber())
    {
        mData->setMaxCharCount(value.toUInt32());
    }
    else
    {
        engine->currentContext()->throwError(QScriptContext::TypeError,
            QLatin1String("Setting max char count: value was not a number."));
    }
}

void StringDataInformation::setTerminationCodePoint(const QScriptValue& value, QScriptEngine* engine)
{
    uint cp = 0;
    if (value.isNull())
    {
        //clear the mode and set to null terminated of none is left
        mData->setTerminationMode(mData->terminationMode() & ~StringData::Sequence);
        if (mData->terminationMode() == StringData::None)
            mData->setTerminationCodePoint(0);
    }
    else if (value.isString()) {
        QString str = value.toString();
        if (str.length() != 1) {
            engine->currentContext()->throwError(QScriptContext::RangeError,
                QLatin1String("Setting termination char: expected one char or a number, got a string with length ")
                + QString::number(str.length()));
            return;
        }
        cp = str[0].unicode();
    }
    else if (value.isNumber())
    {
        cp = value.toUInt32();
    }
    else
    {
        engine->currentContext()->throwError(QScriptContext::TypeError,
            QLatin1String("Setting termination char: value was not a number."));
        return;
    }
    if (cp > StringData::UNICODE_MAX)
            engine->currentContext()->throwError(QLatin1String("Setting termination char: U+")
                + QString::number(cp, 16) + QLatin1String("is out of unicode range!"));
    else
        mData->setTerminationCodePoint(cp);
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
