/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHANGESDATASTORAGE_HPP
#define OKTETA_CHANGESDATASTORAGE_HPP

// Qt
#include <QByteArray>

namespace Okteta {

class ChangesDataStorage
{
public:
    ChangesDataStorage();

public: // set/action
    void append(int from, char byte);
    void append(int from, const char* data, int dataLength);
    void appendFill(int from, char fillDatum, int fillLength);

    void clear();

public: // get
    QByteArray data(int offset, int length) const;
    char operator[](int offset) const;

private:
    QByteArray mData;
};

inline ChangesDataStorage::ChangesDataStorage() = default;

inline QByteArray ChangesDataStorage::data(int offset, int length) const
{
    return mData.mid(offset, length);
}

inline char ChangesDataStorage::operator[](int offset) const { return mData[offset]; }

inline void ChangesDataStorage::clear() { mData.clear(); }

inline void ChangesDataStorage::append(int from, char datum)
{
    mData.resize(from);
    mData.append(datum);
}

inline void ChangesDataStorage::append(int from, const char* data, int dataLength)
{
    mData.resize(from + dataLength);
    memcpy(mData.data() + from, data, dataLength);
}

inline void ChangesDataStorage::appendFill(int from, char fillDatum, int fillLength)
{
    mData.resize(from + fillLength);
    memset(mData.data() + from, fillDatum, fillLength);
}

}

#endif
