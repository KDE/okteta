/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITOR_HPP

// lib
#include "bytearrayrandomdatagenerator.hpp"
// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditor>

class QSpinBox;

namespace Kasten {

class ByteArrayRandomDataGeneratorConfigEditor : public AbstractModelDataGeneratorConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayRandomDataGeneratorConfigEditor(ByteArrayRandomDataGenerator* generator, QWidget* parent = nullptr);

    ~ByteArrayRandomDataGeneratorConfigEditor() override;

public: // AbstractModelDataGeneratorConfigEditor API
    QString name() const override;

public:
    void rememberCurrentSettings();

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayRandomDataGenerator* mGenerator;
    ByteArrayRandomDataGeneratorSettings mSettings;

    QSpinBox* mSizeInput;
};

}

#endif
