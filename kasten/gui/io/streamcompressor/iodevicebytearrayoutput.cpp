/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "iodevicebytearrayoutput.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QIODevice>
#include <QString>

namespace Kasten {

QString IoDeviceByteArrayOutput::errorString() const
{
    return i18n("Error writing to underlying device: %1", m_ioDevice->errorString());
}

bool IoDeviceByteArrayOutput::writeData(const char* data, std::size_t dataSize)
{
    const qint64 writtenSize = m_ioDevice->write(data, dataSize);

    return (writtenSize == static_cast<qint64>(dataSize));
}

}
