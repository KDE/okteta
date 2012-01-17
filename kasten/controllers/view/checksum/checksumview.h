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

#ifndef CHECKSUMVIEW_H
#define CHECKSUMVIEW_H

// Kasten gui
#include <abstracttoolwidget.h>

class AbstractByteArrayChecksumParameterSet;

class KComboBox;
class KPushButton;
class KLineEdit;

class QStackedWidget;


namespace Kasten2
{

class ChecksumTool;


class ChecksumView : public AbstractToolWidget
{
  Q_OBJECT

  public:
    explicit ChecksumView( ChecksumTool* tool, QWidget* parent = 0 );
    virtual ~ChecksumView();

  public:
    ChecksumTool* tool() const;

  private:
    void getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const;

  private:
    void addAlgorithms();

  private Q_SLOTS: // gui
//     void setDirty( bool dirty );
    void onOperationChange( int index );
    void onCalculateClicked();

  private Q_SLOTS: // tool
    void onChecksumUptodateChanged( bool checksumUptodate );
    void onApplyableChanged( bool isApplyable );
    void onValidityChanged( bool isValid );

  private:
    ChecksumTool* mTool;

    KComboBox* mAlgorithmComboBox;
    QStackedWidget* mParameterSetEditStack;
    KLineEdit* mChecksumLabel;
    KPushButton* mCalculateButton;
};


inline ChecksumTool* ChecksumView::tool() const { return mTool; }

}

#endif
