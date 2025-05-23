/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCOLUMNVIEW_P_HPP
#define OKTETA_BYTEARRAYCOLUMNVIEW_P_HPP

// lib
#include "bytearraycolumnview.hpp"
#include "abstractbytearrayview_p.hpp"
#include "valuebytearraycolumnrenderer.hpp"
#include "charbytearraycolumnrenderer.hpp"

namespace Okteta {
class Coord;

class ByteArrayColumnViewPrivate : public AbstractByteArrayViewPrivate
{
    Q_DECLARE_PUBLIC(ByteArrayColumnView)

public:
    explicit ByteArrayColumnViewPrivate(ByteArrayColumnView* parent);
    ~ByteArrayColumnViewPrivate() override;

public:
    void init();

public:
    // value column
    [[nodiscard]]
    int /*PixelX*/ byteSpacingWidth() const;
    [[nodiscard]]
    int noOfGroupedBytes() const;
    [[nodiscard]]
    int /*PixelX*/ groupSpacingWidth() const;
    [[nodiscard]]
    int /*PixelX*/ binaryGapWidth() const;

    // char column
    [[nodiscard]]
    bool showsNonprinting() const;
    [[nodiscard]]
    QChar substituteChar() const;
    [[nodiscard]]
    QChar undefinedChar() const;

    [[nodiscard]]
    bool isByteTypeColored() const;

public: // logic value service
    [[nodiscard]]
    QSize minimumSizeHint() const;

public: // modification access
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

public: // AbstractByteArrayViewPrivate API
    void setByteArrayModel(AbstractByteArrayModel* byteArrayModel) override;
    void ensureVisible(const AddressRange& range, bool ensureStartVisible) override;
    void ensureCursorVisible() override;
    void placeCursor(QPoint point) override;
    [[nodiscard]]
    QRect cursorRect() const override;
    [[nodiscard]]
    Address indexByPoint(QPoint point) const override;
    void blinkCursor() override;

public:
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
    void setCharCoding(const QString& charCodingName);
    void setByteTypeColored(bool isColored);

private: // drawing related operations
    /** recreates the cursor pixmaps and paints active and inactive cursors if doable */
    void createCursorPixmaps();
    /** draws the blinking cursor or removes it */
    void drawActiveCursor(QPainter* painter);
    void drawInactiveCursor(QPainter* painter);
    void updateCursor(const AbstractByteArrayColumnRenderer& column);

private:
    /** recalcs a layout due to the resize style that fits into the view size
     * and updates the dependent values
     */
    void ensureVisible(const AbstractByteArrayColumnRenderer& column, Coord coord);
    void renderColumns(QPainter* painter, int cx, int cy, int cw, int ch) override;

protected: // AbstractByteArrayViewPrivate API
    // cursor control
    void pauseCursor() override;
    void updateCursors() override;

    void setActiveCoding(AbstractByteArrayView::CodingTypeId codingId) override;
    void setVisibleCodings(int visibleCodings) override;

protected: // AbstractByteArrayViewPrivate API
    [[nodiscard]]
    AbstractByteArrayView::CodingTypeId activeCoding() const override;
    [[nodiscard]]
    AbstractByteArrayView::CodingTypes visibleCodings() const override;
    [[nodiscard]]
    int fittingBytesPerLine() const override;

    void adjustToLayoutNoOfBytesPerLine() override;
    void adjustTToScaledFont() override;
    void updateChanged() override;

private:
    ValueByteArrayColumnRenderer* mValueColumn;
    BorderColumnRenderer*         mMiddleBorderColumn;
    CharByteArrayColumnRenderer*  mCharColumn;

    /** points to the column with keyboard focus */
    AbstractByteArrayColumnRenderer* mActiveColumn;
    /** points to the column without keyboard focus (if there is) */
    AbstractByteArrayColumnRenderer* mInactiveColumn;
};

inline PixelX ByteArrayColumnViewPrivate::byteSpacingWidth()  const { return mValueColumn->byteSpacingWidth(); }
inline int ByteArrayColumnViewPrivate::noOfGroupedBytes()      const { return mValueColumn->noOfGroupedBytes(); }
inline PixelX ByteArrayColumnViewPrivate::groupSpacingWidth() const { return mValueColumn->groupSpacingWidth(); }
inline PixelX ByteArrayColumnViewPrivate::binaryGapWidth()    const { return mValueColumn->binaryGapWidth(); }
inline bool ByteArrayColumnViewPrivate::showsNonprinting()     const { return mCharColumn->isShowingNonprinting(); }
inline QChar ByteArrayColumnViewPrivate::substituteChar()      const { return mCharColumn->substituteChar(); }
inline QChar ByteArrayColumnViewPrivate::undefinedChar()       const { return mCharColumn->undefinedChar(); }
inline bool ByteArrayColumnViewPrivate::isByteTypeColored()    const { return mValueColumn->isByteTypeColored(); }

}

#endif
