/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTESPERLINEDIALOG_HPP
#define KASTEN_BYTESPERLINEDIALOG_HPP

// Qt
#include <QDialog>

class QSpinBox;

namespace Kasten {

class BytesPerLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BytesPerLineDialog(QWidget* parent = nullptr);

    ~BytesPerLineDialog() override;

public:
    void setBytesPerLine(int bytesPerLine);

public:
    int bytesPerLine() const;

Q_SIGNALS:
    void bytesPerLineAccepted(int bytesPerLine);

private Q_SLOTS:
    void onFinished(int result);

private:
    QSpinBox* mBytesPerLineEdit;
};

}

#endif
