/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearrayfilterparametersetedit.hpp"

class QCheckBox;

class ReverseByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit ReverseByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);
    ~ReverseByteArrayFilterParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const override;

private:
    QCheckBox* mInvertsBitsCheckBox;
};

#endif
