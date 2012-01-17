/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef FILTERVIEW_H
#define FILTERVIEW_H

// Kasten gui
#include <abstracttoolwidget.h>

class AbstractByteArrayFilterParameterSet;

class KPushButton;
class KComboBox;

class QStackedWidget;
class QString;


namespace Kasten2
{

class FilterTool;


class FilterView : public AbstractToolWidget
{
  Q_OBJECT

  public:
    explicit FilterView( FilterTool *tool, QWidget* parent = 0 );
    virtual ~FilterView();

  public:
    FilterTool* tool() const;

  private: // get 
    void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const;

  private: // separation
    void addFilters();

  private Q_SLOTS:
    void onFilterClicked();
    void onOperationChange( int index );
    void onHasWriteableChanged( bool hasWriteable );
    void onCharCodecChanged( const QString &charCodecName );
    void onValidityChanged( bool isValid );

  private:
    FilterTool *mTool;
    KComboBox *mOperationComboBox;
    QStackedWidget *mParameterSetEditStack;
    KPushButton *mFilterButton;
};


inline FilterTool* FilterView::tool() const { return mTool; }

}

#endif
