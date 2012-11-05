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
#ifndef TAGGEDUNIONDATAINFORMATION_H_
#define TAGGEDUNIONDATAINFORMATION_H_

#include "datainformationwithchildren.h"
#include "../allprimitivetypes.h"

#include <QScriptValue>

/** A class holding the data of a struct for Okteta*/
class TaggedUnionDataInformation : public DataInformationWithChildren
{
    DATAINFORMATION_CLONE_DECL(TaggedUnionDataInformation, DataInformationWithChildren);
public:
    /** If selector is a function and it evaluates to true these fields are chosen.
     *  If selector is a number then there must be exactly one field before that is then compared
     *  with the value and if identical the fields are chosen
     *  If name is not empty it is used instead of the name of this tagged union */
    struct FieldInfo
    {
        inline explicit FieldInfo(const QString& n, const QScriptValue& s, const QVector<DataInformation*>& f)
                : name(n), selectIf(s), fields(f) {}
        FieldInfo() {};
        /** The name this tagged union gets when this is selected */
        QString name;
        QScriptValue selectIf;
        QVector<DataInformation*> fields;
    };

    TaggedUnionDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~TaggedUnionDataInformation();

    virtual qint64 readData(Okteta::AbstractByteArrayModel *input,
            Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset);
    virtual bool isTaggedUnion() const;
    //implement the DataInformation pure virtual functions
    QString typeName() const;
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const;

    //reimplement these
    virtual BitCount32 size() const;

    virtual bool replaceChildAt(unsigned int index, DataInformation* newChild);
    virtual int indexOf(const DataInformation* const data) const;

    virtual DataInformation* childAt(unsigned int index) const;
    virtual unsigned int childCount() const;
    void appendDefaultField(DataInformation* field, bool emitSignal);
    void setAlternatives(const QVector<FieldInfo>& alternatives, bool emitSignal);


private:
    const QVector<DataInformation*>& currentChildren() const;
    int determineSelection(TopLevelDataInformation* top);

private:
    QVector<FieldInfo> mAlternatives;
    QVector<DataInformation*> mDefaultFields; //always sorted
    //TODO
//    /** If mUnion is non-null the children are displayed inside a union and the correct
//     * child is highlighted. Otherwise only the correct fields will be appended after mChildren
//     */
//    QScopedPointer<UnionDataInformation> mUnion;
    int mLastIndex;
};

inline bool TaggedUnionDataInformation::isTaggedUnion() const
{
    return true;
}

inline const QVector<DataInformation*>& TaggedUnionDataInformation::currentChildren() const
{
    Q_ASSERT(mLastIndex < mAlternatives.size());
    return mLastIndex >= 0 ? mAlternatives.at(mLastIndex).fields : mDefaultFields;
}

#endif /* TAGGEDUNIONDATAINFORMATION_H_ */
