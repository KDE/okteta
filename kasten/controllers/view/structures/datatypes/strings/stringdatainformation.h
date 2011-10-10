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



#ifndef STRINGDATAINFORMATION_H
#define STRINGDATAINFORMATION_H

#include "../datainformation.h"
#include "stringdata.h"

#include <QtCore/QFlags>

class DummyDataInformation;

const QLatin1String stringEncodings[] = {
    QLatin1String("ascii"), QLatin1String("latin1"), QLatin1String("utf-8"), QLatin1String("utf-16le"),
    QLatin1String("utf-16-be"), QLatin1String("utf32-le"), QLatin1String("utf32-be")
};

class StringDataInformation : public DataInformation
{
protected:
    virtual BitCount32 offset(unsigned int index) const;
    StringDataInformation(const StringDataInformation&);
public:
    enum StringType {
        InvalidEncoding = -1, ASCII = 0, Latin1, UTF8, UTF16_LE, UTF16_BE, UTF32_LE, UTF32_BE
    };

    StringDataInformation(const QString& name, StringType encoding, DataInformationBase* parent = 0);
    StringDataInformation(const QString& name, QString encoding, DataInformationBase* parent = 0);
    virtual ~StringDataInformation();
    DATAINFORMATION_CLONE(String)

    virtual bool canHaveChildren() const;
    virtual DataInformation* childAt(unsigned int) const;

    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);

    virtual BitCount32 size() const;
    virtual void setWidgetData(QWidget* w) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QString typeName() const;
    virtual unsigned int childCount() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    virtual bool isString() const;

    virtual QVariant data(int column, int role) const;
    virtual QVariant childData(int row, int column, int role) const;
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const;
    virtual BitCount32 childSize(int index) const;

    virtual QString valueString() const;
    virtual QString name() const;
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);

    StringType encoding() const;
    void setEncoding(QString encoding);
    void setEncoding(StringType encoding);
    DummyDataInformation* dummy() const;
    uint terminationCodePoint() const;
    void setTerminationCodePoint(uint term);
    void setTerminationCodePoint(const QScriptValue& value, QScriptEngine* engine);
    uint maxCharCount() const;
    void setMaxCharCount(uint count);
    void setMaxCharCount(const QScriptValue& value, QScriptEngine* engine);
    uint maxByteCount() const;
    void setMaxByteCount(uint count);
    void setMaxByteCount(const QScriptValue& value, QScriptEngine* engine);
    int stringLength() const;
    int stringByteLength() const;
    uint terminationMode() const;
    QString valueAt(int index) const;
private:
    DummyDataInformation* mDummy;
    StringData* mData;
    StringType mEncoding;
};

inline bool StringDataInformation::canHaveChildren() const
{
    return true;
}

inline DummyDataInformation* StringDataInformation::dummy() const
{
    return mDummy;
}

inline StringDataInformation::StringType StringDataInformation::encoding() const
{
    return mEncoding;
}

inline uint StringDataInformation::maxByteCount() const
{
    return mData->maxByteCount();
}

inline void StringDataInformation::setMaxByteCount(uint count)
{
    mData->setMaxByteCount(count);
}

inline uint StringDataInformation::maxCharCount() const
{
    return mData->terminationCodePoint();
}

inline void StringDataInformation::setMaxCharCount(uint count)
{
    mData->setMaxCharCount(count);
}

inline uint StringDataInformation::terminationCodePoint() const
{
    return mData->terminationCodePoint();
}

inline void StringDataInformation::setTerminationCodePoint(uint term)
{
    mData->setTerminationCodePoint(term);
}

inline int StringDataInformation::stringLength() const
{
    return mData->count();
}

inline int StringDataInformation::stringByteLength() const
{
    return mData->size() / 8;
}

inline uint StringDataInformation::terminationMode() const
{
    return mData->terminationMode();
}

inline QString StringDataInformation::valueAt(int index) const
{
    Q_ASSERT(index >= 0 && index < mData->count());
    return mData->stringValue(index);
}

#endif // STRINGDATAINFORMATION_H
