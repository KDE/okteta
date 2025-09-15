/*
    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowview.hpp"
#include "bytearrayrowview_p.hpp"

namespace Okteta {

ByteArrayRowView::ByteArrayRowView(QWidget* parent)
    : AbstractByteArrayView(std::make_unique<ByteArrayRowViewPrivate>(this), parent)
{
    Q_D(ByteArrayRowView);
    d->init();
}

ByteArrayRowView::~ByteArrayRowView() = default;

PixelX ByteArrayRowView::byteSpacingWidth() const
{
    Q_D(const ByteArrayRowView);
    return d->byteSpacingWidth();
}
int ByteArrayRowView::noOfGroupedBytes() const
{
    Q_D(const ByteArrayRowView);
    return d->noOfGroupedBytes();
}
PixelX ByteArrayRowView::groupSpacingWidth() const
{
    Q_D(const ByteArrayRowView);
    return d->groupSpacingWidth();
}
PixelX ByteArrayRowView::binaryGapWidth() const
{
    Q_D(const ByteArrayRowView);
    return d->binaryGapWidth();
}
bool ByteArrayRowView::showsNonprinting() const
{
    Q_D(const ByteArrayRowView);
    return d->showsNonprinting();
}
QChar ByteArrayRowView::substituteChar() const
{
    Q_D(const ByteArrayRowView);
    return d->substituteChar();
}
QChar ByteArrayRowView::undefinedChar() const
{
    Q_D(const ByteArrayRowView);
    return d->undefinedChar();
}
bool ByteArrayRowView::isByteTypeColored() const
{
    Q_D(const ByteArrayRowView);
    return d->isByteTypeColored();
}
void ByteArrayRowView::setBufferSpacing(/*PixelX*/ int byteSpacing, int noOfGroupedBytes, /*PixelX*/ int groupSpacing)
{
    Q_D(ByteArrayRowView);
    d->setBufferSpacing(byteSpacing, noOfGroupedBytes, groupSpacing);
}

void ByteArrayRowView::setValueCoding(ValueCoding valueCoding)
{
    Q_D(ByteArrayRowView);
    d->setValueCoding(valueCoding);
}

void ByteArrayRowView::setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth)
{
    Q_D(ByteArrayRowView);
    d->setByteSpacingWidth(byteSpacingWidth);
}

void ByteArrayRowView::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    Q_D(ByteArrayRowView);
    d->setNoOfGroupedBytes(noOfGroupedBytes);
}

void ByteArrayRowView::setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth)
{
    Q_D(ByteArrayRowView);
    d->setGroupSpacingWidth(groupSpacingWidth);
}

void ByteArrayRowView::setBinaryGapWidth(int /*PixelX*/ binaryGapWidth)
{
    Q_D(ByteArrayRowView);
    d->setBinaryGapWidth(binaryGapWidth);
}

void ByteArrayRowView::setSubstituteChar(QChar substituteChar)
{
    Q_D(ByteArrayRowView);
    d->setSubstituteChar(substituteChar);
}

void ByteArrayRowView::setUndefinedChar(QChar undefinedChar)
{
    Q_D(ByteArrayRowView);
    d->setUndefinedChar(undefinedChar);
}

void ByteArrayRowView::setShowsNonprinting(bool showsNonprinting)
{
    Q_D(ByteArrayRowView);
    d->setShowsNonprinting(showsNonprinting);
}

void ByteArrayRowView::setCharCoding(const QString& charCodingName)
{
    Q_D(ByteArrayRowView);
    d->setCharCoding(charCodingName);
}

void ByteArrayRowView::setByteTypeColored(bool isColored)
{
    Q_D(ByteArrayRowView);
    d->setByteTypeColored(isColored);
}

void ByteArrayRowView::changeEvent(QEvent* event)
{
    Q_D(ByteArrayRowView);

    d->changeEvent(event);
}

QSize ByteArrayRowView::minimumSizeHint() const
{
    Q_D(const ByteArrayRowView);
    // TODO: better minimal width (visibility!)
    return d->minimumSizeHint();
}

}

#include "moc_bytearrayrowview.cpp"
