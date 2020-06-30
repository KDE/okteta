/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_NOBYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_NOBYTEARRAYFILTERPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearrayfilterparametersetedit.hpp"

class NoByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit NoByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);
    ~NoByteArrayFilterParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const override;

};

#endif
