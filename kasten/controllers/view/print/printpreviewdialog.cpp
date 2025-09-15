/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printpreviewdialog.hpp"

// Qt
#include <QPrinter>

namespace Kasten {

PrintPreviewDialog::PrintPreviewDialog(QPrinter* printer, QWidget* parent)
    : QPrintPreviewDialog(printer, parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
}

PrintPreviewDialog::~PrintPreviewDialog()
{
    delete printer();
}

}

#include "moc_printpreviewdialog.cpp"
