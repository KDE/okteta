/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETVIEW_HPP
#define KASTEN_GOTOOFFSETVIEW_HPP

// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>
// Kasten gui
#include <Kasten/AbstractToolWidget>

class QPushButton;

class QCheckBox;

namespace Kasten {

class GotoOffsetTool;

class GotoOffsetView : public AbstractToolWidget
{
    Q_OBJECT

public:
    enum GotoDirection
    {
        GotoForward = 0,
        GotoBackward = 1,
    };

private:
    static constexpr char ConfigGroupId[] = "GotoOffsetTool";

    static constexpr char OffsetCodingConfigKey[] = "OffsetCoding";
    static constexpr char FromCursorConfigKey[] = "FromCursor";
    static constexpr char ExtendSelectionConfigKey[] = "ExtendSelection";
    static constexpr char DirectionConfigKey[] = "Direction";

    static constexpr bool DefaultFromCursor = false;
    static constexpr bool DefaultExtendSelection = false;
    static constexpr GotoDirection DefaultDirection = GotoForward;
    static constexpr Okteta::AddressComboBox::Coding DefaultOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;

public:
    explicit GotoOffsetView(GotoOffsetTool* tool, QWidget* parent = nullptr);
    ~GotoOffsetView() override;

public:
    [[nodiscard]]
    GotoOffsetTool* tool() const;

private Q_SLOTS: // gui
    void onGotoButtonClicked();
    void onFormatChanged(int index);
    void onAddressTypeChanged(Okteta::AddressComboBox::AddressType addressType);

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);

private:
    GotoOffsetTool* const mTool;

    Okteta::AddressComboBox* mAddressEdit;
    QPushButton* mGotoButton;
    QCheckBox* mAtCursorCheckBox;
    QCheckBox* mExtendSelectionCheckBox;
    QCheckBox* mBackwardsCheckBox;
};

inline GotoOffsetTool* GotoOffsetView::tool() const { return mTool; }

}

#endif
