/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FIXEDSIZEBYTEARRAYMODEL_HPP
#define OKTETA_FIXEDSIZEBYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"
// Std
#include <memory>

namespace Okteta {

/** base class for all mData buffers that are used to display
 * TODO: think about a way to inform KHexEdit that there has been
 * a change in the buffer outside. what kind of changes are possible?
 * @author Friedrich W. H. Kossebau
*/

class FixedSizeByteArrayModel : public AbstractByteArrayModel
{
    Q_OBJECT

public:
    /** creates a readonly buffer around the given data */
    FixedSizeByteArrayModel(Byte* data, int size, Byte fillUpByte = '\0', QObject* parent = nullptr);
    /** creates a writeable buffer which is deleted at the end */
    explicit FixedSizeByteArrayModel(int size, Byte fillUpByte = '\0', QObject* parent = nullptr);

    ~FixedSizeByteArrayModel() override;

public: // AbstractByteArrayModel API
    [[nodiscard]]
    Byte byte(Address offset) const override;
    [[nodiscard]]
    Size size() const override;
    [[nodiscard]]
    bool isReadOnly() const override;
    [[nodiscard]]
    bool isModified() const override;

    [[nodiscard]]
    Size insertBytes(Address offset, const Byte* insertData, int insertLength) override;
    [[nodiscard]]
    Size removeBytes(const AddressRange& removeRange) override;
    [[nodiscard]]
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    [[nodiscard]]
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    [[nodiscard]]
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;

    void setModified(bool modified = true) override;
    void setReadOnly(bool readOnly = true) override;

public:
    [[nodiscard]]
    int compare(const AbstractByteArrayModel& other, const AddressRange& otherRange, Address offset = 0) const;
    [[nodiscard]]
    int compare(const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset = 0) const;
    [[nodiscard]]
    int compare(const AbstractByteArrayModel& other) const;

public:
    [[nodiscard]]
    Byte* rawData() const;

private:
    void reset(unsigned int offset, unsigned int length);

private:
    /** */
    std::unique_ptr<Byte[]> mData;
    /***/
    int mSize;
    /** */
    Byte mFillUpByte;
    /**  */
    bool mReadOnly : 1;
    /** */
    bool mModified : 1;
    /** */
    bool mAutoDelete : 1;
};

inline Byte FixedSizeByteArrayModel::byte(Address offset) const { return mData[offset]; }
inline Size FixedSizeByteArrayModel::size() const { return mSize; }

inline bool FixedSizeByteArrayModel::isReadOnly()   const { return mReadOnly; }
inline bool FixedSizeByteArrayModel::isModified()   const { return mModified; }

inline void FixedSizeByteArrayModel::setReadOnly(bool readOnly)  { mReadOnly = readOnly; }
inline void FixedSizeByteArrayModel::setModified(bool modified)   { mModified = modified; }

inline int FixedSizeByteArrayModel::compare(const AbstractByteArrayModel& other) const
{ return compare(other, AddressRange::fromWidth(0, other.size()), 0); }

inline int FixedSizeByteArrayModel::compare(const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset) const
{ return compare(other, AddressRange::fromWidth(otherOffset, otherLength), offset); }

inline Byte* FixedSizeByteArrayModel::rawData() const { return mData.get(); }

}

#endif
