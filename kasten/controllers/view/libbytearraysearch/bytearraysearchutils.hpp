/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSEARCHUTILS_HPP
#define KASTEN_BYTEARRAYSEARCHUTILS_HPP

// libfinddialog
#include <finddirection.hpp>
// Okteta core
#include <Okteta/AddressRange>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

namespace ByteArraySearchUtils {

bool getSearchIndexes(Okteta::Address* searchFirstIndex, Okteta::Address* searchLastIndex,
                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                      Okteta::AddressRange selection,
                      Okteta::Address cursorPosition,
                      const QByteArray& searchData,
                      FindDirection direction,
                      bool fromCursor, bool inSelection);

}

}

#endif
