/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_LICENSEDIALOG_HPP
#define KASTEN_LICENSEDIALOG_HPP

// Qt
#include <QDialog>

class KAboutLicense;

namespace Kasten {

class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    LicenseDialog(const KAboutLicense& license, QWidget* parent);
};

}

#endif
