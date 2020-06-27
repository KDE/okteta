/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTDIFFERENTSIZEDIALOG_HPP
#define KASTEN_ABSTRACTDIFFERENTSIZEDIALOG_HPP

// Kasten core
#include <Kasten/KastenCore>

namespace Kasten {

class AbstractDifferentSizeDialog
{
protected:
    AbstractDifferentSizeDialog();

public:
    virtual ~AbstractDifferentSizeDialog();

public: // API to be implemented
    virtual Answer query(int newValueSize, int oldValueSize, int sizeLeft) = 0;
};

inline AbstractDifferentSizeDialog::AbstractDifferentSizeDialog() = default;
inline AbstractDifferentSizeDialog::~AbstractDifferentSizeDialog() = default;

}

#endif
