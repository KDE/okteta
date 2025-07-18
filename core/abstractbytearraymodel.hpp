/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYMODEL_HPP
#define OKTETA_ABSTRACTBYTEARRAYMODEL_HPP

// lib
#include "oktetacore_export.hpp"
#include "addressrange.hpp"
#include "size.hpp"
#include "byte.hpp"
// Qt
#include <QObject>
#include <QByteArray>
// Std
#include <memory>

namespace Okteta {

class ArrayChangeMetricsList;
class CharCodec;
class AbstractByteArrayModelPrivate;

/** could it be useful to hide the data access behind an iterator? *
class KDataBufferIterator
{
public:
    bool hasNext();
    char next();
protected:

protected:
    char *
    int Length;
}

bool KDataBufferIterator::hasNext()
{
}
// this function should be simple as possible
char KDataBufferIterator::next()
{
   // if next span is empty
   if(  )
        return *NextChar++;
}
 */

/** base class for all Data buffers that are used to display
 * TODO: think about a way to inform KHexEdit that there has been
 * a change in the buffer outside. what kind of changes are possible?
 *
 * Operations on the data:
 *
 * Finding: is implemented stateless. FindNext has to be done by perhaps a FindManager
 * Replacing: not available. Implement within a ReplaceManager
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETACORE_EXPORT AbstractByteArrayModel : public QObject
{
    friend class KAbstractByteArrayModelIterator;

    Q_OBJECT

protected:
    OKTETACORE_NO_EXPORT explicit AbstractByteArrayModel(std::unique_ptr<AbstractByteArrayModelPrivate>&& dd, QObject* parent = nullptr);
    explicit AbstractByteArrayModel(QObject* parent = nullptr);

public:
    ~AbstractByteArrayModel() override;

public: // data access API
    /** locates working range
     * The idea behind is to tell buffer which range will be requested in the following time,
     * so that e.g. the corresponding pages will be loaded in advance
     * TODO: do we really need this? Where will this lead to real enhancements?
     * @param Range
     * @return @c true if successfully, @c false otherwise
     */
    // virtual bool prepareRange( const Section &Range ) const = 0;
    /** convenience function, same as above */
    // bool prepareRange( int Offset, int Length ) const;

    /** creates an iterator to */
    // virtual KDataBufferIterator *iterator() const = 0;
    /** expects pointer to memory, should be in prepared range
     * it is only expected to be a valid pointer until any further call
     * to this or any modifying function
     * @param Section
     * @return pointer to
     */
    // virtual const char *dataSet( const Section &Section ) const = 0;
    /** convenience function, same as above */
    // const char *dataSet( int Offset, int Length ) const;

    /** requests a single byte
     * if the offset is not valid the behaviour is undefined
     * @param offset offset of the datum requested
     */
    [[nodiscard]]
    virtual Byte byte(Address offset) const = 0;

    /**
     * @return the size of the data
     */
    [[nodiscard]]
    virtual Size size() const = 0;

public: // state read API
    /**
     * Default returns @c true.
     * @return @c true if the buffer can only be read, @c false if writing is allowed
     */
    [[nodiscard]]
    virtual bool isReadOnly() const;
    /**
     * @return @c true if the buffer has been modified, @c false otherwise
     */
    [[nodiscard]]
    virtual bool isModified() const = 0;

// TODO: for data outside the model using char* and int as well as QByteArray should always work, no?

public: // modification API
    /** inserts bytes copied from the given source at Position.
     * The original data beginnung at the position is moved
     * with the buffer enlarged as needed
     * If the buffer is readOnly this is a noop and returns 0.
     * @param offset
     * @param insertData data source
     * @param insertLength number of bytes to copy
     * @return length of inserted data
     */
    [[nodiscard]]
    virtual Size insertBytes(Address offset, const Byte* insertData, int insertLength);
    [[nodiscard]]
    Size insertBytes(Address offset, const QByteArray& insertData);

    /** removes beginning with position as much as possible
     * @param removeRange
     * @return length of removed data
     */
    [[nodiscard]]
    virtual Size removeBytes(const AddressRange& removeRange);
    /** convenience function, behaves as above */
    [[nodiscard]]
    Size removeBytes(Address offset, Size removeLength);

    /** replaces as much as possible
     * @param removeRange
     * @param insertData
     * @param insertLength
     * @return length of inserted data
     */
    [[nodiscard]]
    virtual Size replaceBytes(const AddressRange& removeRange, const Byte* insertData, int insertLength) = 0;
    /** convenience function, behaves as above */
    [[nodiscard]]
    Size replaceBytes(const AddressRange& removeRange, const QByteArray& insertData);
    /** convenience function, behaves as above */
    [[nodiscard]]
    Size replaceBytes(Address offset, Size removeLength,
                      const Byte* insertData, Size insertLength);

    // todo use parameters grouped differrently?
    /** moves the second section before the start of the first
     * which is the same as moving the first behind the second.
     * @param firstStart position of the data where the section should be moved behind
     * @param secondRange data range to be moved
     * @return @p true if operation was successful, @p false otherwise
     */
    [[nodiscard]]
    virtual bool swapBytes(Address firstStart, const AddressRange& secondRange) = 0;

