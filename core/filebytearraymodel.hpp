/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FILEBYTEARRAYMODEL_HPP
#define OKTETA_FILEBYTEARRAYMODEL_HPP

// lib
#include "abstractbytearraymodel.hpp"

namespace Okteta {

class FileByteArrayModelPrivate;

/**
 * @author Friedrich W. H. Kossebau
 */

class OKTETACORE_EXPORT FileByteArrayModel : public AbstractByteArrayModel
{
    friend class FileByteArrayModelPrivate;

    Q_OBJECT

public:
    /** default is only 50*4k = 200k memory image */
    explicit FileByteArrayModel(int pageNumber = 50, int pageSize = 4096, QObject* parent = nullptr);
    ~FileByteArrayModel() override;

public: // AbstractByteArrayModel API
    Byte byte(Address offset) const override;
    Size size() const override;
    bool isReadOnly() const override;
    bool isModified() const override;

    Size insert(Address offset, const Byte* insertData, int insertLength) override;
    Size remove(const AddressRange& removeRange) override;
    Size replace(const AddressRange& removeRange, const Byte* insertData, int insertLength) override;
    bool swap(Address firstStart, const AddressRange& secondRange) override;
    Size fill(Byte fillByte, Address offset = 0, Size fillLength = -1) override;
    void setByte(Address offset, Byte byte) override;

    void setModified(bool modified = true) override;

public:
    void setReadOnly(bool readOnly = true) override;
    bool isOpen() const;
    bool open(const QString& filename);
    bool close();

private:
    Q_DECLARE_PRIVATE(FileByteArrayModel)
};

}

#endif
