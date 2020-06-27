/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP
#define KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearraychecksumparametersetedit.hpp"

class KComboBox;

class ModSumByteArrayChecksumParameterSetEdit : public AbstractByteArrayChecksumParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit ModSumByteArrayChecksumParameterSetEdit(QWidget* parent = nullptr);
    ~ModSumByteArrayChecksumParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const override;
    bool isValid() const override;

private:
    KComboBox* mByteOrderComboBox;
};

#endif
