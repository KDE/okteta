/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYPATTERNGENERATORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYPATTERNGENERATORCONFIGEDITOR_HPP

// lib
#include "bytearraypatterngenerator.hpp"
// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditor>

namespace Okteta {
class ByteArrayComboBox;
}
class QSpinBox;

namespace Kasten {

class ByteArrayPatternGeneratorConfigEditor : public AbstractModelDataGeneratorConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayPatternGeneratorConfigEditor(ByteArrayPatternGenerator* generator, QWidget* parent = nullptr);

    ~ByteArrayPatternGeneratorConfigEditor() override;

public: // AbstractModelDataGeneratorConfigEditor API
    bool isValid() const override;
    QString name() const override;

public:
    void rememberCurrentSettings();

private Q_SLOTS:
    void onSettingsChanged();
    void onPatternChanged(const QByteArray& pattern);

private:
    ByteArrayPatternGenerator* mGenerator;
    ByteArrayPatternGeneratorSettings mSettings;

    QSpinBox* mNumberInput;
    Okteta::ByteArrayComboBox* mPatternEdit;
};

}

#endif
