/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "taggeduniondatainformation.hpp"

// lib
#include "topleveldatainformation.hpp"
#include "structuredatainformation.hpp"
#include <primitivedatainformation.hpp>
#include <parserutils.hpp>
// KF
#include <KLocalizedString>
// Std
#include <utility>

TaggedUnionDataInformation::TaggedUnionDataInformation(const QString& name, DataInformation* parent)
    : DataInformationWithChildren(name, std::vector<std::unique_ptr<DataInformation>>(), parent)
{
}

TaggedUnionDataInformation::TaggedUnionDataInformation(const TaggedUnionDataInformation& d)
    : DataInformationWithChildren(d)
    , mDefaultFields(cloneList(d.mDefaultFields, this))
{
    Q_ASSERT(mDefaultFields.empty() || mDefaultFields.at(0) != nullptr);
    mAlternatives.reserve(d.mAlternatives.size());
    for (const FieldInfo& fi : d.mAlternatives) {
        mAlternatives.emplace_back(FieldInfo(fi.name, fi.selectIf, cloneList(fi.fields, this)));
    }
}

TaggedUnionDataInformation::~TaggedUnionDataInformation() = default;

QString TaggedUnionDataInformation::typeNameImpl() const
{
    if (mLastIndex >= 0) {
        return i18nc("data type in C/C++, then name", "struct %1", mAlternatives.at(mLastIndex).name);
    }

    return i18nc("data type, then name", "tagged union %1", name());
}

void TaggedUnionDataInformation::appendDefaultField(DataInformation* field, bool emitSignal)
{
    const uint oldCount = childCount();
    if (emitSignal && mLastIndex == -1) {
        topLevelDataInformation()->_childCountAboutToChange(this, oldCount, oldCount + 1);
    }
    mDefaultFields.emplace_back(field);
    if (emitSignal && mLastIndex == -1) {
        topLevelDataInformation()->_childCountChanged(this, oldCount, oldCount + 1);
    }
}

void TaggedUnionDataInformation::setAlternatives(std::vector<FieldInfo>&& alternatives, bool emitSignal)
{
    const uint oldChildCount = childCount();
    mLastIndex = -1;
    const uint newChidCount = childCount();
    if (emitSignal) {
        topLevelDataInformation()->_childCountAboutToChange(this, oldChildCount, newChidCount);
    }

    mAlternatives = std::move(alternatives);

    // set parent
    for (const FieldInfo& fi : mAlternatives) {
        for (const auto& field : fi.fields) {
            field->setParent(this);
        }
    }

    if (emitSignal) {
        topLevelDataInformation()->_childCountChanged(this, oldChildCount, newChidCount);
    }
}

int TaggedUnionDataInformation::determineSelection(TopLevelDataInformation* top)
{
    // now find out which one of the alternatives to select
    for (std::size_t i = 0; i < mAlternatives.size(); ++i) {
        const FieldInfo& fi = mAlternatives.at(i);
        if (fi.selectIf.isFunction()) {
            QScriptValue result = top->scriptHandler()->callFunction(fi.selectIf, this,
                                                                     ScriptHandlerInfo::Mode::TaggedUnionSelection);
            if (!result.isBool()) {
                logError() << "Evaluating select function for alternative" << i
                           << "did not return a boolean value. Got the following instead:" << result.toString();
                continue;
            }
            if (result.toBool()) {
                return i;
            }
        } else {
            ParsedNumber<quint64> number = ParserUtils::uint64FromScriptValue(fi.selectIf);
            if (!number.isValid) {
                logError() << "Alternative number" << i << "is not valid. SelectIf"
                    " is neither function nor number:" << fi.selectIf.toString();
                continue;

            }
            // number is valid -> there must be exactly one field
            if (mChildren.size() != 1) {
                logError() << "Alternative number" << i << "is not valid. SelectIf is number, but there is not exactly one child!";
                continue;
            }
            if (!mChildren.front()->isPrimitive()) {
                logError() << "Alternative number" << i << "is not valid. SelectIf is number, but only child is not primitive!";
                continue;
            }
            if (mChildren.front()->asPrimitive()->value() == number.value) {
                return i; // found it
            }
        }
    }

    return -1;
}

