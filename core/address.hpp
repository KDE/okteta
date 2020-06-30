/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ADDRESS_HPP
#define OKTETA_ADDRESS_HPP

// Qt
#include <QtGlobal>

namespace Okteta {

// Address needs to be <= int, as long as we use QByteArray and its int parameters
using Address = qint32;

}

#endif
