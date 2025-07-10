/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYVIEW_HPP
#define OKTETA_ABSTRACTBYTEARRAYVIEW_HPP

// lib
#include "oktetagui_export.hpp"
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QAbstractScrollArea>
// Std
#include <memory>

class QMenu;
class QMimeData;
class QByteArray;

namespace Okteta {
class Bookmark;
class ValueCodec;
class CharCodec;
class AbstractByteArrayModel;

class AbstractByteArrayViewPrivate;

class OKTETAGUI_EXPORT AbstractByteArrayView : public QAbstractScrollArea
{
    friend class Dropper;

    Q_OBJECT
    Q_PROPERTY(bool overwriteMode READ isOverwriteMode WRITE setOverwriteMode NOTIFY overwriteModeChanged)
    Q_PROPERTY(bool overwriteOnly READ isOverwriteOnly WRITE setOverwriteOnly)
    Q_PROPERTY(bool modified READ isModified WRITE setModified DESIGNABLE false)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly NOTIFY readOnlyChanged)

    // Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
    // Q_PROPERTY( QByteArray selectedData READ selectedData )
    Q_PROPERTY(bool tabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus)
    Q_PROPERTY(LayoutStyle layoutStyle READ layoutStyle WRITE setLayoutStyle NOTIFY layoutStyleChanged)
    Q_PROPERTY(int noOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine NOTIFY noOfBytesPerLineChanged)
    Q_PROPERTY(int startOffset READ startOffset WRITE setStartOffset)
    Q_PROPERTY(int firstLineOffset READ firstLineOffset WRITE setFirstLineOffset)
    Q_PROPERTY(bool offsetColumnVisible READ offsetColumnVisible WRITE toggleOffsetColumn NOTIFY offsetColumnVisibleChanged)
    Q_PROPERTY(OffsetCoding offsetCoding READ offsetCoding WRITE setOffsetCoding NOTIFY offsetCodingChanged)
    Q_PROPERTY(CodingTypes visibleCodings READ visibleCodings WRITE setVisibleCodings NOTIFY visibleByteArrayCodingsChanged)

    Q_PROPERTY(bool byteTypeColored READ isByteTypeColored WRITE setByteTypeColored)
    // value column
    Q_PROPERTY(ValueCoding valueCoding READ valueCoding WRITE setValueCoding NOTIFY valueCodingChanged)
    Q_PROPERTY(int byteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth)
    Q_PROPERTY(int noOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes NOTIFY noOfGroupedBytesChanged)
    Q_PROPERTY(int groupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth)
    Q_PROPERTY(int binaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth)
    // char column
    Q_PROPERTY(bool showNonprinting READ showsNonprinting WRITE setShowsNonprinting NOTIFY showsNonprintingChanged)
    Q_PROPERTY(QChar substituteChar READ substituteChar WRITE setSubstituteChar NOTIFY substituteCharChanged)

public:
    enum OffsetCoding
    {
        HexadecimalOffset = 0,
        DecimalOffset = 1,
        OctalOffset = 2,
        MaxOffsetCodingId = 0xFF
    };
    Q_ENUM(OffsetCoding)
    enum ValueCoding
    {
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        MaxCodingId = 0xFFFF
    };
    Q_ENUM(ValueCoding)
    enum LayoutStyle
    {
        FixedLayoutStyle = 0,
        WrapOnlyByteGroupsLayoutStyle = 1,
        FullSizeLayoutStyle = 2,
        LastUserLayout = 0xFF
    };
    Q_ENUM(LayoutStyle)

    enum CodingTypeId
    {
        NoCodingId = 0,
        ValueCodingId = 1,
        CharCodingId = 2
    };
    enum CodingTypes
    {
        OnlyValueCoding = ValueCodingId,
        OnlyCharCoding = CharCodingId,
        ValueAndCharCodings = ValueCodingId | CharCodingId
    };
    Q_ENUM(CodingTypes)

protected:
    OKTETAGUI_NO_EXPORT AbstractByteArrayView(std::unique_ptr<AbstractByteArrayViewPrivate>&& dd, QWidget* parent);

public:
    ~AbstractByteArrayView() override;

public: // value access
    [[nodiscard]]
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    [[nodiscard]]
    bool isModified() const;

