/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewprofile.hpp"

// Okteta Gui
#include <Okteta/ByteArrayColumnView>
// Qt
#include <QString>

namespace Kasten {

class ByteArrayViewProfilePrivate : public QSharedData
{
public:
    ByteArrayViewProfilePrivate();

public:
    QString mId;

    QString mViewProfileTitle;
    int mOffsetCoding;
    QString mCharCodingName;
    int mValueCoding;
    bool mOffsetColumnVisible = true;
    int mVisibleByteArrayCodings;
    int mNoOfBytesPerLine;
    int mLayoutStyle;
    QChar mSubstituteChar;
    QChar mUndefinedChar;
    bool mShowsNonprinting;
    int mNoOfGroupedBytes;
    int mViewModus = 0;
};

QString DefaultViewProfileCharCodingName() { return QStringLiteral("ISO-8859-1"); }
static constexpr bool DefaultViewProfileShowingNonprinting = false;
static constexpr QChar DefaultViewProfileSubstituteChar =  QLatin1Char('.');
static constexpr QChar DefaultViewProfileUndefinedChar =   QChar(QChar::ReplacementCharacter);
static constexpr int DefaultViewProfileNoOfGroupedBytes = 4;
static constexpr int DefaultViewProfileNoOfBytesPerLine =  16;
static constexpr Okteta::AbstractByteArrayView::OffsetCoding DefaultViewProfileOffsetCoding =
    Okteta::AbstractByteArrayView::HexadecimalOffset;
static constexpr Okteta::AbstractByteArrayView::ValueCoding DefaultViewProfileValueCoding =
    Okteta::AbstractByteArrayView::HexadecimalCoding;
static constexpr Okteta::AbstractByteArrayView::LayoutStyle DefaultViewProfileResizeStyle =
    Okteta::AbstractByteArrayView::FixedLayoutStyle;
static constexpr Okteta::AbstractByteArrayView::CodingTypes DefaultViewProfileVisibleByteArrayCodings =
    Okteta::AbstractByteArrayView::ValueAndCharCodings;

ByteArrayViewProfilePrivate::ByteArrayViewProfilePrivate()
    : mOffsetCoding(DefaultViewProfileOffsetCoding)
    , mCharCodingName(DefaultViewProfileCharCodingName())
    , mValueCoding(DefaultViewProfileValueCoding)
    , mVisibleByteArrayCodings(DefaultViewProfileVisibleByteArrayCodings)
    , mNoOfBytesPerLine(DefaultViewProfileNoOfBytesPerLine)
    , mLayoutStyle(DefaultViewProfileResizeStyle)
    , mSubstituteChar(DefaultViewProfileSubstituteChar)
    , mUndefinedChar(DefaultViewProfileUndefinedChar)
    , mShowsNonprinting(DefaultViewProfileShowingNonprinting)
    , mNoOfGroupedBytes(DefaultViewProfileNoOfGroupedBytes)
{
}

ByteArrayViewProfile::ByteArrayViewProfile()
    : d(new ByteArrayViewProfilePrivate)
{
}
ByteArrayViewProfile::ByteArrayViewProfile(const ByteArrayViewProfile& other) = default;

ByteArrayViewProfile::~ByteArrayViewProfile() = default;

ByteArrayViewProfile& ByteArrayViewProfile::operator=(const ByteArrayViewProfile& other) = default;

ByteArrayViewProfile::Id ByteArrayViewProfile::id() const { return d->mId; }
QString ByteArrayViewProfile::viewProfileTitle()    const { return d->mViewProfileTitle; }
int ByteArrayViewProfile::offsetCoding()            const { return d->mOffsetCoding; }
QString ByteArrayViewProfile::charCodingName()      const { return d->mCharCodingName; }
int ByteArrayViewProfile::valueCoding()             const { return d->mValueCoding; }
bool ByteArrayViewProfile::offsetColumnVisible()    const { return d->mOffsetColumnVisible; }
int ByteArrayViewProfile::visibleByteArrayCodings() const { return d->mVisibleByteArrayCodings; }
int ByteArrayViewProfile::noOfBytesPerLine()        const { return d->mNoOfBytesPerLine; }
int ByteArrayViewProfile::layoutStyle()             const { return d->mLayoutStyle; }
QChar ByteArrayViewProfile::substituteChar()        const { return d->mSubstituteChar; }
QChar ByteArrayViewProfile::undefinedChar()         const { return d->mUndefinedChar; }
bool ByteArrayViewProfile::showsNonprinting()       const { return d->mShowsNonprinting; }
int ByteArrayViewProfile::noOfGroupedBytes()        const { return d->mNoOfGroupedBytes; }
int ByteArrayViewProfile::viewModus()               const { return d->mViewModus; }

void ByteArrayViewProfile::setId(const Id& id) { d->mId = id; }
void ByteArrayViewProfile::setViewProfileTitle(const QString& title) { d->mViewProfileTitle = title; }
void ByteArrayViewProfile::setOffsetCoding(int offsetCoding) { d->mOffsetCoding = offsetCoding; }
void ByteArrayViewProfile::setValueCoding(int valueCoding) { d->mValueCoding = valueCoding; }
void ByteArrayViewProfile::setCharCoding(const QString& charCodingName) { d->mCharCodingName = charCodingName; }
void ByteArrayViewProfile::setSubstituteChar(const QChar& substituteChar) { d->mSubstituteChar = substituteChar; }
void ByteArrayViewProfile::setUndefinedChar(const QChar& undefinedChar) { d->mUndefinedChar = undefinedChar; }
void ByteArrayViewProfile::setOffsetColumnVisible(bool visible) { d->mOffsetColumnVisible = visible; }
void ByteArrayViewProfile::setVisibleByteArrayCodings(int columns) { d->mVisibleByteArrayCodings = columns; }
void ByteArrayViewProfile::setLayoutStyle(int layoutStyle) { d->mLayoutStyle = layoutStyle; }
void ByteArrayViewProfile::setNoOfBytesPerLine(int noOfBytesPerLine) { d->mNoOfBytesPerLine = noOfBytesPerLine; }
void ByteArrayViewProfile::setShowsNonprinting(bool showsNonprinting) { d->mShowsNonprinting = showsNonprinting; }
void ByteArrayViewProfile::setNoOfGroupedBytes(int noOfGroupedBytes) { d->mNoOfGroupedBytes = noOfGroupedBytes; }
void ByteArrayViewProfile::setViewModus(int viewModus) { d->mViewModus = viewModus; }

}
