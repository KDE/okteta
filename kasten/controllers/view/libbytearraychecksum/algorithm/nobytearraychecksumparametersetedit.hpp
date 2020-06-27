/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_NOBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP
#define KASTEN_NOBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearraychecksumparametersetedit.hpp"

class NoByteArrayChecksumParameterSetEdit : public AbstractByteArrayChecksumParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit NoByteArrayChecksumParameterSetEdit(QWidget* parent = nullptr);
    ~NoByteArrayChecksumParameterSetEdit() override;

public: // AbstractByteArrayChecksumParameterSetEdit API
    void setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const override;
};

#endif
