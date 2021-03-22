/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYVIEW_HPP
#define OKTETA_ABSTRACTBYTEARRAYVIEW_HPP

// lib
#include "columnsview.hpp"
// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QClipboard>

class QMenu;
class QMimeData;
class QByteArray;

namespace Okteta {
class Bookmark;
class ValueCodec;
class CharCodec;
class AbstractByteArrayModel;
class ArrayChangeMetricsList;

class AbstractController;
class AbstractWheelController;
class ByteArrayTableLayout;
class ByteArrayTableCursor;
class ByteArrayTableRanges;

class AbstractByteArrayViewPrivate;

// TODO: for now inherit from ColumnsView, but later on invert this,
// so it's AbstractByteArrayView < ColumnsView < {ByteArrayRowView,ByteArrayColumnView}
class OKTETAGUI_EXPORT AbstractByteArrayView : public ColumnsView
{
    friend class TabController;
    friend class KeyNavigator;
    friend class AbstractEditor;
    friend class ValueEditor;
    friend class CharEditor;
    friend class Dropper;
    friend class MouseNavigator;
    friend class MousePaster;
    friend class Dragger;
    friend class TapNavigator;

    Q_OBJECT
    Q_PROPERTY(bool OverwriteMode READ isOverwriteMode WRITE setOverwriteMode NOTIFY overwriteModeChanged)
    Q_PROPERTY(bool OverwriteOnly READ isOverwriteOnly WRITE setOverwriteOnly)
    Q_PROPERTY(bool Modified READ isModified WRITE setModified DESIGNABLE false)
    Q_PROPERTY(bool ReadOnly READ isReadOnly WRITE setReadOnly NOTIFY readOnlyChanged)

    // Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
    // Q_PROPERTY( QByteArray SelectedData READ selectedData )
    Q_PROPERTY(bool TabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus)
    Q_PROPERTY(LayoutStyle LayoutStyle READ layoutStyle WRITE setLayoutStyle NOTIFY layoutStyleChanged)
    Q_PROPERTY(int NoOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine NOTIFY noOfBytesPerLineChanged)
    Q_PROPERTY(int StartOffset READ startOffset WRITE setStartOffset)
    Q_PROPERTY(int FirstLineOffset READ firstLineOffset WRITE setFirstLineOffset)
    Q_PROPERTY(bool OffsetColumnVisible READ offsetColumnVisible WRITE toggleOffsetColumn NOTIFY offsetColumnVisibleChanged)
    Q_PROPERTY(OffsetCoding OffsetCoding READ offsetCoding WRITE setOffsetCoding NOTIFY offsetCodingChanged)
    Q_PROPERTY(CodingTypes VisibleCodings READ visibleCodings WRITE setVisibleCodings NOTIFY visibleByteArrayCodingsChanged)

    Q_PROPERTY(bool ByteTypeColored READ isByteTypeColored WRITE setByteTypeColored)
    // value column
    Q_PROPERTY(ValueCoding Coding READ valueCoding WRITE setValueCoding NOTIFY valueCodingChanged)
    Q_PROPERTY(int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth)
    Q_PROPERTY(int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes NOTIFY noOfGroupedBytesChanged)
    Q_PROPERTY(int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth)
    Q_PROPERTY(int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth)
    // char column
    Q_PROPERTY(bool ShowNonprinting READ showsNonprinting WRITE setShowsNonprinting NOTIFY showsNonprintingChanged)
    Q_PROPERTY(QChar SubstituteChar READ substituteChar WRITE setSubstituteChar NOTIFY substituteCharChanged)

public:
    enum OffsetCoding
    {
        HexadecimalOffset = 0,
        DecimalOffset = 1,
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
    enum CharCoding
    {
        LocalEncoding = 0,
        ISO8859_1Encoding = 1,
        EBCDIC1047Encoding = 2,
        StartOfOwnEncoding = 0x8000,
        MaxEncodingId = 0xFFFF
    };
    Q_ENUM(CharCoding)
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
    AbstractByteArrayView(AbstractByteArrayViewPrivate* d, QWidget* parent);

public:
    AbstractByteArrayView() = delete;
    ~AbstractByteArrayView() override;

public: // value access
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    bool isModified() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isReadOnly() const;

    /** returns the index of the cursor position */
    Address cursorPosition() const;
    /***/
    bool isCursorBehind() const;