    [[nodiscard]]
    bool isOverwriteMode() const;
    [[nodiscard]]
    bool isOverwriteOnly() const;
    [[nodiscard]]
    bool isReadOnly() const;

    /** returns the index of the cursor position */
    [[nodiscard]]
    Address cursorPosition() const;
    /***/
    [[nodiscard]]
    bool isCursorBehind() const;

    [[nodiscard]]
    Address startOffset() const;
    [[nodiscard]]
    Address firstLineOffset() const;
    [[nodiscard]]
    int noOfBytesPerLine() const;

    [[nodiscard]]
    LayoutStyle layoutStyle() const;

    [[nodiscard]]
    bool tabChangesFocus() const;

    [[nodiscard]]
    CodingTypes visibleCodings() const;
    [[nodiscard]]
    CodingTypeId activeCoding() const;

    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    OffsetCoding offsetCoding() const;

    /** returns true if there is a selected range in the array */
    [[nodiscard]]
    bool hasSelectedData() const;
    [[nodiscard]]
    AddressRange selection() const;

    [[nodiscard]]
    AddressRange marking() const;

    [[nodiscard]]
    ValueCoding valueCoding() const;
    /**
     * @return name of the encoding used for the chars
     */
    [[nodiscard]]
    QString charCodingName() const;

public: // zooming values
    [[nodiscard]]
    double zoomScale() const;
    [[nodiscard]]
    int zoomInLevelsSize() const;
    [[nodiscard]]
    int zoomOutLevelsSize() const;
    [[nodiscard]]
    double zoomScaleForLevel(int zoomLevel) const;
    [[nodiscard]]
    int zoomLevelForScale(double zoomScale) const;

public: // value access API
    // value column
    [[nodiscard]]
    virtual int /*PixelX*/ byteSpacingWidth() const = 0;
    [[nodiscard]]
    virtual int noOfGroupedBytes() const = 0;
    [[nodiscard]]
    virtual int /*PixelX*/ groupSpacingWidth() const = 0;
    [[nodiscard]]
    virtual int /*PixelX*/ binaryGapWidth() const = 0;

    // char column
    /** reports if "non-printing" chars are displayed in the char column
     * with their original character. Default is false
     * @return @c true if original chars are displayed, otherwise @c false
     */
    [[nodiscard]]
    virtual bool showsNonprinting() const = 0;
    /** gives the used substitute character for "unprintable" chars, default is '.'
     * @return substitute character
     */
    [[nodiscard]]
    virtual QChar substituteChar() const = 0;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    [[nodiscard]]
    virtual QChar undefinedChar() const = 0;

    [[nodiscard]]
    virtual bool isByteTypeColored() const = 0;

public:
    void setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel);

    // setting parameters
    // value column parameters
    /** sets the spacing between the bytes in the value column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * default is 3
     */
    virtual void setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth) = 0;
    /** sets the number of grouped bytes in the value column
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * default is 4
     */
    virtual void setNoOfGroupedBytes(int noOfGroupedBytes) = 0;
    /** sets the spacing between the groups of bytes in the value column
     * @param groupSpacingWidth spacing between the groups in pixels
     * default is 9
     */
    virtual void setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth) = 0;
    /** sets the spacing in the middle of a binary byte in the value column
     * @param binaryGapWidth spacing in the middle of a binary in pixels
     * returns true if there was a change
     */
    virtual void setBinaryGapWidth(int binaryGapWidth) = 0;
    /** sets the spacing in the value column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * @param groupSpacingWidth spacing between the groups in pixels
     * Default is 4 for NoOfGroupedBytes
     */
    virtual void setBufferSpacing(int /*PixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int /*PixelX*/ groupSpacingWidth = 0) = 0;
    /** sets the format of the value column. Default is Okteta::HexadecimalCoding */
    virtual void setValueCoding(ValueCoding valueCoding) = 0;
    // char column parameters
    /** sets whether control chars or "non-printing" chars should be displayed in the char column
     * with their corresponding character. Currently this simply means all chars with value <32,
     * as known from the ASCII.
     * @param showsNonprinting
     * returns true if there was a change
     */
    virtual void setShowsNonprinting(bool showsNonprinting = true) = 0;
    /** sets the substitute character for "non-printing" chars
     * returns true if there was a change
     */
    virtual void setSubstituteChar(QChar substituteChar) = 0;
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    virtual void setUndefinedChar(QChar undefinedChar) = 0;
    /** sets the encoding of the char column. Default is Okteta::LocalEncoding.
     * If the encoding is not available the format will not be changed.
     * @param charCodingName name of the encoding
     */
    virtual void setCharCoding(const QString& charCodingName) = 0;
    virtual void setByteTypeColored(bool isColored) = 0;

