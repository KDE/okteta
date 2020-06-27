/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UNIONDATAINFORMATION_HPP
#define KASTEN_UNIONDATAINFORMATION_HPP

#include "datainformationwithchildren.hpp"

/** A class holding the data of a union for Okteta*/
class UnionDataInformation : public DataInformationWithChildren
{
    DATAINFORMATION_CLONE(UnionDataInformation, DataInformationWithChildren) {}

public:
    explicit UnionDataInformation(const QString& name,
                                  const QVector<DataInformation*>& children = QVector<DataInformation*>(),
                                  DataInformation* parent = nullptr);
    ~UnionDataInformation() override;

    bool isUnion() const override;
    BitCount32 size() const override;
    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

private:
    QString typeNameImpl() const override;
};

#endif /* KASTEN_UNIONDATAINFORMATION_HPP */