qint64 TaggedUnionDataInformation::readData(const Okteta::AbstractByteArrayModel* input,
                                            Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated);
    // update must have been called prior to reading
    TopLevelDataInformation* top = topLevelDataInformation();
    Q_CHECK_PTR(top);

    const std::vector<std::unique_ptr<DataInformation>>& oldChildren = currentChildren();

    qint64 readBits = 0;
    mWasAbleToRead = readChildren(mChildren, input, address, bitsRemaining, bitOffset, &readBits, top);
    mLastIndex = determineSelection(top);
    const std::vector<std::unique_ptr<DataInformation>>& others = currentChildren();
    // check whether we have different children now, if yes we have to emit child count changed
    if (oldChildren != others) {
        const std::size_t fixedSize = mChildren.size();
        // tell the model that all children have changed by setting to 0 and then to new size
        top->_childCountAboutToChange(this, fixedSize + oldChildren.size(), fixedSize);
        top->_childCountChanged(this, fixedSize + oldChildren.size(), fixedSize);
        top->_childCountAboutToChange(this, fixedSize, fixedSize + others.size());
        top->_childCountChanged(this, fixedSize, fixedSize + others.size());
    }

    // this is important since the remaining children might have changed since before the read
    // where beginRead was called on the children at that time
    for (const auto& other : others) {
        other->beginRead();
    }

    if (!mWasAbleToRead) {
        Q_ASSERT(readBits == -1);
        return -1;
    }
    // otherwise continue reading the remaining children
    mWasAbleToRead = StructureDataInformation::readChildren(others,
                                                            input, address, bitsRemaining, bitOffset, &readBits, top);
    return readBits;
}

BitCount64 TaggedUnionDataInformation::childPosition(const DataInformation* child,
                                                     Okteta::Address start) const
{
    BitCount64 offset = 0;
    bool found = false;
    // sum size of elements up to index
    for (const auto& current : mChildren) {
        if (current.get() == child) {
            found = true;
            break;
        }
        offset += current->size();
    }

    if (!found) {
        const std::vector<std::unique_ptr<DataInformation>>& others = currentChildren();
        for (const auto& current : others) {
            if (current.get() == child) {
                found = true;
                break;
            }
            offset += current->size();
        }
    }
    Q_ASSERT(found);
    if (mParent->isTopLevel()) {
        return start * 8 + offset;
    }

    return mParent->asDataInformation()->childPosition(this, start) + offset;
}

BitCount32 TaggedUnionDataInformation::size() const
{
    BitCount32 total = 0;
    for (const auto& child : mChildren) {
        total += child->size();
    }

    const std::vector<std::unique_ptr<DataInformation>>& others = currentChildren();
    for (const auto& other : others) {
        total += other->size();
    }

    return total;
}

bool TaggedUnionDataInformation::replaceChildAt(unsigned int index, std::unique_ptr<DataInformation>&& newChild)
{
    Q_ASSERT(false); // TODO implement
    Q_UNUSED(index)
    Q_UNUSED(newChild)
    return false;
}

int TaggedUnionDataInformation::indexOf(const DataInformation* const data) const
{
    int index = 0;
    for (const auto& child : mChildren) {
        if (child.get() == data) {
            return index;
        }
        index++;
    }

    const std::vector<std::unique_ptr<DataInformation>>& others = currentChildren();
    for (const auto& other : others) {
        if (other.get() == data) {
            return index;
        }
        index++;
    }

    Q_ASSERT(false); // should never land here
    return -1;
}

DataInformation* TaggedUnionDataInformation::childAt(unsigned int index) const
{
    const std::size_t permanentChildCount = mChildren.size();
    if (index < permanentChildCount) {
        return mChildren[index].get();
    }
    const std::vector<std::unique_ptr<DataInformation>>& others = currentChildren();
    if (index < permanentChildCount + others.size()) {
        return others[index - permanentChildCount].get();
    }
    Q_ASSERT(false); // should never happen
    return nullptr;
}

unsigned int TaggedUnionDataInformation::childCount() const
{
    return mChildren.size() + currentChildren().size();
}

bool TaggedUnionDataInformation::isTaggedUnion() const
{
    return true;
}
