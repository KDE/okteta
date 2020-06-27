/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ROTATEBYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_ROTATEBYTEARRAYFILTERPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearrayfilterparametersetedit.hpp"

class QSpinBox;

class RotateByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit RotateByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);
    ~RotateByteArrayFilterParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const override;
    bool isValid() const override;

private Q_SLOTS:
    void onValueChanged(int value);

private:
    QSpinBox* mGroupSizeEdit;
    QSpinBox* mMoveBitWidthEdit;
};

#endif
