/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTRANGEVIEW_HPP
#define KASTEN_SELECTRANGEVIEW_HPP

// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>
// Kasten gui
#include <Kasten/AbstractToolWidget>


class QPushButton;

class QCheckBox;

namespace Kasten {

class SelectRangeTool;

class SelectRangeView : public AbstractToolWidget
{
    Q_OBJECT

public:
    enum SelectDirection
    {
        SelectForward = 0,
        SelectBackward = 1,
    };

private:
    static inline constexpr Okteta::AddressComboBox::Coding DefaultStartOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;
    static inline constexpr Okteta::AddressComboBox::Coding DefaultEndOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;
    static inline constexpr bool DefaultRelativeToEnd = false;
    static inline constexpr SelectDirection DefaultDirection = SelectForward;

    static inline constexpr char ConfigGroupId[] = "SelectRangeTool";

    static inline constexpr char StartOffsetCodingConfigKey[] = "StartOffsetCoding";
    static inline constexpr char EndOffsetCodingConfigKey[] = "EndOffsetCoding";
    static inline constexpr char RelativeToEndConfigKey[] = "RelativeToEnd";
    static inline constexpr char DirectionConfigKey[] = "Direction";

public:
    explicit SelectRangeView(SelectRangeTool* tool, QWidget* parent = nullptr);
    ~SelectRangeView() override;

public:
    [[nodiscard]]
    SelectRangeTool* tool() const;

private Q_SLOTS: // gui
    void onSelectButtonClicked();

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);

private:
    SelectRangeTool* const mTool;

    Okteta::AddressComboBox* mStartEdit;
    Okteta::AddressComboBox* mEndEdit;
    QCheckBox* mRelativeCheckBox;
    QCheckBox* mBackwardsCheckBox;
    QPushButton* mSelectButton;
};

inline SelectRangeTool* SelectRangeView::tool() const { return mTool; }

}

#endif
