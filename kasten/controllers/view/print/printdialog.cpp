/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printdialog.hpp"
// Qt
#include <QPrinter>

namespace Kasten {

PrintDialog::PrintDialog(QPrinter* printer, QWidget* parent)
    : QPrintDialog(printer, parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
}

PrintDialog::~PrintDialog()
{
    if (result() != QDialog::Accepted) {
        delete printer();
    }
}

}