public:
    /** sets whether the data should be treated modified or not */
    void setModified(bool modified);

    /** sets the resizestyle for the value column. Default is Okteta::FixedLayoutStyle */
    void setLayoutStyle(LayoutStyle layoutStyle);
    /** sets whether the widget is readonly or not, Default is true.
     * If the databuffer which is worked on can't be written the widget stays readonly
     */
    void setReadOnly(bool readOnly);
    /** sets whether the widget is overwriteonly or not. Default is false. */
    void setOverwriteOnly(bool overwriteOnly);
    /** sets whether the widget is in overwrite mode or not. Default is true. */
    void setOverwriteMode(bool overwriteMode);

    /** sets the number of bytes per line, switching the resize style to Okteta::FixedLayoutStyle */
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    /** sets absolut offset of the data */
    void setStartOffset(Address startOffset);
    /** sets offset of the char in the upper left corner */
    void setFirstLineOffset(Address firstLineOffset);

    /** sets whether on a tab key there should be switched from the char column back to the value column
     * or be switched to the next focusable widget. Default is false
     */
    void setTabChangesFocus(bool tabChangesFocus = true);

    /***/
    void setActiveCoding(CodingTypeId codingId);
    /** */
    void setVisibleCodings(int visibleCodings);

    /** switches the Offset column on/off */
    void toggleOffsetColumn(bool offsetColumnVisible);
    /** sets the format of the offset column. Default is Okteta::HexadecimalCoding */
    void setOffsetCoding(OffsetCoding offsetCoding);

public:
    /**
     * @return deep copy of the selected data
     */
    [[nodiscard]]
    QByteArray selectedBytes() const;
    [[nodiscard]]
    std::unique_ptr<QMimeData> selectedBytesAsMimeData() const;

public: // modification access
    void insertBytesFromMimeData(const QMimeData* data);
    /** removes the selected data, takes care of the cursor */
    void removeSelectedData();
    /** inserts */
    void insertBytes(const QByteArray& bytes);

    /** puts the cursor to the position of index, handles all drawing
     * @param index
     * @param isBehind
     */
    void setCursorPosition(Address index, bool isBehind = false);
    void setSelectionCursorPosition(Address index);

    /** de-/selects all data */
    void selectAll(bool select);

    void setSelection(Address start, Address end);
    void setSelection(const AddressRange& selection);
    /** selects word at index, returns true if there is one */
    [[nodiscard]]
    bool selectWord(/*unsigned*/ Address index /*, Chartype*/);

    void setMarking(Address start, Address end);
    void setMarking(const AddressRange& marking);

    /**
     * @param range an address range
     * @param ensureStartVisible if @c true scrolls the view as much as needed to have the start of the range
     * fully visible, otherwise to the end of the range
     */
    void ensureVisible(const AddressRange& range, bool ensureStartVisible = false);
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor(QPoint point);

public:
    [[nodiscard]]
    bool canReadData(const QMimeData* data) const;

public: // zooming
    void zoomIn(int pointInc);
    void zoomIn();
    void zoomOut(int pointDec);
    void zoomOut();
    void zoomTo(int pointSize);
    void unZoom();
    void setZoomScale(double zoomScale);

public: // cursor control
    /** we have focus again, start the timer */
    void startCursor();
    /** we lost focus, stop the timer */
    void stopCursor();
    /** simply pauses any blinking, i.e. ignores any calls to blinkCursor */
    void pauseCursor();
    /** undoes pauseCursor */
    void unpauseCursor();

