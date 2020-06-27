/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYROWVIEW_P_HPP
#define OKTETA_BYTEARRAYROWVIEW_P_HPP

// lib
#include "bytearrayrowview.hpp"
#include "abstractbytearrayview_p.hpp"
#include "bytearrayrowcolumnrenderer.hpp"

namespace Okteta {
class Coord;

class ByteArrayRowViewPrivate : public AbstractByteArrayViewPrivate
{
    Q_DECLARE_PUBLIC(ByteArrayRowView)

public:
    explicit ByteArrayRowViewPrivate(ByteArrayRowView* parent);
    ~ByteArrayRowViewPrivate() override;

public:
    void init();

public:
    // value column
    int /*PixelX*/ byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int /*PixelX*/ groupSpacingWidth() const;
    int /*PixelX*/ binaryGapWidth() const;

    // char column
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;

    bool isByteTypeColored() const;

public: // logic value service
    QSize minimumSizeHint() const;

public: // modification access
    void setByteArrayModel(AbstractByteArrayModel* byteArrayModel);

public: // AbstractByteArrayViewPrivate API
    void ensureVisible(const AddressRange& range, bool ensureStartVisible) override;
    void ensureCursorVisible() override;
    void placeCursor(QPoint point) override;
    QRect cursorRect() const override;
    Address indexByPoint(QPoint point) const override;
    void blinkCursor() override;

public:
    // setting parameters
    // value column parameters
    void setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    void setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth);
    void setBinaryGapWidth(int binaryGapWidth);
    void setBufferSpacing(/*PixelX*/ int byteSpacingWidth, int noOfGroupedBytes, /*PixelX*/ int groupSpacingWidth);
    void setValueCoding(AbstractByteArrayView::ValueCoding valueCoding);
    // char column parameters
    void setShowsNonprinting(bool showsNonprinting = true);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);
    void setCharCoding(AbstractByteArrayView::CharCoding charCoding);
    void setCharCoding(const QString& charCodingName);
    void setByteTypeColored(bool isColored);

private: // drawing related operations
    /** recreates the cursor pixmaps and paints active and inactive cursors if doable */
    void createCursorPixmaps();
    /** draws the blinking cursor or removes it */
    void drawActiveCursor(QPainter* painter);
    void drawInactiveCursor(QPainter* painter);
    void updateCursor(const ByteArrayRowColumnRenderer& column, AbstractByteArrayView::CodingTypeId codingId);

private: // partial operations
    void changeEvent(QEvent* event);

private:
    /** recalcs a layout due to the resize style that fits into the view size
     * and updates the dependent values
     */
    void ensureVisible(const ByteArrayRowColumnRenderer& column, Coord coord);
    void renderColumns(QPainter* painter, int cx, int cy, int cw, int ch);

protected: // AbstractByteArrayViewPrivate API
    // cursor control
    void pauseCursor() override;
    void updateCursors() override;

    void setActiveCoding(AbstractByteArrayView::CodingTypeId codingId) override;
    void setVisibleCodings(int visibleCodings) override;

protected: // AbstractByteArrayViewPrivate API
    AbstractByteArrayView::CodingTypeId activeCoding() const override;
    AbstractByteArrayView::CodingTypes visibleCodings() const override;
    int fittingBytesPerLine() const override;
    void adjustToLayoutNoOfBytesPerLine() override;
    void updateChanged() override;

private:
    ByteArrayRowColumnRenderer*   mByteArrayColumn;

    ByteArrayRowView::CodingTypeId mActiveCoding;
    ByteArrayRowView::CodingTypeId mInactiveCoding;
};

inline PixelX ByteArrayRowViewPrivate::byteSpacingWidth()  const { return mByteArrayColumn->byteSpacingWidth(); }
inline int ByteArrayRowViewPrivate::noOfGroupedBytes()      const { return mByteArrayColumn->noOfGroupedBytes(); }
inline PixelX ByteArrayRowViewPrivate::groupSpacingWidth() const { return mByteArrayColumn->groupSpacingWidth(); }
inline PixelX ByteArrayRowViewPrivate::binaryGapWidth()    const { return mByteArrayColumn->binaryGapWidth(); }
inline bool ByteArrayRowViewPrivate::showsNonprinting()     const { return mByteArrayColumn->isShowingNonprinting(); }
inline QChar ByteArrayRowViewPrivate::substituteChar()      const { return mByteArrayColumn->substituteChar(); }
inline QChar ByteArrayRowViewPrivate::undefinedChar()       const { return mByteArrayColumn->undefinedChar(); }
inline bool ByteArrayRowViewPrivate::isByteTypeColored()    const { return mByteArrayColumn->isByteTypeColored(); }

}

#endif
