/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTRANGEVIEW_HPP
#define KASTEN_SELECTRANGEVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolWidget>

namespace Okteta {
class AddressComboBox;
}

class QPushButton;

class QCheckBox;

namespace Kasten {

class SelectRangeTool;

class SelectRangeView : public AbstractToolWidget
{
    Q_OBJECT

public:
    explicit SelectRangeView(SelectRangeTool* tool, QWidget* parent = nullptr);
    ~SelectRangeView() override;

public:
    SelectRangeTool* tool() const;

private Q_SLOTS: // gui
    void onSelectButtonClicked();

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);

private:
    SelectRangeTool* mTool;

    Okteta::AddressComboBox* mStartEdit;
    Okteta::AddressComboBox* mEndEdit;
    QCheckBox* mRelativeCheckBox;
    QCheckBox* mBackwardsCheckBox;
    QPushButton* mSelectButton;
};

inline SelectRangeTool* SelectRangeView::tool() const { return mTool; }

}

#endif
