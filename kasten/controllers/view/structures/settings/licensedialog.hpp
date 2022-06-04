/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LICENSEDIALOG_HPP
#define LICENSEDIALOG_HPP

// Qt
#include <QDialog>

class KAboutLicense;

class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    LicenseDialog(const KAboutLicense& license, QWidget* parent);
};

#endif
