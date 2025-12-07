/*
    SPDX-FileCopyrightText: 2009, 2011, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychecksumalgorithmfactory.hpp"

// lib
#include "algorithm/adler32bytearraychecksumalgorithm.hpp"
#include "algorithm/crc32bytearraychecksumalgorithm.hpp"
#include "algorithm/crc64bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum8bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum16bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum32bytearraychecksumalgorithm.hpp"
#include "algorithm/modsum64bytearraychecksumalgorithm.hpp"
#include "algorithm/qcryptographicbytearraychecksumalgorithm.hpp"
// NEWCHECKSUM(start)
// Here add the name of your header file of your checksum algorithm,
// e.g.
// #include "algorithm/mybytearraychecksumalgorithm.hpp"
// NEWCHECKSUM(end)
// KF
#include <KLocalizedString>

template<typename T, typename ... Ptrs>
auto make_unique_vector(Ptrs&& ... ptrs)
{
    std::vector<std::unique_ptr<T>> vector;
    vector.reserve(sizeof...(Ptrs));
    ( vector.emplace_back(std::unique_ptr<T>(ptrs)), ... );
    return vector;
}

namespace {
inline
void addQCryptographicHash(std::vector<std::unique_ptr<AbstractByteArrayChecksumAlgorithm>>& algorithmList,
                           const QString& name, const QString& id, QCryptographicHash::Algorithm algorithm)
{
    if (QCryptographicHash::supportsAlgorithm(algorithm)) {
        algorithmList.emplace_back(std::unique_ptr<AbstractByteArrayChecksumAlgorithm>(new QCryptographicByteArrayChecksumAlgorithm(name, id, algorithm)));
    }
}
}

std::vector<std::unique_ptr<AbstractByteArrayChecksumAlgorithm>> ByteArrayChecksumAlgorithmFactory::createAlgorithms()
{
    std::vector<std::unique_ptr<AbstractByteArrayChecksumAlgorithm>> result = make_unique_vector<AbstractByteArrayChecksumAlgorithm>(
        new ModSum8ByteArrayChecksumAlgorithm(),
        new ModSum16ByteArrayChecksumAlgorithm(),
        new ModSum32ByteArrayChecksumAlgorithm(),
        new ModSum64ByteArrayChecksumAlgorithm(),
        new Adler32ByteArrayChecksumAlgorithm(),
        new Crc32ByteArrayChecksumAlgorithm(),
        new Crc64ByteArrayChecksumAlgorithm()
// NEWCHECKSUM(start)
// Here add the creation of an object of your checksum algorithm class and add it to the list,
// e.g.
//         new MyByteArrayChecksumAlgorithm(),
// NEWCHECKSUM(end)
    );

    // TODO: MD2? was provided by QCA
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "MD4"),
                          QStringLiteral("MD4"),  QCryptographicHash::Md4);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "MD5"),
                          QStringLiteral("MD5"),  QCryptographicHash::Md5);
    // TODO: SHA0? was provided by QCA
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA-1"),
                          QStringLiteral("SHA-1"), QCryptographicHash::Sha1);
    // TODO: RIPEMD160? was provided by QCA
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA-224"),
                          QStringLiteral("SHA-224"),  QCryptographicHash::Sha224);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA-256"),
                          QStringLiteral("SHA-256"),  QCryptographicHash::Sha256);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA-384"),
                          QStringLiteral("SHA-384"),  QCryptographicHash::Sha384);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA-512"),
                          QStringLiteral("SHA-512"),  QCryptographicHash::Sha512);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA3-224"),
                          QStringLiteral("SHA3-224"),  QCryptographicHash::Sha3_224);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA3-256"),
                          QStringLiteral("SHA3-256"),  QCryptographicHash::Sha3_256);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA3-384"),
                          QStringLiteral("SHA3-384"),  QCryptographicHash::Sha3_384);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "SHA3-512"),
                          QStringLiteral("SHA3-512"),  QCryptographicHash::Sha3_512);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "Keccak-224"),
                          QStringLiteral("Keccak-224"),  QCryptographicHash::Keccak_224);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "Keccak-256"),
                          QStringLiteral("Keccak-256"),  QCryptographicHash::Keccak_256);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "Keccak-384"),
                          QStringLiteral("Keccak-384"),  QCryptographicHash::Keccak_384);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "Keccak-512"),
                          QStringLiteral("Keccak-512"),  QCryptographicHash::Keccak_512);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2b-160"),
                          QStringLiteral("BLAKE2b-160"),  QCryptographicHash::Blake2b_160);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2b-256"),
                          QStringLiteral("BLAKE2b-256"),  QCryptographicHash::Blake2b_256);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2b-384"),
                          QStringLiteral("BLAKE2b-384"),  QCryptographicHash::Blake2b_384);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2b-512"),
                          QStringLiteral("BLAKE2b-512"),  QCryptographicHash::Blake2b_512);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2s-128"),
                          QStringLiteral("BLAKE2s-128"),  QCryptographicHash::Blake2s_128);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2s-160"),
                          QStringLiteral("BLAKE2s-160"),  QCryptographicHash::Blake2s_160);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2s-224"),
                          QStringLiteral("BLAKE2s-224"),  QCryptographicHash::Blake2s_224);
    addQCryptographicHash(result, i18nc("name of the hash algorithm", "BLAKE2s-256"),
                          QStringLiteral("BLAKE2s-256"),  QCryptographicHash::Blake2s_256);
    // TODO: Whirlpool? was provided by QCA

    return result;
}
