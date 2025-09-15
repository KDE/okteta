/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_QCRYPTOGRAPHICBYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_QCRYPTOGRAPHICBYTEARRAYCHECKSUMALGORITHM_HPP

//
#include "nobytearraychecksumparameterset.hpp"
//
#include <abstractbytearraychecksumalgorithm.hpp>
// Qt
#include <QCryptographicHash>

class QCryptographicByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
{
    Q_OBJECT

public:
    QCryptographicByteArrayChecksumAlgorithm(const QString& name, const QString& id,
                                             QCryptographicHash::Algorithm algorithm);
    ~QCryptographicByteArrayChecksumAlgorithm() override;

public: // AbstractByteArrayChecksumAlgorithm API
    [[nodiscard]]
    bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    [[nodiscard]]
    AbstractByteArrayChecksumParameterSet* parameterSet() override;

private:
    NoByteArrayChecksumParameterSet mParameterSet;
    const QCryptographicHash::Algorithm m_algorithm;
};

#endif
