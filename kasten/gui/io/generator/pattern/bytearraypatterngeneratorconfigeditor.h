/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BYTEARRAYPATTERNGENERATORCONFIGEDITOR_H
#define BYTEARRAYPATTERNGENERATORCONFIGEDITOR_H

// lib
#include "bytearraypatterngenerator.h"
// Kasten gui
#include <abstractmodeldatageneratorconfigeditor.h>

namespace Okteta {
class ByteArrayComboBox;
}
class KIntNumInput;


namespace Kasten2
{

class ByteArrayPatternGeneratorConfigEditor : public AbstractModelDataGeneratorConfigEditor
{
  Q_OBJECT

  public:
    explicit ByteArrayPatternGeneratorConfigEditor( ByteArrayPatternGenerator* generator, QWidget* parent = 0 );

    virtual ~ByteArrayPatternGeneratorConfigEditor();

  public: // AbstractModelDataGeneratorConfigEditor API
    virtual bool isValid() const;
    virtual QString name() const;

  public:
    void rememberCurrentSettings();

  protected Q_SLOTS:
    void onSettingsChanged();
    void onPatternChanged( const QByteArray& pattern );

  protected:
    ByteArrayPatternGenerator* mGenerator;
    ByteArrayPatternGeneratorSettings mSettings;

    KIntNumInput* mNumberInput;
    Okteta::ByteArrayComboBox* mPatternEdit;
};

}

#endif
