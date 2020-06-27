/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETVIEW_HPP
#define KASTEN_GOTOOFFSETVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolWidget>

namespace Okteta {
class AddressComboBox;
}

class QPushButton;

class QCheckBox;

namespace Kasten {

class GotoOffsetTool;

class GotoOffsetView : public AbstractToolWidget
{
    Q_OBJECT

public:
    explicit GotoOffsetView(GotoOffsetTool* tool, QWidget* parent = nullptr);
    ~GotoOffsetView() override;

public:
    GotoOffsetTool* tool() const;

private Q_SLOTS: // gui
    void onGotoButtonClicked();
    void onFormatChanged(int index);
    void onAddressTypeChanged(int newType);

private Q_SLOTS: // tool
    void onApplyableChanged(bool isApplyable);

private:
    GotoOffsetTool* mTool;

    Okteta::AddressComboBox* mAddressEdit;
    QPushButton* mGotoButton;
    QCheckBox* mAtCursorCheckBox;
    QCheckBox* mExtendSelectionCheckBox;
    QCheckBox* mBackwardsCheckBox;
};

inline GotoOffsetTool* GotoOffsetView::tool() const { return mTool; }

}

#endif
