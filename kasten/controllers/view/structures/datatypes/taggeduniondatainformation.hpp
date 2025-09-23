/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TAGGEDUNIONDATAINFORMATION_HPP
#define KASTEN_TAGGEDUNIONDATAINFORMATION_HPP

// lib
#include "datainformationwithchildren.hpp"
#include <allprimitivetypes.hpp>
// Qt
#include <QScriptValue>
// Std
#include <memory>
#include <vector>

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
        inline explicit FieldInfo(const QString& n, const QScriptValue& s,
                                  std::vector<std::unique_ptr<DataInformation>>&& f)
            : name(n)
            , selectIf(s)
            , fields(std::move(f))
        {}
        FieldInfo() = default;
        FieldInfo(const FieldInfo&) = delete;
        FieldInfo(FieldInfo&&) = default;

        /** The name this tagged union gets when this is selected */
        QString name;
        QScriptValue selectIf;
        std::vector<std::unique_ptr<DataInformation>> fields;
    };

public:
    explicit TaggedUnionDataInformation(const QString& name, DataInformation* parent = nullptr);
    ~TaggedUnionDataInformation() override;

public: // DataInformationWithChildren API
    [[nodiscard]]
    bool replaceChildAt(unsigned int index, std::unique_ptr<DataInformation>&& newChild) override;

public: // DataInformation API
    [[nodiscard]]
    unsigned int childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(unsigned int index) const override;
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input,
                    Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset) override;

    [[nodiscard]]
    int indexOf(const DataInformation* const data) const override;

public: // DataInformation API
    [[nodiscard]]
    bool isTaggedUnion() const override;

public:
    void appendDefaultField(std::unique_ptr<DataInformation>&& field, bool emitSignal);
    void setAlternatives(std::vector<FieldInfo>&& alternatives, bool emitSignal);

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;

private:
    [[nodiscard]]
    const std::vector<std::unique_ptr<DataInformation>>& currentChildren() const;
    [[nodiscard]]
    int determineSelection(TopLevelDataInformation* top);

private:
    std::vector<FieldInfo> mAlternatives;
    std::vector<std::unique_ptr<DataInformation>> mDefaultFields; // always sorted
    // TODO
//    /** If mUnion is non-null the children are displayed inside a union and the correct
//     * child is highlighted. Otherwise only the correct fields will be appended after mChildren
//     */
//    std::unique_ptr<UnionDataInformation> mUnion;
    int mLastIndex = -1;
};

inline const std::vector<std::unique_ptr<DataInformation>>& TaggedUnionDataInformation::currentChildren() const
{
    Q_ASSERT(mLastIndex < static_cast<int>(mAlternatives.size()));
    return mLastIndex >= 0 ? mAlternatives.at(mLastIndex).fields : mDefaultFields;
}

#endif /* KASTEN_TAGGEDUNIONDATAINFORMATION_HPP */
