/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FIXEDSIZEBYTEARRAYMODEL_HPP
#define OKTETA_FIXEDSIZEBYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"

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
    FixedSizeByteArrayModel(Byte* data, int size, Byte fillUpChar = '\0', QObject* parent = nullptr);
    /** creates a writeable buffer which is deleted at the end */
    explicit FixedSizeByteArrayModel(int size, Byte fillUpChar = '\0', QObject* parent = nullptr);

    ~FixedSizeByteArrayModel() override;

public: // AbstractByteArrayModel API
    Byte byte(Address offset) const override;
    Size size() const override;
    bool isReadOnly() const override;
    bool isModified() const override;

    Size insert(Address offset, const Byte* insertData, int insertLength) override;
    Size remove(const AddressRange& removeRange) override;
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;

    void setModified(bool modified = true) override;
    void setReadOnly(bool readOnly = true) override;

public:
    int compare(const AbstractByteArrayModel& other, const AddressRange& otherRange, Address offset = 0);
    int compare(const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset = 0);
    int compare(const AbstractByteArrayModel& other);

public:
    Byte* rawData() const;

private:
    void reset(unsigned int pos, unsigned int length);

private:
    /** */
    Byte* mData;
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

inline int FixedSizeByteArrayModel::compare(const AbstractByteArrayModel& other)
{ return compare(other, AddressRange::fromWidth(0, other.size()), 0); }

inline int FixedSizeByteArrayModel::compare(const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset)
{ return compare(other, AddressRange::fromWidth(otherOffset, otherLength), offset); }

inline Byte* FixedSizeByteArrayModel::rawData() const { return mData; }

}

#endif
