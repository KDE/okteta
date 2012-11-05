/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
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
#include "taggeduniondatainformation.h"

#include "topleveldatainformation.h"
#include "structuredatainformation.h"
#include "primitive/primitivedatainformation.h"
#include "../parsers/parserutils.h"

#include <KLocalizedString>

QString TaggedUnionDataInformation::typeName() const
{
    if (mLastIndex >= 0)
        return i18nc("data type in C/C++, then name", "struct %1", mAlternatives.at(mLastIndex).name);
    else
        return i18nc("data type, then name", "tagged union %1", name());
}

TaggedUnionDataInformation::~TaggedUnionDataInformation()
{
    qDeleteAll(mDefaultFields);
    Q_FOREACH(const FieldInfo& fi, mAlternatives)
    {
        qDeleteAll(fi.fields);
    }
}


TaggedUnionDataInformation::TaggedUnionDataInformation(const QString& name, DataInformation* parent)
        : DataInformationWithChildren(name, QVector<DataInformation*>(), parent), mLastIndex(-1)
{
}

TaggedUnionDataInformation::TaggedUnionDataInformation(const TaggedUnionDataInformation& d)
    : DataInformationWithChildren(d), mDefaultFields(cloneList(d.mDefaultFields, this)), mLastIndex(-1)
{
    Q_ASSERT(mDefaultFields.isEmpty() || mDefaultFields.at(0) != 0);
    Q_FOREACH(const FieldInfo& fi, d.mAlternatives)
    {
        mAlternatives.append(FieldInfo(fi.name, fi.selectIf, cloneList(fi.fields, this)));
    }
}

void TaggedUnionDataInformation::appendDefaultField(DataInformation* field, bool emitSignal)
{
    const uint oldCount = childCount();
    if (emitSignal && mLastIndex == -1)
        topLevelDataInformation()->_childCountAboutToChange(this, oldCount, oldCount + 1);
    mDefaultFields.append(field);
    if (emitSignal && mLastIndex == -1)
        topLevelDataInformation()->_childCountChanged(this, oldCount, oldCount + 1);
}

void TaggedUnionDataInformation::setAlternatives(const QVector<FieldInfo>& alternatives, bool emitSignal)
{
    const uint oldChildCount = childCount();
    mLastIndex = -1;
    const uint newChidCount = childCount();
    if (emitSignal)
        topLevelDataInformation()->_childCountAboutToChange(this, oldChildCount, newChidCount);
    //remove them all
    Q_FOREACH(const FieldInfo& fi, mAlternatives)
    {
        qDeleteAll(fi.fields);
    }
    mAlternatives.clear();
    mAlternatives = alternatives;
    //set parent
    Q_FOREACH(const FieldInfo& fi, mAlternatives)
    {
        for (int i = 0; i <  fi.fields.size(); ++i)
            fi.fields.at(i)->setParent(this);
    }
    if (emitSignal)
        topLevelDataInformation()->_childCountChanged(this, oldChildCount, newChidCount);
}

int TaggedUnionDataInformation::determineSelection(TopLevelDataInformation* top)
{
    //now find out which one of the alternatives to select
    for (int i = 0; i < mAlternatives.size(); ++i)
    {
        const FieldInfo& fi = mAlternatives.at(i);
        if (fi.selectIf.isFunction())
        {
            QScriptValue result = top->scriptHandler()->callFunction(fi.selectIf, this,
                    ScriptHandlerInfo::TaggedUnionSelection);
            if (!result.isBool())
            {
                logError() << "Evaluating select function for alternative" << i
                        << "did not return a boolean value. Got the following instead:" << result.toString();
                continue;
            }
            if (result.toBool())
            {
                return i;
            }
        }
        else
        {
            ParsedNumber<quint64> number = ParserUtils::uint64FromScriptValue(fi.selectIf);
            if (!number.isValid)
            {
                logError() << "Alternative number" << i << "is not valid. SelectIf"
                        " is neither function nor number:" << fi.selectIf.toString();
                continue;

            }
            //number is valid -> there must be exactly one field
            if (mChildren.size() != 1)
            {
                logError() << "Alternative number" << i << "is not valid. SelectIf is number, but there is not exactly one child!";
                continue;
            }
            if (!mChildren.at(0)->isPrimitive())
            {
                logError() << "Alternative number" << i << "is not valid. SelectIf is number, but only child is not primitive!";
                continue;
            }
            if (mChildren.at(0)->asPrimitive()->value() == number.value)
                return i; //found it
        }
    }
    return -1;
}

