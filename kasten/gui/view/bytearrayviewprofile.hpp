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
    [[nodiscard]]
    Id id() const;

public: // AbstractModel API
    void setViewProfileTitle(const QString& title);
    [[nodiscard]]
    QString viewProfileTitle() const;

public: // codings
    void setOffsetCoding(int offsetCoding);
    [[nodiscard]]
    int offsetCoding() const;
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    [[nodiscard]]
    int valueCoding() const;
    [[nodiscard]]
    QString charCodingName() const;
    void setShowsNonprinting(bool showsNonprinting);
    [[nodiscard]]
    bool showsNonprinting() const;
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);
    [[nodiscard]]
    QChar substituteChar() const;
    [[nodiscard]]
    QChar undefinedChar() const;

public: // elements
    void setOffsetColumnVisible(bool visible);
    void setVisibleByteArrayCodings(int columns);
    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    int visibleByteArrayCodings() const;

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    [[nodiscard]]
    int noOfBytesPerLine() const;
    [[nodiscard]]
    int layoutStyle() const;
    [[nodiscard]]
    int noOfGroupedBytes() const;

public:
    void setViewModus(int viewModus);
    [[nodiscard]]
    int viewModus() const;

private:
    QSharedDataPointer<ByteArrayViewProfilePrivate> d;
};

}

Q_DECLARE_TYPEINFO(Kasten::ByteArrayViewProfile, Q_MOVABLE_TYPE);

#endif
