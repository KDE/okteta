/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWSTATUSCONTROLLER_HPP
#define KASTEN_VIEWSTATUSCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Okteta gui
#include <Okteta/OffsetFormat>
// Okteta core
#include <Okteta/Address>

class KComboBox;
class KSqueezedTextLabel;
class QLabel;
class QString;

namespace Kasten {

class ByteArrayView;
class ToggleButton;
class StatusBar;
class AbstractModelSelection;

class ViewStatusController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ViewStatusController(StatusBar* statusBar);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private:
    void fixWidths(int offsetCoding);

private Q_SLOTS:
    void setOverwriteMode(bool overwrite);
    void setValueCoding(int valueCoding);
    void setCharCoding(int charCoding);

    void onCursorPositionChanged(Okteta::Address offset);
    void onSelectedDataChanged(const Kasten::AbstractModelSelection* modelSelection);
    void onOffsetCodingChanged(int offsetCoding);
    void onValueCodingChanged(int valueCoding);
    void onCharCodecChanged(const QString& charCodecName);

private:
    ByteArrayView* mByteArrayView = nullptr;

    StatusBar* mStatusBar;
    QLabel* mOffsetLabel;
    QLabel* mSelectionLabel;
    ToggleButton* mOverwriteModeToggleButton;
    KComboBox* mValueCodingComboBox;
    KComboBox* mCharCodingComboBox;

    Okteta::OffsetFormat::print mPrintFunction;
    Okteta::Address mStartOffset;
};

}

#endif
