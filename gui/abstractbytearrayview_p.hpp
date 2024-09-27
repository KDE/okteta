/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010, 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYVIEW_P_HPP
#define OKTETA_ABSTRACTBYTEARRAYVIEW_P_HPP

// lib
#include "columnsviewscrollareaengine.hpp"
#include "abstractbytearrayview.hpp"
#include "bytearraytablecursor.hpp"
#include "bytearraytableranges.hpp"
#include "bytearraytablelayout.hpp"
#include "cursor.hpp"
#include "controller/undoredocontroller.hpp"
#include "controller/chareditor.hpp"
#include "controller/clipboardcontroller.hpp"
#include "controller/dropper.hpp"
#include "controller/keynavigator.hpp"
#include "controller/mousenavigator.hpp"
#include "controller/mousepaster.hpp"
#include "controller/tabcontroller.hpp"
#include "controller/tapnavigator.hpp"
#include "controller/valueeditor.hpp"
#include "controller/zoomwheelcontroller.hpp"
#include "controller/zoompinchcontroller.hpp"
#include "offsetcolumnrenderer.hpp"
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
// Qt
#include <QClipboard>
// Std
#include <memory>

namespace Okteta {

class WidgetColumnStylist;
class BorderColumnRenderer;

class AbstractByteArrayViewPrivate : public ColumnsViewScrollAreaEngine
{
    friend class AbstractEditor;
    friend class CharEditor;
    friend class ClipboardController;
    friend class Dragger;
    friend class Dropper;
    friend class KeyNavigator;
    friend class MouseNavigator;
    friend class MousePaster;
    friend class TabController;
    friend class TapNavigator;
    friend class UndoRedoController;
    friend class ValueEditor;

public:
    explicit AbstractByteArrayViewPrivate(AbstractByteArrayView* parent);

    ~AbstractByteArrayViewPrivate() override;

public: // value access
    AbstractByteArrayModel* byteArrayModel() const;

    // TODO: needed?
    bool isModified() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isViewReadOnly() const;
    bool isEffectiveReadOnly() const;

    /** returns the index of the cursor position */
    Address cursorPosition() const;
    /***/
    bool isCursorBehind() const;

    bool offsetColumnVisible() const;
    AbstractByteArrayView::OffsetCoding offsetCoding() const;

    int noOfBytesPerLine() const;
    Address startOffset() const;
    Address firstLineOffset() const;

    AbstractByteArrayView::LayoutStyle layoutStyle() const;

    bool tabChangesFocus() const;

    bool hasSelectedData() const;
    AddressRange selection() const;
    QByteArray selectedData() const;
    std::unique_ptr<QMimeData> selectionAsMimeData() const;

    AddressRange marking() const;

    AbstractController* controller() const;
    AbstractWheelController* wheelController() const;

    ByteArrayTableLayout* tableLayout();
    const ByteArrayTableLayout* tableLayout() const;
    ByteArrayTableCursor* tableCursor();
    const ByteArrayTableCursor* tableCursor() const;
    ByteArrayTableRanges* tableRanges();
    const ByteArrayTableRanges* tableRanges() const;

    const ValueCodec* valueCodec() const;
    AbstractByteArrayView::ValueCoding valueCoding() const;
    const CharCodec* charCodec() const;
    QString charCodingName() const;

public: // zooming values
    double zoomScale() const;
    int zoomInLevelsSize() const;
    int zoomOutLevelsSize() const;
    double zoomScaleForLevel(int zoomLevel) const;
    int zoomLevelForScale(double zoomScale) const;

public:
    void init();
    void setReadOnly(bool readOnly);
    void setOverwriteOnly(bool overwriteOnly);
    void setOverwriteMode(bool overwriteMode);

    virtual void setByteArrayModel(AbstractByteArrayModel* byteArrayModel);

public: // setting parameters
    void setValueCoding(AbstractByteArrayView::ValueCoding valueCoding);
    void setCharCoding(const QString& charCodingName);
    void setLayoutStyle(AbstractByteArrayView::LayoutStyle layoutStyle);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    void setStartOffset(Address startOffset);
    void setFirstLineOffset(Address firstLineOffset);
    void setModified(bool modified);