public:
    // clipboard interaction
    void copy() const;
    void cut();
    void paste();

public:
    [[nodiscard]]
    QRect cursorRect() const;
    [[nodiscard]]
    QMenu* createStandardContextMenu(QPoint position);

public: // QWidget API
    [[nodiscard]]
    QSize sizeHint() const override;

public: // logic value service
    /** detects the index of the byte at the given point
     * @param point in viewport coordinate system
     * @return index of the byte that covers the point
     */
    [[nodiscard]]
    Address indexByPoint(QPoint point) const;

Q_SIGNALS:
    /** Index of the byte that was clicked */
    void clicked(Okteta::Address index);
    /** Index of the byte that was double clicked */
    void doubleClicked(Okteta::Address index);

    void cursorPositionChanged(Okteta::Address index);
    /** */
    void overwriteModeChanged(bool newOverwriteMode);
    /** */
    void readOnlyChanged(bool isReadOnly);
    /** selection has changed */
    void hasSelectedDataChanged(bool hasSelectedData);
    void selectionChanged(const Okteta::AddressRange& selection);
    /** there is a cut available or not */
    void cutAvailable(bool Really);
    /** there is a copy available or not */
    void copyAvailable(bool Really);
    /** */
    void charCodecChanged(const QString& codecName);
    /** */
    void valueCodingChanged(int valueCoding);
    void focusChanged(bool hasFocus);

    void offsetColumnVisibleChanged(bool visible);
    void offsetCodingChanged(int offsetCoding);
    void visibleByteArrayCodingsChanged(int columns);
    void layoutStyleChanged(int layoutStyle);
    void noOfBytesPerLineChanged(int noOfBytesPerLine);
    void showsNonprintingChanged(bool showsNonprinting);
    void substituteCharChanged(QChar substituteChar);
    void undefinedCharChanged(QChar undefinedChar);
    void noOfGroupedBytesChanged(int noOfGroupedBytes);

    void zoomScaleChanged(double zoomScale);
    void zoomLevelsChanged();

protected:
    [[nodiscard]]
    const Okteta::ValueCodec* valueCodec() const;
    [[nodiscard]]
    const Okteta::CharCodec* charCodec() const;

protected: // QWidget API
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void mousePressEvent(QMouseEvent* mousePressEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseReleaseEvent) override;
    void mouseMoveEvent(QMouseEvent* mouseMoveEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent) override;
    /// reimplemented to catch Tab and BackTab keys, which otherwise gets stolen
    [[nodiscard]]
    bool event(QEvent* event) override;
    void showEvent(QShowEvent* showEvent) override;
    void focusInEvent(QFocusEvent* focusEvent) override;
    void focusOutEvent(QFocusEvent* focusEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;
    void dragEnterEvent(QDragEnterEvent* dragEnterEvent) override;
    void dragMoveEvent(QDragMoveEvent* dragMoveEvent) override;
    void dragLeaveEvent(QDragLeaveEvent* dragLeaveEvent) override;
    void dropEvent(QDropEvent* dropEvent) override;
    void contextMenuEvent(QContextMenuEvent* contextMenuEvent) override;
    void timerEvent(QTimerEvent* timerEvent) override;
    void paintEvent(QPaintEvent* paintEvent) override;

protected: // QAbstractScrollArea API
    void wheelEvent(QWheelEvent* wheelEvent) override;
    [[nodiscard]]
    bool viewportEvent(QEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

protected: // Q_SLOTS QWidget API
    void changeEvent(QEvent* event) override;

protected:
    const std::unique_ptr<AbstractByteArrayViewPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void onBookmarksChange(const QVector<Okteta::Bookmark> &bookmarks))
    Q_PRIVATE_SLOT(d_func(), void onRevertedToVersionIndex(int versionIndex))
//     Q_PRIVATE_SLOT( d_func(), void onClipboardChanged() )

private:
    Q_DECLARE_PRIVATE(AbstractByteArrayView)
};

}

#endif
