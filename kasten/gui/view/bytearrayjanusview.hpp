/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYJANUSVIEW_HPP
#define KASTEN_BYTEARRAYJANUSVIEW_HPP

// lib
#include "usermessagesoverlayedwidget.hpp"
// Okteta core
#include <Okteta/AddressRange>

class QMimeData;

namespace Okteta {
class AbstractByteArrayView;
class AbstractByteArrayModel;

class ByteArrayJanusView : public Kasten::UserMessagesOverlayedWidget
{
    Q_OBJECT

public:
    enum
    {
        ColumnViewId = 0,
        RowViewId
    };

public:
    explicit ByteArrayJanusView(QWidget* parent = nullptr);
    ~ByteArrayJanusView() override;

public:
    void setByteArrayModel(AbstractByteArrayModel* byteArrayModel);

    void setZoomScale(double zoomScale);
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
    void setReadOnly(bool isReadOnly);
    [[nodiscard]]
    bool isReadOnly() const;

public: // cursor API
    void setCursorPosition(Address cursorPosition);
    void setSelectionCursorPosition(Address index);
    [[nodiscard]]
    Address cursorPosition() const;
    [[nodiscard]]
    QRect cursorRect() const;

public: // codings
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    [[nodiscard]]
    QString charCodingName() const;
    [[nodiscard]]
    int valueCoding() const;

public:
    void setSelection(Address start, Address end);
    void selectAll(bool selectAll);
    void removeSelectedData();
    void insertBytesFromMimeData(const QMimeData* data);
    void insertBytes(const QByteArray& byteArray);
    [[nodiscard]]
    bool hasSelectedData() const;
    [[nodiscard]]
    std::unique_ptr<QMimeData> selectedBytesAsMimeData() const;
    [[nodiscard]]
    bool canReadData(const QMimeData* data) const;
    [[nodiscard]]
    AddressRange selection() const;

    void setMarking(const AddressRange& marking);
    void ensureVisible(const AddressRange& range);

public: // overwrite
    void setOverwriteMode(bool overwriteMode);
    [[nodiscard]]
    bool isOverwriteMode() const;
    [[nodiscard]]
    bool isOverwriteOnly() const;

public: // elements
    void toggleOffsetColumn(bool offsetColumnVisible);
    void setOffsetCoding(int offsetCoding);
    void setVisibleCodings(int columns);
    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    int offsetCoding() const;
    [[nodiscard]]
    int visibleCodings() const;

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setStartOffset(Address startOffset);
    void setFirstLineOffset(Address firstLineOffset);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    [[nodiscard]]
    Address startOffset() const;
    [[nodiscard]]
    Address firstLineOffset() const;
    [[nodiscard]]
    int noOfBytesPerLine() const;
    [[nodiscard]]
    int layoutStyle() const;

public: // layout settings
    void setShowsNonprinting(bool showsNonprinting = true);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);
    [[nodiscard]]
    QChar substituteChar() const;
    [[nodiscard]]
    QChar undefinedChar() const;
    [[nodiscard]]
    bool showsNonprinting() const;

    [[nodiscard]]
    int byteSpacingWidth() const;
    [[nodiscard]]
    int noOfGroupedBytes() const;
    [[nodiscard]]
    int groupSpacingWidth() const;
    [[nodiscard]]
    int binaryGapWidth() const;

public:
    void setViewModus(int viewModus);
    [[nodiscard]]
    int viewModus() const;

    void setViewPos(QPoint pos);
    [[nodiscard]]
    QRect viewRect() const;

    // needed, as the zooming logic currently changes the font for the real view,
    // so decouples from font change propagation
    void propagateFont(const QFont& font);

Q_SIGNALS:
    void readOnlyChanged(bool);
    void overwriteModeChanged(bool overwriteMode);
    void hasSelectedDataChanged(bool hasSelectedData);
    void selectionChanged(const Okteta::AddressRange& selection);
    void cursorPositionChanged(Okteta::Address cursorPosition);
    void valueCodingChanged(int valueCoding);
    void charCodecChanged(const QString& charCodingName);
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
    void viewModusChanged(int viewModus);
    void viewContextMenuRequested(QPoint pos);

private:
    AbstractByteArrayView* mView = nullptr;
    int mViewModus = -1;
};

inline int ByteArrayJanusView::viewModus()               const { return mViewModus; }

}

#endif