    void setTabChangesFocus(bool tabChangesFocus = true);

    void setMarking(Address start, Address end);
    void setMarking(const AddressRange& marking);

    void toggleOffsetColumn(bool offsetColumnVisible);
    void setOffsetCoding(AbstractByteArrayView::OffsetCoding offsetCoding);

public: // zooming
    void zoomIn(int pointInc);
    void zoomIn();
    void zoomOut(int pointDec);
    void zoomOut();
    void zoomTo(int pointSize);
    void unZoom();
    void setZoomScale(double zoomScale);

public: // interaction
    void selectAll(bool select);
    bool selectWord(Address index /*, Chartype*/);
    void setSelection(const AddressRange& selection);

    void setCursorPosition(Address index, bool isBehind);
    void setSelectionCursorPosition(Address index);

public:
    QMenu* createStandardContextMenu(QPoint position);

public: // API to be implemented
    virtual void ensureVisible(const AddressRange& range, bool ensureStartVisible) = 0;
    virtual void ensureCursorVisible() = 0;
    virtual void placeCursor(QPoint point) = 0;
    virtual QRect cursorRect() const = 0;
    virtual Address indexByPoint(QPoint point) const = 0;
    virtual void blinkCursor() = 0;

public: // ColumnsViewScrollAreaEngine API
    void setNoOfLines(int newNoOfLines) override;

public: // events
    bool event(QEvent* event);
    void changeEvent(QEvent* event);
    void resizeEvent(QResizeEvent* resizeEvent);
    void focusInEvent(QFocusEvent* focusEvent);
    void focusOutEvent(QFocusEvent* focusEvent);
    void dragEnterEvent(QDragEnterEvent* dragEnterEvent);
    void dragMoveEvent(QDragMoveEvent* dragMoveEvent);
    void dragLeaveEvent(QDragLeaveEvent* dragLeaveEvent);
    void dropEvent(QDropEvent* dropEvent);
    void contextMenuEvent(QContextMenuEvent* contextMenuEvent);
    void timerEvent(QTimerEvent* timerEvent);
    bool viewportEvent(QEvent* event);

    void mousePressEvent(QMouseEvent* mousePressEvent);
    void mouseReleaseEvent(QMouseEvent* mouseReleaseEvent);
    void mouseMoveEvent(QMouseEvent* mouseMoveEvent);
    void mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent);

public: // slots
    void onContentsChanged(const ArrayChangeMetricsList& changeList);
    void onBookmarksChange(const QVector<Bookmark>& bookmarks);
    void onRevertedToVersionIndex(int versionIndex);
    void onByteArrayReadOnlyChange(bool isByteArrayReadOnly);
//     void onClipboardChanged();

protected:
    /** handles screen update in case of a change to any of the width sizes
     */
    void updateViewByWidth();
    void adjustLayoutToSize();
    void adaptController();
    void cancelByteEditor();
    void finishByteEditor();
    void initPainterFromWidget(QPainter* painter) const;

protected:
    bool getNextChangedRange(CoordRange* changedRange, const CoordRange& visibleRange) const;
    bool isByteEditorActive() const;

protected:
    void removeSelectedData();
    void insert(const QByteArray& data);
    void pasteData(const QMimeData* data);
    bool canReadData(const QMimeData* data) const;

protected: // clipboard interaction
    void cutToClipboard(QClipboard::Mode mode = QClipboard::Clipboard);
    void copyToClipboard(QClipboard::Mode mode = QClipboard::Clipboard) const;
    void pasteFromClipboard(QClipboard::Mode mode = QClipboard::Clipboard);

protected: // cursor control
    void startCursor();
    void stopCursor();
    void unpauseCursor();

protected: // API to be implemented
    // cursor control
    virtual void pauseCursor() = 0;
    virtual void updateCursors() = 0;

