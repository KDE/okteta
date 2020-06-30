/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTCOLUMNSTYLIST_HPP
#define KASTEN_PRINTCOLUMNSTYLIST_HPP

// Okteta gui
#include <Okteta/AbstractColumnStylist>
// Qt
#include <QPalette>

namespace Okteta {

class PrintColumnStylist : public Okteta::AbstractColumnStylist
{
public:
    PrintColumnStylist();
    ~PrintColumnStylist() override;

public: // AbstractColumnStylist API
    const QPalette& palette() const override;

private:
    QPalette mPalette = {Qt::white};
};

}

#endif