    Address startOffset() const;
    Address firstLineOffset() const;
    int noOfBytesPerLine() const;

    LayoutStyle layoutStyle() const;

    bool tabChangesFocus() const;

    CodingTypes visibleCodings() const;
    CodingTypeId activeCoding() const;

    bool offsetColumnVisible() const;
    OffsetCoding offsetCoding() const;

    /** returns true if there is a selected range in the array */
    bool hasSelectedData() const;
    AddressRange selection() const;

    AddressRange marking() const;

    ValueCoding valueCoding() const;
    /**
     * @return encoding used for the chars
     */
    CharCoding charCoding() const;
    /**
     * @return name of the encoding used for the chars
     */
    const QString& charCodingName() const;

    double zoomLevel() const;

public: // value access API
    // value column
    virtual int /*PixelX*/ byteSpacingWidth() const = 0;
    virtual int noOfGroupedBytes() const = 0;
    virtual int /*PixelX*/ groupSpacingWidth() const = 0;
    virtual int /*PixelX*/ binaryGapWidth() const = 0;

    // char column
    /** reports if "non-printing" chars are displayed in the char column
     * with their original character. Default is false
     * @return @c true if original chars are displayed, otherwise @c false
     */
    virtual bool showsNonprinting() const = 0;
    /** gives the used substitute character for "unprintable" chars, default is '.'
     * @return substitute character
     */
    virtual QChar substituteChar() const = 0;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    virtual QChar undefinedChar() const = 0;

    virtual bool isByteTypeColored() const = 0;

public:
    virtual void setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel);

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
     * If the encoding is not available the format will not be changed. */
    virtual void setCharCoding(CharCoding charCoding) = 0;
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
    QByteArray selectedData() const;
    QMimeData* selectionAsMimeData() const;

public: // modification access
    void pasteData(const QMimeData* data);
    /** removes the selected data, takes care of the cursor */
    void removeSelectedData();
    /** inserts */
    void insert(const QByteArray& data);

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
    void placeCursor(const QPoint& point);

public:
    bool canReadData(const QMimeData* data) const;

public: // zooming
    void zoomIn(int pointInc);
    void zoomIn();
    void zoomOut(int pointDec);
    void zoomOut();
    void zoomTo(int pointSize);
    void unZoom();
    void setZoomLevel(double level);

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
    virtual void copy();
    virtual void cut();
    virtual void paste();

public:
    QRect cursorRect() const;
    QMenu* createStandardContextMenu(const QPoint& position);

public: // QWidget API
    QSize sizeHint() const override;

public: // logic value service
    /** detects the index of the byte at the given point
     * @param point in viewport coordinate system
     * @return index of the byte that covers the point
     */
    Address indexByPoint(const QPoint& point) const;

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

    void zoomLevelChanged(double level);

protected:
    void finishByteEdit();
    void emitSelectionSignals();
    void updateChanged();
    void copyToClipboard(QClipboard::Mode mode) const;
    void pasteFromClipboard(QClipboard::Mode mode);

protected:
    const Okteta::ValueCodec* valueCodec() const;
    const Okteta::CharCodec* charCodec() const;
    ByteArrayTableCursor* tableCursor() const;
    ByteArrayTableRanges* tableRanges() const;
    ByteArrayTableLayout* layout() const;

protected: // QWidget API
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void mousePressEvent(QMouseEvent* mousePressEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseReleaseEvent) override;
    void mouseMoveEvent(QMouseEvent* mouseMoveEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent) override;
    /// reimplemented to catch Tab and BackTab keys, which otherwise gets stolen
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

protected: // QAbstractScrollArea API
    void wheelEvent(QWheelEvent* e) override;
    bool viewportEvent(QEvent* event) override;

protected: // ColumnsView API
    void setNoOfLines(int newNoOfLines) override;

protected: // Q_SLOTS QWidget API
    void changeEvent(QEvent* event) override;

private:
    Q_PRIVATE_SLOT(d_func(), void onBookmarksChange(const QVector<Okteta::Bookmark> &bookmarks))
    Q_PRIVATE_SLOT(d_func(), void onRevertedToVersionIndex(int versionIndex))
//     Q_PRIVATE_SLOT( d_func(), void onClipboardChanged() )

private:
    Q_DECLARE_PRIVATE(AbstractByteArrayView)
};

}

#endif
