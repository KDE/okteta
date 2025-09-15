/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTPREVIEWDIALOG_HPP
#define KASTEN_PRINTPREVIEWDIALOG_HPP

// Qt
#include <QPrintPreviewDialog>

namespace Kasten {

class PrintPreviewDialog : public QPrintPreviewDialog
{
    Q_OBJECT

public:
    /**
     * The dialog takes ownership of the @p printer.
     */
    explicit PrintPreviewDialog(QPrinter* printer, QWidget* parent = nullptr);
    ~PrintPreviewDialog() override;
};

}

#endif
