/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYJANUSVIEW_HPP
#define KASTEN_BYTEARRAYJANUSVIEW_HPP

// Okteta core
#include <Okteta/AddressRange>
// Qt
#include <QWidget>

class QHBoxLayout;
class QMimeData;

namespace Okteta {
class AbstractByteArrayView;
class AbstractByteArrayModel;

class ByteArrayJanusView : public QWidget
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

    void setZoomLevel(double Level);
    double zoomLevel() const;

public:
    void setReadOnly(bool isReadOnly);
    bool isReadOnly() const;

public: // cursor API
    void setCursorPosition(Address cursorPosition);
    void setSelectionCursorPosition(Address index);
    Address cursorPosition() const;
    QRect cursorRect() const;

public: // codings
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    QString charCodingName() const;
    int valueCoding() const;

public:
    void setSelection(Address start, Address end);
    void selectAll(bool selectAll);
    void removeSelectedData();
    void pasteData(const QMimeData* data);
    void insert(const QByteArray& byteArray);
    bool hasSelectedData() const;
    QMimeData* selectionAsMimeData() const;
    bool canReadData(const QMimeData* data) const;
    AddressRange selection() const;

    void setMarking(const AddressRange& marking);
    void ensureVisible(const AddressRange& range);

public: // overwrite
    void setOverwriteMode(bool overwriteMode);
    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;

public: // elements
    void toggleOffsetColumn(bool visible);
    void setOffsetCoding(int offsetCoding);
    void setVisibleCodings(int columns);
    bool offsetColumnVisible() const;
    int offsetCoding() const;
    int visibleCodings() const;

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setStartOffset(Address startOffset);
    void setFirstLineOffset(Address firstLineOffset);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    Address startOffset() const;
    Address firstLineOffset() const;
    int noOfBytesPerLine() const;
    int layoutStyle() const;

public: // layout settings
    void setShowsNonprinting(bool showsNonprinting = true);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);
    QChar substituteChar() const;
    QChar undefinedChar() const;
    bool showsNonprinting() const;

    int byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int groupSpacingWidth() const;
    int binaryGapWidth() const;

public:
    void setViewModus(int viewModus);
    int viewModus() const;

    void setViewPos(QPoint pos);
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
    void zoomLevelChanged(double level);
    void viewModusChanged(int viewModus);
    void viewContextMenuRequested(QPoint pos);

private:
    AbstractByteArrayView* mView = nullptr;
    int mViewModus = -1;
    QHBoxLayout* mLayout;
};

inline int ByteArrayJanusView::viewModus()               const { return mViewModus; }

}

#endif
