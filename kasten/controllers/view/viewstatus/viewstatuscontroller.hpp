/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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
