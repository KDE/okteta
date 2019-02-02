/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_BYTEARRAYCOLUMNVIEW_HPP
#define OKTETA_BYTEARRAYCOLUMNVIEW_HPP

// lib
#include <okteta/abstractbytearrayview.hpp>

namespace Okteta {
class ByteArrayColumnViewPrivate;

/** the main widget
 *
 * The functions split up in helper functions and those that are complete.
 *
 * Complete functions can be called from the outside and leave the widget in
 * a consistent state. They care for exceptions so one can safely call them in all
 * situations (like empty buffer, cursor behind end etc.)
 *
 * Helper functions do only partial tasks and need to be completed. They often do not
 * check for exceptions so one has to care for this.
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT ByteArrayColumnView : public AbstractByteArrayView
{
    Q_OBJECT

public:
    explicit ByteArrayColumnView(QWidget* parent = nullptr);
    ~ByteArrayColumnView() override;

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
    Q_DECLARE_PRIVATE(ByteArrayColumnView)

private: // Disabling copy constructor and operator= - not useful
    ByteArrayColumnView(const ByteArrayColumnView&);
    ByteArrayColumnView& operator=(const ByteArrayColumnView&);
};

}

#endif
