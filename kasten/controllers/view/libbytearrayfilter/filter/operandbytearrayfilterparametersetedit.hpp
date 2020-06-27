/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OPERANDBYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_OPERANDBYTEARRAYFILTERPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearrayfilterparametersetedit.hpp"

namespace Okteta {
class ByteArrayComboBox;
}
class QCheckBox;

class OperandByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit OperandByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);
    ~OperandByteArrayFilterParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) override;
    void setCharCodec(const QString& charCodecName) override;
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const override;
    bool isValid() const override;
    void rememberCurrentSettings() override;

private Q_SLOTS:
    void onInputChanged(const QByteArray& data);

private:
    Okteta::ByteArrayComboBox* mOperandEdit;
    QCheckBox* mAlignAtEndCheckBox;
};

#endif
