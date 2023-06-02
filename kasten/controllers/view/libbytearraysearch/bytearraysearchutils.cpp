/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysearchutils.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>

namespace Kasten {

namespace ByteArraySearchUtils {

// TODO: add a compare API to AbstractByteArrayModel
static
bool isSearchDataEqual(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::AddressRange selection,
                       const QByteArray& searchData)
{
    if (searchData.size() != selection.width()) {
        return false;
    }
    Okteta::Address address = selection.start();
    for (int i = 0; i < searchData.size(); ++i, ++address) {
        if (byteArrayModel->byte(address) != static_cast<Okteta::Byte>(searchData.at(i))) {
            return false;
        }
    }
    return true;
}

bool getSearchIndexes(Okteta::Address* searchFirstIndex, Okteta::Address* searchLastIndex,
                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                      Okteta::AddressRange selection,
                      Okteta::Address cursorPosition,
                      const QByteArray& searchData,
                      FindDirection direction,
                      bool fromCursor, bool inSelection)
{
    if (inSelection) {
        if (!selection.isValid()) {
            // nothing selected, so skip any search
            return false;
        }
        if (searchData.size() > selection.width()) {
            // searched data does not even fit, so skip any search
            // TODO: catch in dialog already
            return false ;
        }

        *searchFirstIndex = selection.start();
        *searchLastIndex =  selection.end();
    } else {
        if (searchData.size() > byteArrayModel->size()) {
            // searched data does not even fit, so skip any search
            // also handles case of empty bytearray
            // TODO: catch in dialog already
            return false;
        }

        if (fromCursor) {
            // assuming that selection is coupled with cursor
            if (selection.isValid()) {
                // skip current selection if matching
                if (isSearchDataEqual(byteArrayModel, selection, searchData)) {
                    if (direction == FindForward) {
                        *searchFirstIndex = selection.nextBehindEnd();
                        *searchLastIndex = selection.end();
                    } else {
                        *searchLastIndex = selection.start() - 1;
                        *searchFirstIndex = selection.start();
                    }
                } else {
                    // treating selection as extended cursor
                    if (direction == FindForward) {
                        *searchFirstIndex = selection.start();
                        // no wrap needed when starting at 0
                        *searchLastIndex = (selection.start() == 0) ? byteArrayModel->size() - 1 : selection.start() - 1;
                    } else {
                        *searchLastIndex = selection.end();
                        // no wrap needed when starting at end
                        *searchFirstIndex = (selection.end() == byteArrayModel->size() - 1) ? 0 : selection.nextBehindEnd();
                    }
                }
            } else {
                if (direction == FindForward) {
                    *searchFirstIndex = cursorPosition;
                    // no wrap needed when starting at 0
                    *searchLastIndex = (*searchFirstIndex == 0) ? byteArrayModel->size() - 1 : *searchFirstIndex - 1;
                } else {
                    *searchLastIndex = cursorPosition - 1;
                    // no wrap needed when starting at end
                    *searchFirstIndex = (*searchLastIndex == byteArrayModel->size() - 1) ? 0 : *searchLastIndex + 1;
                }
            }
        } else {
            *searchFirstIndex = 0;
            *searchLastIndex = byteArrayModel->size() - 1;
        }
    }

    return true;
}

}

}
