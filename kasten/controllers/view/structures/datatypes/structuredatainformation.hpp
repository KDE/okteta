/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREDATAINFORMATION_HPP
#define KASTEN_STRUCTUREDATAINFORMATION_HPP

#include "datainformationwithchildren.hpp"

/** A class holding the data of a struct for Okteta*/
class StructureDataInformation : public DataInformationWithChildren
{
    DATAINFORMATION_CLONE(StructureDataInformation, DataInformationWithChildren) {}

public:
    explicit StructureDataInformation(const QString& name,
                                      const QVector<DataInformation*>& children = QVector<DataInformation*>(),
                                      DataInformation* parent = nullptr);
    ~StructureDataInformation() override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    bool isStruct() const override;

    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    static bool readChildren(const QVector<DataInformation*>& children, Okteta::AbstractByteArrayModel* input,
                             Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset, qint64* readBitsPtr,
                             TopLevelDataInformation* top);

private:
    QString typeNameImpl() const override;
};

#endif /* KASTEN_STRUCTUREDATAINFORMATION_HPP */
