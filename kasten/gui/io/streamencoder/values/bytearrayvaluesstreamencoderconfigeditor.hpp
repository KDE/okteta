/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVALUESSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYVALUESSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearrayvaluesstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QLineEdit;

namespace Kasten {

class ByteArrayValuesStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayValuesStreamEncoderConfigEditor(ByteArrayValuesStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayValuesStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayValuesStreamEncoder* mEncoder;
    ValuesStreamEncoderSettings mSettings;

//     KComboBox* mValueCodingSelect;
    QLineEdit* mSeparationEdit;
};

}

#endif