    /**
     * fills the buffer with the FillChar. If the buffer is to small it will be extended as much as possible.
     * @param fillByte byte to use
     * @param offset position where the filling starts
     * @param fillLength number of bytes to fill. If Length is -1, the buffer is filled till the end.
     * @return number of filled bytes
     */
    [[nodiscard]]
    virtual Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) = 0;
    [[nodiscard]]
    Size fill(Byte fillChar, const AddressRange& fillRange);

    /** sets a single byte
     * if the offset is not valid the behaviour is undefined
     * @param offset offset of the datum requested
     * @param byte new byte value
     */
    virtual void setByte(Address offset, Byte byte) = 0;

    /** sets the modified flag for the buffer
     * @param modified
     */
    virtual void setModified(bool modified) = 0;

    /** sets the readonly flag for the byte array if this is possible.
     * Default implementation does not do anything.
     * @param isReadOnly new state
     */
    virtual void setReadOnly(bool isReadOnly);

public: // service functions
    /** copies the data of the section into a given array Dest. If the section extends the buffers range
     * the section is limited to the buffer's end. If the section is invalid the behaviour is undefined.
     * @param dest pointer to a char array large enough to hold the copied section
     * @param copyRange
     * @return number of copied bytes
     */
    [[nodiscard]]
    virtual Size copyTo(Byte* dest, const AddressRange& copyRange) const;
    /** convenience function, behaves as above */
    [[nodiscard]]
    Size copyTo(Byte* dest, Address offset, Size copyLength) const;

public: // finding API
    /** searches beginning with byte at Pos.
     * @param pattern
     * @param patternLength length of search string
     * @param fromOffset the position to start the search
     * @param toOffset the position to end the search (-1 is to the end of the data stream)
     * @return index of the first or -1
     */
    [[nodiscard]]
    virtual Address indexOf(const Byte* pattern, int patternLength, Address fromOffset = 0, Address toOffset = -1) const;
    [[nodiscard]]
    Address indexOf(const QByteArray& pattern, Address fromOffset = 0, Address toOffset = -1) const;
    [[nodiscard]]
    Address indexOfCaseInsensitive(const CharCodec* charCodec, const QByteArray& pattern, Address fromOffset = 0, Address toOffset = -1) const;

    /** searches for a given data string
     * The section limits the data within which the key has to be found
     * If the end of the section is lower than the start the search continues at the start???
     * @param
     * @param Length length of search string
     * @param Section section within the keydata is to be found
     * @return index of the first occurrence or -1
     */
//     virtual int indexOf( const char*KeyData, int Length, const Section &Section ) const { return -1; }//= 0;
    /** searches backward beginning with byte at Pos.
     * @param pattern
     * @param patternLength length of search string
     * @param fromOffset the position to start the search. If -1 the search starts at the end.
     * @return index of the first  or -1
     */
    [[nodiscard]]
    virtual Address lastIndexOf(const Byte* pattern, int patternLength, Address fromOffset = -1, Address toOffset = 0) const;
    [[nodiscard]]
    Address lastIndexOf(const QByteArray& pattern, Address fromOffset = -1, Address toOffset = 0) const;
    [[nodiscard]]
    Address lastIndexOfCaseInsensitive(const CharCodec* charCodec, const QByteArray& pattern, Address fromOffset = -1, Address toOffset = 0) const;

/*     virtual int find( const QString &expr, bool cs, bool wo, bool forward = true, int *index = 0 ); */

Q_SIGNALS:
    // TODO: how to deal replacing with fixed size of buffer?
    void contentsChanged(const Okteta::ArrayChangeMetricsList& changeList);

    void readOnlyChanged(bool isReadOnly);
    void modifiedChanged(bool isModified);

    // TODO: how to handle a typedef with signals
    void searchedBytes(Okteta::Size bytes) const;

protected:
    const std::unique_ptr<AbstractByteArrayModelPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractByteArrayModel)
};

// TODO: find why static_cast fails
inline Size AbstractByteArrayModel::insertBytes(Address offset, const QByteArray& insertData)
{ return insertBytes(offset, reinterpret_cast<const Byte*>(insertData.constData()), insertData.size()); }

inline Size AbstractByteArrayModel::removeBytes(Address offset, Size removeLength)
{ return removeBytes(AddressRange::fromWidth(offset, removeLength)); }

inline Size AbstractByteArrayModel::replaceBytes(const AddressRange& removeRange, const QByteArray& insertData)
{ return replaceBytes(removeRange, reinterpret_cast<const Byte*>(insertData.constData()), insertData.size());}

inline Size AbstractByteArrayModel::replaceBytes(Address offset, Size removeLength,
                                                 const Byte* insertData, Size insertLength)
{ return replaceBytes(AddressRange::fromWidth(offset, removeLength), insertData, insertLength); }

inline Size AbstractByteArrayModel::fill(const Byte fillChar, const AddressRange& fillRange)
{ return fill(fillChar, fillRange.start(), fillRange.width()); }

inline Size AbstractByteArrayModel::copyTo(Byte* dest, Address offset, Size copyLength) const
{ return copyTo(dest, AddressRange::fromWidth(offset, copyLength)); }

inline Address AbstractByteArrayModel::indexOf(const QByteArray& pattern, Address fromOffset, Address toOffset) const
{ return indexOf(reinterpret_cast<const Byte*>(pattern.constData()), pattern.size(), fromOffset, toOffset); }

inline Address AbstractByteArrayModel::lastIndexOf(const QByteArray& pattern, Address fromOffset, Address toOffset) const
{ return lastIndexOf(reinterpret_cast<const Byte*>(pattern.constData()), pattern.size(), fromOffset, toOffset); }

}

#endif
