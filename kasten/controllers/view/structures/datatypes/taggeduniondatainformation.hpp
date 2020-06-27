/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TAGGEDUNIONDATAINFORMATION_HPP
#define KASTEN_TAGGEDUNIONDATAINFORMATION_HPP

#include "datainformationwithchildren.hpp"
#include "../allprimitivetypes.hpp"

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
            : name(n)
            , selectIf(s)
            , fields(f)
        {}
        FieldInfo() = default;
        /** The name this tagged union gets when this is selected */
        QString name;
        QScriptValue selectIf;
        QVector<DataInformation*> fields;
    };

    explicit TaggedUnionDataInformation(const QString& name, DataInformation* parent = nullptr);
    ~TaggedUnionDataInformation() override;

    qint64 readData(Okteta::AbstractByteArrayModel* input,
                    Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset) override;
    bool isTaggedUnion() const override;

    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    BitCount32 size() const override;

    bool replaceChildAt(unsigned int index, DataInformation* newChild) override;
    int indexOf(const DataInformation* const data) const override;

    DataInformation* childAt(unsigned int index) const override;
    unsigned int childCount() const override;
    void appendDefaultField(DataInformation* field, bool emitSignal);
    void setAlternatives(const QVector<FieldInfo>& alternatives, bool emitSignal);

private:
    QString typeNameImpl() const override;

private:
    const QVector<DataInformation*>& currentChildren() const;
    int determineSelection(TopLevelDataInformation* top);

private:
    QVector<FieldInfo> mAlternatives;
    QVector<DataInformation*> mDefaultFields; // always sorted
    // TODO
//    /** If mUnion is non-null the children are displayed inside a union and the correct
//     * child is highlighted. Otherwise only the correct fields will be appended after mChildren
//     */
//    QScopedPointer<UnionDataInformation> mUnion;
    int mLastIndex = -1;
};

inline const QVector<DataInformation*>& TaggedUnionDataInformation::currentChildren() const
{
    Q_ASSERT(mLastIndex < mAlternatives.size());
    return mLastIndex >= 0 ? mAlternatives.at(mLastIndex).fields : mDefaultFields;
}

#endif /* KASTEN_TAGGEDUNIONDATAINFORMATION_HPP */
