/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEW_HPP
#define KASTEN_BYTEARRAYVIEW_HPP

// lib
#include "oktetakastengui_export.hpp"
// Okteta Kasten core
#include <Kasten/Okteta/ByteArraySelection>
// Kasten gui
#include <Kasten/AbstractView>
#include <Kasten/Zoomable> // TODO: able instead of interface? or else?
#include <Kasten/ZoomLevelsQueryable>
#include <Kasten/DataSelectable>
#include <Kasten/SelectedDataWriteable>
#include <Kasten/UserErrorReportsInlineable>
#include <Kasten/UserNotificationsInlineable>
// Okteta core
#include <Okteta/AddressRange>
// Std
#include <memory>

namespace Okteta {
class ByteArrayJanusView;
}
class QRect;
class QPoint;

namespace Kasten {
class ByteArrayViewProfileSynchronizer;
class ByteArrayDocument;

class OKTETAKASTENGUI_EXPORT ByteArrayView : public AbstractView
                                           , public If::Zoomable
                                           , public If::ZoomLevelsQueryable
                                           , public If::DataSelectable
                                           , public If::SelectedDataWriteable
                                           , public If::UserErrorReportsInlineable
                                           , public If::UserNotificationsInlineable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::Zoomable
        Kasten::If::ZoomLevelsQueryable
        Kasten::If::DataSelectable
        Kasten::If::SelectedDataWriteable
        Kasten::If::UserErrorReportsInlineable
        Kasten::If::UserNotificationsInlineable
    )

public:
    enum
    {
        ColumnViewId = 0,
        RowViewId
    };

public:
    ByteArrayView(ByteArrayDocument* document, std::unique_ptr<ByteArrayViewProfileSynchronizer>&& synchronizer);
    ByteArrayView(ByteArrayView* other, std::unique_ptr<ByteArrayViewProfileSynchronizer>&& synchronizer,
                  Qt::Alignment alignment = {});

    ~ByteArrayView() override;

public:
//     KCursorObject *cursor() const;

public: // AbstractModel API
    QString title() const override;
    bool isModifiable() const override;
    bool isReadOnly() const override;
    void setReadOnly(bool isReadOnly) override;

public: // AbstractView API
    void setFocus() override;
    QWidget* widget() const override;
    bool hasFocus() const override;

public: // If::Zoomable API TODO: make all such methods slots?
    void setZoomScale(double zoomScale) override;
    double zoomScale() const override;
Q_SIGNALS:
    void zoomScaleChanged(double zoomScale) override;

public: // If::ZoomLevelsQueryable API
    int zoomInLevelsSize() const override;
    int zoomOutLevelsSize() const override;
    double zoomScaleForLevel(int zoomLevel) const override;
    int zoomLevelForScale(double zoomScale) const override;
Q_SIGNALS:
    void zoomLevelsChanged() override;

public: // If::DataSelectable API
    void selectAllData(bool selectAll) override;
    bool hasSelectedData() const override;
    std::unique_ptr<QMimeData> copySelectedData() const override;
    const AbstractModelSelection* modelSelection() const override;
//     virtual void setSelection();
Q_SIGNALS:
    void hasSelectedDataChanged(bool hasSelectedData) override;
    void selectedDataChanged(const Kasten::AbstractModelSelection* modelSelection) override;

public: // If::SelectedDataWriteable API
    void insertData(const QMimeData* data) override;
    std::unique_ptr<QMimeData> cutSelectedData() override;
    void deleteSelectedData() override;
    bool canReadData(const QMimeData* data) const override;
    bool canCutSelectedData() const override;
Q_SIGNALS:
    void canCutSelectedDataChanged(bool canCutSelectedData) override;

public: // If::UserErrorReportsInlineable API
    void showErrorReport(UserErrorReport* errorReport) override;
    bool isErrorReportShown() const override;

Q_SIGNALS:
    void errorReportHidden() override;

public: // If::UserNotificationsInlineable API
    void showNotification(UserNotification* notification) override;

public: // cursor API
    void setCursorPosition(Okteta::Address cursorPosition);
    void setSelectionCursorPosition(Okteta::Address index);
    Okteta::Address cursorPosition() const;
Q_SIGNALS:
    void cursorPositionChanged(Okteta::Address cursorPosition);

public: // codings
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    QString charCodingName() const;
    int valueCoding() const;
Q_SIGNALS:
    void charCodecChanged(const QString& charCodingName);
    void valueCodingChanged(int valueCoding);

public:
    // TODO: see how this can be solved by modelSelection
    Okteta::AddressRange selection() const;
    void setSelection(Okteta::Address start, Okteta::Address end);
    void insert(const QByteArray& byteArray);
    QRect cursorRect() const;

public: // overwrite
    void setOverwriteMode(bool overwriteMode);
    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
Q_SIGNALS:
    void overwriteModeChanged(bool overwriteMode);

public: // elements
    void toggleOffsetColumn(bool offsetColumnVisible);
    void setOffsetCoding(int offsetCoding);
    void setVisibleByteArrayCodings(int columns);
    void setMarking(const Okteta::AddressRange& range, bool ensureVisible = false);
    bool offsetColumnVisible() const;
    int offsetCoding() const;
    int visibleByteArrayCodings() const;
Q_SIGNALS:
    void offsetColumnVisibleChanged(bool visible);
    void visibleByteArrayCodingsChanged(int columns);
    void offsetCodingChanged(int offsetCoding);

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    Okteta::Address startOffset() const;
    Okteta::Address firstLineOffset() const;
    int noOfBytesPerLine() const;
    int layoutStyle() const;
Q_SIGNALS:
    void layoutStyleChanged(int layoutStyle);
    void noOfBytesPerLineChanged(int noOfBytesPerLine);

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
Q_SIGNALS:
    void showsNonprintingChanged(bool showsNonprinting);
    void substituteCharChanged(QChar substituteChar);
    void undefinedCharChanged(QChar undefinedChar);
    void noOfGroupedBytesChanged(int noOfGroupedBytes);

public:
    void setViewModus(int viewModus);
    int viewModus() const;
Q_SIGNALS:
    void viewModusChanged(int viewModus);

Q_SIGNALS:
    void viewContextMenuRequested(QPoint pos);

public:
    ByteArrayViewProfileSynchronizer* synchronizer() const;

private:
    OKTETAKASTENGUI_NO_EXPORT void init();

private Q_SLOTS:
    OKTETAKASTENGUI_NO_EXPORT void setFontByGlobalSettings();

    OKTETAKASTENGUI_NO_EXPORT void onSelectionChanged(const Okteta::AddressRange& selection);
    OKTETAKASTENGUI_NO_EXPORT void onOverwriteModeChanged(bool overwriteMode);

private:
    const std::unique_ptr<ByteArrayViewProfileSynchronizer> mByteArrayViewProfileSynchronizer;

    std::unique_ptr<Okteta::ByteArrayJanusView> mWidget;

    ByteArrayDocument* mDocument;
    ByteArraySelection mSelection;
};

}

#endif
