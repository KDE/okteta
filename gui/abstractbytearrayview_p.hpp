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
#include "fontscalingzoomstate.hpp"
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
    [[nodiscard]]
    AbstractByteArrayModel* byteArrayModel() const;

    // TODO: needed?
    [[nodiscard]]
    bool isModified() const;

    [[nodiscard]]
    bool isOverwriteMode() const;
    [[nodiscard]]
    bool isOverwriteOnly() const;
    [[nodiscard]]
    bool isViewReadOnly() const;
    [[nodiscard]]
    bool isEffectiveReadOnly() const;

    /** returns the index of the cursor position */
    [[nodiscard]]
    Address cursorPosition() const;
    /***/
    [[nodiscard]]
    bool isCursorBehind() const;

    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    AbstractByteArrayView::OffsetCoding offsetCoding() const;

    [[nodiscard]]
    int noOfBytesPerLine() const;
    [[nodiscard]]
    Address startOffset() const;
    [[nodiscard]]
    Address firstLineOffset() const;

    [[nodiscard]]
    AbstractByteArrayView::LayoutStyle layoutStyle() const;

    [[nodiscard]]
    bool tabChangesFocus() const;

    [[nodiscard]]
    bool hasSelectedBytes() const;
    [[nodiscard]]
    AddressRange selection() const;
    [[nodiscard]]
    QByteArray selectedBytes() const;
    [[nodiscard]]
    std::unique_ptr<QMimeData> selectedBytesAsMimeData() const;

    [[nodiscard]]
    AddressRange marking() const;

    [[nodiscard]]
    AbstractController* controller() const;
    [[nodiscard]]
    AbstractWheelController* wheelController() const;

    [[nodiscard]]
    ByteArrayTableLayout* tableLayout();
    [[nodiscard]]
    const ByteArrayTableLayout* tableLayout() const;
    [[nodiscard]]
    ByteArrayTableCursor* tableCursor();
    [[nodiscard]]
    const ByteArrayTableCursor* tableCursor() const;
    [[nodiscard]]
    ByteArrayTableRanges* tableRanges();
    [[nodiscard]]
    const ByteArrayTableRanges* tableRanges() const;

    [[nodiscard]]
    const ValueCodec* valueCodec() const;
    [[nodiscard]]
    AbstractByteArrayView::ValueCoding valueCoding() const;
    [[nodiscard]]
    const CharCodec* charCodec() const;
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
    [[nodiscard]]
    bool selectWord(Address index /*, Chartype*/);
    void setSelection(const AddressRange& selection);

    void setCursorPosition(Address index, bool isBehind);
    void setSelectionCursorPosition(Address index);

public:
    [[nodiscard]]
    QMenu* createStandardContextMenu(QPoint position);

public: // API to be implemented
    virtual void ensureVisible(const AddressRange& range, bool ensureStartVisible) = 0;
    virtual void ensureCursorVisible() = 0;
    virtual void placeCursor(QPoint point) = 0;
    [[nodiscard]]
    virtual QRect cursorRect() const = 0;
    [[nodiscard]]
    virtual Address indexByPoint(QPoint point) const = 0;
    virtual void blinkCursor() = 0;

public: // ColumnsViewScrollAreaEngine API
    void setNoOfLines(int newNoOfLines) override;

public: // events
    [[nodiscard]]
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
    [[nodiscard]]
    bool viewportEvent(QEvent* event);

    void keyPressEvent(QKeyEvent* keyEvent);
    void mousePressEvent(QMouseEvent* mousePressEvent);
    void mouseReleaseEvent(QMouseEvent* mouseReleaseEvent);
    void mouseMoveEvent(QMouseEvent* mouseMoveEvent);
    void mouseDoubleClickEvent(QMouseEvent* mouseDoubleClickEvent);
    void wheelEvent(QWheelEvent* wheelEvent);

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

protected:
    [[nodiscard]]
    const QFont& renderingFont() const;
    void initPainterFromWidget(QPainter* painter) const;

protected:
    [[nodiscard]]
    bool getNextChangedRange(CoordRange* changedRange, const CoordRange& visibleRange) const;
    [[nodiscard]]
    bool isByteEditorActive() const;

protected:
    void removeSelectedBytes();
    void insertBytes(const QByteArray& bytes);
    void insertBytesFromMimeData(const QMimeData* data);
    [[nodiscard]]
    bool canInsertBytesFromMimeData(const QMimeData* data) const;

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
    [[nodiscard]]
    virtual AbstractByteArrayView::CodingTypeId activeCoding() const = 0;
    [[nodiscard]]
    virtual AbstractByteArrayView::CodingTypes visibleCodings() const = 0;
    [[nodiscard]]
    virtual int fittingBytesPerLine() const = 0;
    /** recalcs all dependent values with the actual NoOfBytesPerLine  */
    virtual void adjustToLayoutNoOfBytesPerLine() = 0;
    virtual void adjustTToScaledFont() = 0;
    /** repaints all the parts that are signed as changed */
    virtual void updateChanged() = 0;

protected:
    void emitDoubleClicked(Address index);
    void emitCursorPositionChanged();
    void setMouseCursor(Qt::CursorShape cursorShape);

    [[nodiscard]]
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
    /** flag if the cursor should be invisible */
    bool mCursorPaused : 1;
    /** flag if the cursor is visible when blinking */
    bool mBlinkCursorVisible : 1;
    /** flag if the cursor is visible */
    bool mCursorVisible : 1;
    /** flag if the current focus was received by a mouse click */
    bool m_wasFocussedByMouseClick : 1;

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
    FontScalingZoomState m_fontScalingZoomState;

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
inline double AbstractByteArrayViewPrivate::zoomScale()         const { return m_fontScalingZoomState.scale(); }
inline int AbstractByteArrayViewPrivate::zoomInLevelsSize()     const { return m_fontScalingZoomState.inLevelsSize(); }
inline int AbstractByteArrayViewPrivate::zoomOutLevelsSize()    const { return m_fontScalingZoomState.outLevelsSize(); }

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
inline bool AbstractByteArrayViewPrivate::hasSelectedBytes()  const { return mTableRanges.hasSelection(); }

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

inline const QFont& AbstractByteArrayViewPrivate::renderingFont() const
{
    return m_fontScalingZoomState.font();
}

inline void AbstractByteArrayViewPrivate::setTabChangesFocus(bool tabChangesFocus)
{
    mTabController.setTabChangesFocus(tabChangesFocus);
}

inline void AbstractByteArrayViewPrivate::setModified(bool modified) { mByteArrayModel->setModified(modified); }

}

#endif
