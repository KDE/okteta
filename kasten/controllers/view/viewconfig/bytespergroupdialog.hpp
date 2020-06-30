/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTESPERGROUPDIALOG_HPP
#define KASTEN_BYTESPERGROUPDIALOG_HPP

// Qt
#include <QDialog>

class QSpinBox;

namespace Kasten {

class BytesPerGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BytesPerGroupDialog(QWidget* parent = nullptr);

    ~BytesPerGroupDialog() override;

public:
    void setGroupedBytesCount(int groupedBytesCount);

public:
    int groupedBytesCount() const;

Q_SIGNALS:
    void bytesPerGroupAccepted(int groupedBytesCount);

private Q_SLOTS:
    void onFinished(int result);

private:
    QSpinBox* mGroupedBytesCountEdit;
};

}

#endif
