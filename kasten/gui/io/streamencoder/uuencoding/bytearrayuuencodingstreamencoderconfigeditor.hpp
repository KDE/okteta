/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYUUENCODINGSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYUUENCODINGSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearrayuuencodingstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QLineEdit;

namespace Kasten {

class ByteArrayUuencodingStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayUuencodingStreamEncoderConfigEditor(ByteArrayUuencodingStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayUuencodingStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayUuencodingStreamEncoder* mEncoder;
    UuencodingStreamEncoderSettings mSettings;

    KComboBox* mEncodingSelect;
    QLineEdit* mFileNameEdit;
};

}

#endif
