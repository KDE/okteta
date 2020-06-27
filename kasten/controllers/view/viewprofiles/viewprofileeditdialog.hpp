/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012-2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILEEDITDIALOG_HPP
#define KASTEN_VIEWPROFILEEDITDIALOG_HPP

// Qt
#include <QDialog>

class QPushButton;

namespace Kasten {
class ViewProfileEdit;
class ByteArrayViewProfile;

class ViewProfileEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewProfileEditDialog(QWidget* parent = nullptr);

    ~ViewProfileEditDialog() override;

public:
    ByteArrayViewProfile viewProfile() const;

public:
    void setViewProfile(const ByteArrayViewProfile& viewProfile);

Q_SIGNALS:
    void viewProfileAccepted(const Kasten::ByteArrayViewProfile& viewProfile);

private Q_SLOTS:
    void onProfileTitleChanged(const QString& title);
    void onFinished(int result);

private:
    ViewProfileEdit* mViewProfileEdit;
    QString mId;
    QPushButton* mOkButton;
};

}

#endif
