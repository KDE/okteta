/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXXENCODINGSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYXXENCODINGSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearrayxxencodingstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QLineEdit;

namespace Kasten {

class ByteArrayXxencodingStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayXxencodingStreamEncoderConfigEditor(ByteArrayXxencodingStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayXxencodingStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayXxencodingStreamEncoder* mEncoder;
    XxencodingStreamEncoderSettings mSettings;

    QLineEdit* mFileNameEdit;
};

}

#endif
