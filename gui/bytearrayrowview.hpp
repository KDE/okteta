/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYROWVIEW_HPP
#define OKTETA_BYTEARRAYROWVIEW_HPP

// lib
#include "abstractbytearrayview.hpp"

namespace Okteta {
class ByteArrayRowViewPrivate;

class OKTETAGUI_EXPORT ByteArrayRowView : public AbstractByteArrayView
{
    Q_OBJECT

public:
    explicit ByteArrayRowView(QWidget* parent = nullptr);
    ~ByteArrayRowView() override;

public: // AbstractByteArrayView API
    // value column
    int /*PixelX*/ byteSpacingWidth() const override;
    int noOfGroupedBytes() const override;
    int /*PixelX*/ groupSpacingWidth() const override;
    int /*PixelX*/ binaryGapWidth() const override;

    // char column
    bool showsNonprinting() const override;
    QChar substituteChar() const override;
    QChar undefinedChar() const override;

    bool isByteTypeColored() const override;

    void setByteArrayModel(AbstractByteArrayModel* byteArrayModel) override;
    void setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth) override;
    void setNoOfGroupedBytes(int noOfGroupedBytes) override;
    void setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth) override;
    void setBinaryGapWidth(int binaryGapWidth) override;
    void setBufferSpacing(int /*PixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int /*PixelX*/ groupSpacingWidth = 0) override;
    void setValueCoding(ValueCoding valueCoding) override;
    // char column parameters
    void setShowsNonprinting(bool showsNonprinting = true) override;
    void setSubstituteChar(QChar substituteChar) override;
    void setUndefinedChar(QChar undefinedChar) override;
    void setCharCoding(CharCoding charCoding) override;
    void setCharCoding(const QString& charCodingName) override;
    void setByteTypeColored(bool isColored) override;

public: // ColumnsView API
    void renderColumns(QPainter* painter, int cx, int cy, int cw, int ch) override;

public: // QWidget API
    QSize minimumSizeHint() const override;

public: // logic value service
    /** calculates the number of bytes per line that fit into a widget with the given size
     * tests whether a vertical scroll bar is needed at all or not due to the given width
     * takes the frame width into account
     * @param TestSize Size the widget might have
     * @return number of bytes per line that fit into a widget with the given size
     */
//     int fittingBytesPerLine() const;

public: // modification access
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

protected: // Q_SLOTS QWidget API
    void changeEvent(QEvent* event) override;

private:
    Q_DECLARE_PRIVATE(ByteArrayRowView)
};

}

#endif
