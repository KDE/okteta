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
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    bool isModifiable() const override;
    [[nodiscard]]
    bool isReadOnly() const override;
    void setReadOnly(bool isReadOnly) override;

public: // AbstractView API
    void setFocus() override;
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    bool hasFocus() const override;

public: // If::Zoomable API TODO: make all such methods slots?
    void setZoomScale(double zoomScale) override;
    [[nodiscard]]
    double zoomScale() const override;
Q_SIGNALS:
    void zoomScaleChanged(double zoomScale) override;

public: // If::ZoomLevelsQueryable API
    [[nodiscard]]
    int zoomInLevelsSize() const override;
    [[nodiscard]]
    int zoomOutLevelsSize() const override;
    [[nodiscard]]
    double zoomScaleForLevel(int zoomLevel) const override;
    [[nodiscard]]
    int zoomLevelForScale(double zoomScale) const override;
Q_SIGNALS:
    void zoomLevelsChanged() override;

public: // If::DataSelectable API
    void selectAllData(bool selectAll) override;
    [[nodiscard]]
    bool hasSelectedData() const override;
    std::unique_ptr<QMimeData> copySelectedData() const override;
    [[nodiscard]]
    const AbstractModelSelection* modelSelection() const override;
//     virtual void setSelection();
Q_SIGNALS:
    void hasSelectedDataChanged(bool hasSelectedData) override;
    void selectedDataChanged(const Kasten::AbstractModelSelection* modelSelection) override;

public: // If::SelectedDataWriteable API
    void insertData(const QMimeData* data) override;
    std::unique_ptr<QMimeData> cutSelectedData() override;
    void deleteSelectedData() override;
    [[nodiscard]]
    bool canReadData(const QMimeData* data) const override;
    [[nodiscard]]
    bool canCutSelectedData() const override;
Q_SIGNALS:
    void canCutSelectedDataChanged(bool canCutSelectedData) override;

public: // If::UserErrorReportsInlineable API
    void showErrorReport(UserErrorReport* errorReport) override;
    [[nodiscard]]
    bool isErrorReportShown() const override;

Q_SIGNALS:
    void errorReportHidden() override;

public: // If::UserNotificationsInlineable API
    void showNotification(UserNotification* notification) override;

public: // cursor API
    void setCursorPosition(Okteta::Address cursorPosition);
    void setSelectionCursorPosition(Okteta::Address index);
    [[nodiscard]]
    Okteta::Address cursorPosition() const;
Q_SIGNALS:
    void cursorPositionChanged(Okteta::Address cursorPosition);

public: // codings
    void setValueCoding(int valueCoding);
    void setCharCoding(const QString& charCodingName);
    [[nodiscard]]
    QString charCodingName() const;
    [[nodiscard]]
    int valueCoding() const;
Q_SIGNALS:
    void charCodecChanged(const QString& charCodingName);
    void valueCodingChanged(int valueCoding);

public:
    // TODO: see how this can be solved by modelSelection
    [[nodiscard]]
    Okteta::AddressRange selection() const;
    void setSelection(Okteta::Address start, Okteta::Address end);
    void insert(const QByteArray& byteArray);
    [[nodiscard]]
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
    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    int offsetCoding() const;
    [[nodiscard]]
    int visibleByteArrayCodings() const;
Q_SIGNALS:
    void offsetColumnVisibleChanged(bool visible);
    void visibleByteArrayCodingsChanged(int columns);
    void offsetCodingChanged(int offsetCoding);

public: // table layout
    void setLayoutStyle(int layoutStyle);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    [[nodiscard]]
    Okteta::Address startOffset() const;
    [[nodiscard]]
    Okteta::Address firstLineOffset() const;
    [[nodiscard]]
    int noOfBytesPerLine() const;
    [[nodiscard]]
    int layoutStyle() const;
Q_SIGNALS:
    void layoutStyleChanged(int layoutStyle);
    void noOfBytesPerLineChanged(int noOfBytesPerLine);

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
Q_SIGNALS:
    void showsNonprintingChanged(bool showsNonprinting);
    void substituteCharChanged(QChar substituteChar);
    void undefinedCharChanged(QChar undefinedChar);
    void noOfGroupedBytesChanged(int noOfGroupedBytes);

public:
    void setViewModus(int viewModus);
    [[nodiscard]]
    int viewModus() const;
Q_SIGNALS:
    void viewModusChanged(int viewModus);

Q_SIGNALS:
    void viewContextMenuRequested(QPoint pos);

public:
    [[nodiscard]]
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
