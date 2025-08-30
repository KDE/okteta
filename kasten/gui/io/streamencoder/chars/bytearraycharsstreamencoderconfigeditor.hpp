/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHARSSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYCHARSSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearraycharsstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QLabel;

namespace Kasten {

class ByteArrayCharsStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayCharsStreamEncoderConfigEditor(ByteArrayCharsStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayCharsStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    [[nodiscard]]
    std::unique_ptr<AbstractSelectionView> createPreviewView() const override;

private Q_SLOTS:
    void setCodingSelectEnabled(int encodeModeIndex);
    void onSettingsChanged();

private:
    ByteArrayCharsStreamEncoder* const m_encoder;

    CharsStreamEncoderSettings m_settings;

    KComboBox* m_encodeModeSelect;
    QLabel* m_escapedValueCodingLabel;
    KComboBox* m_escapedValueCodingSelect;
};

}

#endif
