/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

// QCA
// need to have this first, as QCA needs QT_NO_CAST_FROM_ASCII disabled when included
#include <config-qca2.hpp> // krazy:excludeall=includes
#ifdef HAVE_QCA2
// disable QT_NO_CAST_FROM_ASCII
#ifdef QT_NO_CAST_FROM_ASCII
#undef QT_NO_CAST_FROM_ASCII
#endif
#include <QtCrypto>
#endif

#include "bytearraychecksumalgorithmfactory.hpp"

// lib
#include "algorithm/adler32bytearraychecksumalgorithm.hpp"
#include "algorithm/crc32bytearraychecksumalgorithm.hpp"
#include "algorithm/crc64bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum8bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum16bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum32bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum64bytearraychecksumalgorithm.hpp"
#ifdef HAVE_QCA2
#include "algorithm/qca2bytearraychecksumalgorithm.hpp"
#endif
// NEWCHECKSUM(start)
// Here add the name of your header file of your checksum algorithm,
// e.g.
// #include "algorithm/mybytearraychecksumalgorithm.hpp"
// NEWCHECKSUM(end)
// KF
#include <KLocalizedString>
// Qt
#include <QVector>

#ifdef HAVE_QCA2
static inline void addQca2Algorithm(QVector<AbstractByteArrayChecksumAlgorithm*>& algorithmList, const QString& name, const char* type)
{
    if (QCA::isSupported(type)) {
        algorithmList << new Qca2ByteArrayChecksumAlgorithm(name, QString::fromLatin1(type));
    }
}
#endif

QVector<AbstractByteArrayChecksumAlgorithm*> ByteArrayChecksumAlgorithmFactory::createAlgorithms()
{
    QVector<AbstractByteArrayChecksumAlgorithm*> result {
        new ModSum8ByteArrayChecksumAlgorithm(),
        new ModSum16ByteArrayChecksumAlgorithm(),
        new ModSum32ByteArrayChecksumAlgorithm(),
        new ModSum64ByteArrayChecksumAlgorithm(),
        new Adler32ByteArrayChecksumAlgorithm(),
        new Crc32ByteArrayChecksumAlgorithm(),
        new Crc64ByteArrayChecksumAlgorithm(),
// NEWCHECKSUM(start)
// Here add the creation of an object of your checksum algorithm class and add it to the list,
// e.g.
//         new MyByteArrayChecksumAlgorithm(),
// NEWCHECKSUM(end)
    };

#ifdef HAVE_QCA2
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-0"),     "sha0");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-1"),     "sha1");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "MD2"),       "md2");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "MD4"),       "md4");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "MD5"),       "md5");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "RIPEMD160"), "ripemd160");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-224"),   "sha224");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-256"),   "sha256");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-384"),   "sha384");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "SHA-512"),   "sha512");
    addQca2Algorithm(result, i18nc("name of the hash algorithm", "Whirlpool"), "whirlpool");
#endif

    return result;
}
