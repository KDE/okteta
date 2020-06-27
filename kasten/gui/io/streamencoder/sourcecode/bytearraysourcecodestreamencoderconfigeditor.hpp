/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSOURCECODESTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYSOURCECODESTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearraysourcecodestreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QSpinBox;
class QCheckBox;
class QLineEdit;

namespace Kasten {

class ByteArraySourceCodeStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArraySourceCodeStreamEncoderConfigEditor(ByteArraySourceCodeStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArraySourceCodeStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    bool isValid() const override;
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArraySourceCodeStreamEncoder* mEncoder;
    SourceCodeStreamEncoderSettings mSettings;

    QLineEdit* mVariableNameEdit;
    QSpinBox* mItemsPerLineEdit;
    KComboBox* mDataTypeSelect;
    QCheckBox* mUnsignedAsHexadecimalCheck;
};

}

#endif
