/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWPROFILE_HPP
#define KASTEN_BYTEARRAYVIEWPROFILE_HPP

// lib
#include "oktetakastengui_export.hpp"
// Qt
#include <QSharedDataPointer>

class QChar;
class QString;

namespace Kasten {
class ByteArrayViewProfilePrivate;

class OKTETAKASTENGUI_EXPORT ByteArrayViewProfile
{
public:
    using Id = QString;

public:
    explicit ByteArrayViewProfile();
    ByteArrayViewProfile(const ByteArrayViewProfile& other);

    ~ByteArrayViewProfile();

public:
    ByteArrayViewProfile& operator=(const ByteArrayViewProfile& other);

public:
    void setId(const Id& id);
    Id id() const;

public: // AbstractModel API
    void setViewProfileTitle(const QString& title);
    QString viewProfileTitle() const;

public: // codings
    void setOffsetCoding(int offsetCoding);
    int offsetCoding() const;
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    int valueCoding() const;
    QString charCodingName() const;
    void setShowsNonprinting(bool showsNonprinting);
    bool showsNonprinting() const;
    void setSubstituteChar(const QChar& substituteChar);
    void setUndefinedChar(const QChar& undefinedChar);
    QChar substituteChar() const;
    QChar undefinedChar() const;

public: // elements
    void setOffsetColumnVisible(bool visible);
    void setVisibleByteArrayCodings(int columns);
    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    int noOfBytesPerLine() const;
    int layoutStyle() const;
    int noOfGroupedBytes() const;

public:
    void setViewModus(int viewModus);
    int viewModus() const;

private:
    QSharedDataPointer<ByteArrayViewProfilePrivate> d;
};

}

Q_DECLARE_TYPEINFO(Kasten::ByteArrayViewProfile, Q_MOVABLE_TYPE);

#endif