qint64 TaggedUnionDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated);
    //update must have been called prior to reading
    TopLevelDataInformation* top = topLevelDataInformation();
    Q_CHECK_PTR(top);

    qint64 readBits = 0;
    mWasAbleToRead = StructureDataInformation::readChildren(mChildren,
            input, address, bitsRemaining, bitOffset, &readBits, top);
    mLastIndex = determineSelection(top);
    const QVector<DataInformation*>& others = currentChildren();
    //this is important since the remaining children might have changed since before the read
    //where beginRead was called on the children at that time
    for (int i = 0; i < others.size(); i++)
        others.at(i)->beginRead();

    if (!mWasAbleToRead)
    {
        Q_ASSERT(readBits == -1);
        return -1;
    }
    //otherwise continue reading the remaining children
    mWasAbleToRead = StructureDataInformation::readChildren(others,
            input, address, bitsRemaining, bitOffset, &readBits, top);
    return readBits;
}

BitCount64 TaggedUnionDataInformation::childPosition(const DataInformation* child,
        Okteta::Address start) const
{
    BitCount64 offset = 0;
    bool found = false;
    //sum size of elements up to index
    for (int i = 0; i < mChildren.size(); ++i)
    {
        DataInformation* current = mChildren.at(i);
        if (current == child)
        {
            found = true;
            break;
        }
        offset += current->size();
    }
    if (!found)
    {
        const QVector<DataInformation*> others = currentChildren();
        for (int i = 0; i < others.size(); ++i)
        {
            DataInformation* current = others.at(i);
            if (current == child)
            {
                found = true;
                break;
            }
            offset += current->size();
        }
    }
    Q_ASSERT(found);
    if (mParent->isTopLevel())
        return start * 8 + offset;
    else
        return mParent->asDataInformation()->childPosition(this, start) + offset;
}

BitCount32 TaggedUnionDataInformation::size() const
{
    BitCount32 total = 0;
    for (int i = 0; i < mChildren.size(); ++i)
        total += mChildren.at(i)->size();
    const QVector<DataInformation*> others = currentChildren();
    for (int i = 0; i < others.size(); ++i)
        total += others.at(i)->size();
    return total;
}

bool TaggedUnionDataInformation::replaceChildAt(unsigned int index, DataInformation* newChild)
{
    Q_ASSERT(false); //TODO implement
    return false;
}

int TaggedUnionDataInformation::indexOf(const DataInformation* const data) const
{
    int index = 0;
    for (int i = 0; i < mChildren.size(); ++i)
    {
        if (mChildren.at(i) == data)
            return index;
        index++;
    }
    const QVector<DataInformation*> others = currentChildren();
    for (int i = 0; i < others.size(); ++i)
    {
        if (others.at(i) == data)
            return index;
        index++;
    }
    Q_ASSERT(false); //should never land here
    return -1;
}

DataInformation* TaggedUnionDataInformation::childAt(unsigned int index) const
{
    const uint permanentChildCount = uint(mChildren.size());
    if (index < permanentChildCount)
        return mChildren.at(index);
    const QVector<DataInformation*> others = currentChildren();
    if (index < permanentChildCount + others.size())
        return others.at(index - permanentChildCount);
    Q_ASSERT(false); //should never happen
    return 0;
}

unsigned int TaggedUnionDataInformation::childCount() const
{
    return mChildren.size() + currentChildren().size();
}
