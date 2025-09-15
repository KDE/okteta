/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IODEVICEBYTEARRAYOUTPUT_HPP
#define KASTEN_IODEVICEBYTEARRAYOUTPUT_HPP

// lib
#include "abstractbytearrayoutput.hpp"

class QIODevice;

namespace Kasten {

class IoDeviceByteArrayOutput : public AbstractByteArrayOutput
{
public:
    explicit IoDeviceByteArrayOutput(QIODevice* ioDevice);
    ~IoDeviceByteArrayOutput() override;

public: // AbstractByteArrayOutput API
    [[nodiscard]]
    QString errorString() const override;

    /// returns @false on error
    [[nodiscard]]
    bool writeData(const char* data, std::size_t dataSize) override;

private:
    QIODevice* const m_ioDevice;
};

inline IoDeviceByteArrayOutput::IoDeviceByteArrayOutput(QIODevice* ioDevice)
    : m_ioDevice(ioDevice)
{
}

inline IoDeviceByteArrayOutput::~IoDeviceByteArrayOutput() = default;

}

#endif