    virtual void setActiveCoding(AbstractByteArrayView::CodingTypeId codingId) = 0;
    virtual void setVisibleCodings(int visibleCodings) = 0;

protected: // API to be implemented
    virtual AbstractByteArrayView::CodingTypeId activeCoding() const = 0;
    virtual AbstractByteArrayView::CodingTypes visibleCodings() const = 0;
    virtual int fittingBytesPerLine() const = 0;
    /** recalcs all dependent values with the actual NoOfBytesPerLine  */
    virtual void adjustToLayoutNoOfBytesPerLine() = 0;
    /** repaints all the parts that are signed as changed */
    virtual void updateChanged() = 0;

protected:
    void emitDoubleClicked(Address index);
    void emitCursorPositionChanged();
    void setMouseCursor(Qt::CursorShape cursorShape);
    QPoint mapViewportFromGlobal(QPoint pos) const;

private:
    /** Emits updates on selection & cursor position after a change */
    void emitSelectionUpdates();
    void endViewUpdate();
    void onCursorFlashTimeChanged(int flashTime);

protected:
    AbstractByteArrayModel* mByteArrayModel;

    /** holds the logical layout */
    ByteArrayTableLayout mTableLayout;
    /** */
    ByteArrayTableCursor mTableCursor;
    /** */
    ByteArrayTableRanges mTableRanges;

    OffsetColumnRenderer* mOffsetColumn;
    BorderColumnRenderer* mOffsetBorderColumn;

    std::unique_ptr<WidgetColumnStylist> mStylist;

protected:
    TapNavigator mTapNavigator;
    ZoomPinchController mZoomPinchController;

    /** */
    TabController mTabController;
    /** */
    UndoRedoController mUndoRedoController;
    /** */
    ClipboardController mClipboardController;
    /** */
    KeyNavigator mKeyNavigator;
    /** */
    ValueEditor mValueEditor;
    /** */
    CharEditor mCharEditor;

    MousePaster mMousePaster;
    MouseNavigator mMouseNavigator;

    ZoomWheelController mZoomWheelController;

    Dropper mDropper;

    /** the current input controller */
    AbstractController* mController;
    AbstractMouseController* mMouseController;
    AbstractWheelController* mWheelController;

protected:
    /** Timer that controls the blinking of the cursor */
    int mCursorBlinkTimerId = 0;

    /** object to store the blinking cursor pixmaps */
    Cursor mCursorPixmaps;

protected:
    /** flag whether the widget is set to readonly. Cannot override the databuffer's setting, of course. */
    bool mReadOnly : 1;
    /** flag if only overwrite is allowed */
    bool mOverWriteOnly : 1;
    /** flag if overwrite mode is active */
    bool mOverWrite : 1;
    /** flag whether the font is changed due to a zooming */
    bool mInZooming : 1;
    /** flag if the cursor should be invisible */
    bool mCursorPaused : 1;
    /** flag if the cursor is visible when blinking */
    bool mBlinkCursorVisible : 1;
    /** flag if the cursor is visible */
    bool mCursorVisible : 1;

    /** font size as set by user (used for zooming) */
    int mDefaultFontSize;
    double m_zoomScale = 1.0;
    int m_zoomInLevels = 0;
    int m_zoomOutLevels = 0;

    // parameters
    /** */
    std::unique_ptr<const ValueCodec> mValueCodec;
    /** */
    AbstractByteArrayView::ValueCoding mValueCoding;
    /** */
    std::unique_ptr<const CharCodec> mCharCodec;
    /** style of resizing */
    AbstractByteArrayView::LayoutStyle mResizeStyle;

private:
    Q_DECLARE_PUBLIC(AbstractByteArrayView)
};

inline AbstractByteArrayModel* AbstractByteArrayViewPrivate::byteArrayModel() const { return mByteArrayModel; }
inline AbstractController* AbstractByteArrayViewPrivate::controller() const { return mController; }
inline AbstractWheelController* AbstractByteArrayViewPrivate::wheelController() const { return mWheelController; }

