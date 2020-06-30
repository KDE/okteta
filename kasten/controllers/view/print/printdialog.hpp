/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTDIALOG_HPP
#define KASTEN_PRINTDIALOG_HPP

// Qt
#include <QPrintDialog>

namespace Kasten {

class PrintDialog : public QPrintDialog
{
    Q_OBJECT

public:
    /**
     * The dialog takes ownership of the @p printer, but passes it back via
     * the accepted(QPrinter* printer) signal if the dialog is accepted.
     * Otherwise will delete it in the constructor.
     */
    explicit PrintDialog(QPrinter* printer, QWidget* parent = nullptr);
    ~PrintDialog() override;
};

}

#endif
