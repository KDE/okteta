/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SELECTRANGEVIEW_H
#define SELECTRANGEVIEW_H

// Kasten gui
#include <abstracttoolwidget.h>

namespace Okteta {
class AddressComboBox;
}

class KPushButton;

class QCheckBox;


namespace Kasten2
{

class SelectRangeTool;


class SelectRangeView : public AbstractToolWidget
{
  Q_OBJECT

  public:
    explicit SelectRangeView( SelectRangeTool* tool, QWidget* parent = 0 );
    virtual ~SelectRangeView();

  public:
    SelectRangeTool* tool() const;

  private Q_SLOTS: // gui
    void onSelectButtonClicked();

  private Q_SLOTS: // tool
    void onApplyableChanged( bool isApplyable );

  private:
    SelectRangeTool* mTool;

    Okteta::AddressComboBox* mStartEdit;
    Okteta::AddressComboBox* mEndEdit;
    QCheckBox* mRelativeCheckBox;
    QCheckBox* mBackwardsCheckBox;
    KPushButton* mSelectButton;
};


inline SelectRangeTool* SelectRangeView::tool() const { return mTool; }

}

#endif
