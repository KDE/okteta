/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWPROFILESMANAGEDIALOG_H
#define VIEWPROFILESMANAGEDIALOG_H

// lib
#include <bytearrayviewprofile.h>
// KDE
#include <KDialog>

template< class C > class QList;
class QTreeView;


namespace Kasten2
{

class ByteArrayViewProfileManager;
class ViewProfileTableModel;


class ViewProfilesManageDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit ViewProfilesManageDialog( ByteArrayViewProfileManager* viewProfileManager,
                                       QWidget* parent = 0 );

    virtual ~ViewProfilesManageDialog();

  protected Q_SLOTS:
    void onViewProfileSelectionChanged();

    void onCreateNewButtonClicked();
    void onEditButtonClicked();
    void onSetDefaultButtonClicked();
    void onDeleteButtonClicked();

    void onModelReset();
    void onViewProfilesLocked( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );
    void onViewProfilesUnlocked( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );
    void onDefaultViewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId );

  private:
    ByteArrayViewProfileManager* mViewProfileManager;
    ViewProfileTableModel* mViewProfileTableModel;
    ByteArrayViewProfile::Id mCurrentViewProfileId;

    QTreeView* mViewProfileTableView;
    KPushButton* mEditButton;
    KPushButton* mSetDefaultButton;
    KPushButton* mDeleteButton;
};

}

#endif