inline bool AbstractByteArrayViewPrivate::isOverwriteMode() const { return mOverWrite; }
inline bool AbstractByteArrayViewPrivate::isOverwriteOnly() const { return mOverWriteOnly; }
inline bool AbstractByteArrayViewPrivate::isViewReadOnly()      const { return mReadOnly; }
inline bool AbstractByteArrayViewPrivate::isEffectiveReadOnly() const { return mReadOnly || mByteArrayModel->isReadOnly(); }
inline double AbstractByteArrayViewPrivate::zoomScale()         const { return m_zoomScale; }
inline int AbstractByteArrayViewPrivate::zoomInLevelsSize()     const { return m_zoomInLevels; }
inline int AbstractByteArrayViewPrivate::zoomOutLevelsSize()    const { return m_zoomOutLevels; }

inline const ByteArrayTableLayout* AbstractByteArrayViewPrivate::tableLayout() const { return &mTableLayout; }
inline ByteArrayTableLayout* AbstractByteArrayViewPrivate::tableLayout() { return &mTableLayout; }
inline const ByteArrayTableCursor* AbstractByteArrayViewPrivate::tableCursor() const { return &mTableCursor; }
inline ByteArrayTableCursor* AbstractByteArrayViewPrivate::tableCursor() { return &mTableCursor; }
inline const ByteArrayTableRanges* AbstractByteArrayViewPrivate::tableRanges() const { return &mTableRanges; }
inline ByteArrayTableRanges* AbstractByteArrayViewPrivate::tableRanges() { return &mTableRanges; }

inline const ValueCodec* AbstractByteArrayViewPrivate::valueCodec()  const { return mValueCodec.get(); }
inline AbstractByteArrayView::ValueCoding AbstractByteArrayViewPrivate::valueCoding() const { return mValueCoding; }
inline const CharCodec* AbstractByteArrayViewPrivate::charCodec()   const { return mCharCodec.get(); }
inline QString AbstractByteArrayViewPrivate::charCodingName() const { return mCharCodec->name(); }
inline AbstractByteArrayView::LayoutStyle AbstractByteArrayViewPrivate::layoutStyle() const { return mResizeStyle; }
inline Address AbstractByteArrayViewPrivate::cursorPosition()   const { return mTableCursor.realIndex(); }
inline bool AbstractByteArrayViewPrivate::isCursorBehind()  const { return mTableCursor.isBehind(); }
inline bool AbstractByteArrayViewPrivate::isModified()      const { return mByteArrayModel->isModified(); }
inline int AbstractByteArrayViewPrivate::noOfBytesPerLine()    const { return mTableLayout.noOfBytesPerLine(); }
inline Address AbstractByteArrayViewPrivate::firstLineOffset() const { return mTableLayout.firstLineOffset(); }
inline Address AbstractByteArrayViewPrivate::startOffset()     const { return mTableLayout.startOffset(); }

inline AddressRange AbstractByteArrayViewPrivate::selection() const { return mTableRanges.selection(); }
inline bool AbstractByteArrayViewPrivate::hasSelectedData()   const { return mTableRanges.hasSelection(); }

inline AddressRange AbstractByteArrayViewPrivate::marking() const { return mTableRanges.marking(); }

inline bool AbstractByteArrayViewPrivate::tabChangesFocus()      const { return mTabController.tabChangesFocus(); }
inline bool AbstractByteArrayViewPrivate::isByteEditorActive()   const { return mValueEditor.isInEditMode(); }
inline bool AbstractByteArrayViewPrivate::offsetColumnVisible()  const { return mOffsetColumn->isVisible(); }
inline AbstractByteArrayView::OffsetCoding AbstractByteArrayViewPrivate::offsetCoding() const
{
    return (AbstractByteArrayView::OffsetCoding)mOffsetColumn->format();
}

inline void AbstractByteArrayViewPrivate::cancelByteEditor()
{
    mValueEditor.cancelEdit(false);
}
inline void AbstractByteArrayViewPrivate::finishByteEditor()
{
    mValueEditor.finishEdit();
}
inline void AbstractByteArrayViewPrivate::setTabChangesFocus(bool tabChangesFocus)
{
    mTabController.setTabChangesFocus(tabChangesFocus);
}

inline void AbstractByteArrayViewPrivate::setModified(bool modified) { mByteArrayModel->setModified(modified); }

}

#endif
