/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012-2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILESMANAGEDIALOG_HPP
#define KASTEN_VIEWPROFILESMANAGEDIALOG_HPP

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayViewProfile>
// Qt
#include <QDialog>

template <class C> class QVector;
class QTreeView;
class QPushButton;

namespace Kasten {

class ByteArrayViewProfileManager;
class ViewProfileTableModel;

class ViewProfilesManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewProfilesManageDialog(ByteArrayViewProfileManager* viewProfileManager,
                                      QWidget* parent = nullptr);

    ~ViewProfilesManageDialog() override;

private Q_SLOTS:
    void onViewProfileSelectionChanged();

    void onCreateNewButtonClicked();
    void onEditButtonClicked();
    void onSetDefaultButtonClicked();
    void onDeleteButtonClicked();

    void saveViewProfile(const Kasten::ByteArrayViewProfile& viewProfile);

    void onModelReset();
    void onViewProfilesLocked(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);
    void onViewProfilesUnlocked(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);
    void onDefaultViewProfileChanged(const Kasten::ByteArrayViewProfile::Id& viewProfileId);

private:
    ByteArrayViewProfileManager* mViewProfileManager;
    ViewProfileTableModel* mViewProfileTableModel;
    ByteArrayViewProfile::Id mCurrentViewProfileId;

    QTreeView* mViewProfileTableView;
    QPushButton* mEditButton;
    QPushButton* mSetDefaultButton;
    QPushButton* mDeleteButton;

    QPushButton* mCloseButton;
};

}

#endif
