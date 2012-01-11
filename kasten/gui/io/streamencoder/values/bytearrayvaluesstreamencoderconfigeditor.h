/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYVALUESSTREAMENCODERCONFIGEDITOR_H
#define BYTEARRAYVALUESSTREAMENCODERCONFIGEDITOR_H

// lib
#include "bytearrayvaluesstreamencoder.h"
// Qt
#include <abstractmodelstreamencoderconfigeditor.h>

class KLineEdit;
class KComboBox;


namespace Kasten2
{

class ByteArrayValuesStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
  Q_OBJECT

  public:
    explicit ByteArrayValuesStreamEncoderConfigEditor( ByteArrayValuesStreamEncoder* encoder, QWidget* parent = 0 );
    virtual ~ByteArrayValuesStreamEncoderConfigEditor();

  public: // AbstractModelStreamEncoderConfigEditor API
    virtual AbstractSelectionView* createPreviewView() const;

  protected Q_SLOTS:
    void onSettingsChanged();

  protected:
    ByteArrayValuesStreamEncoder* mEncoder;
    ValuesStreamEncoderSettings mSettings;

//     KComboBox* mValueCodingSelect;
    KLineEdit* mSeparationEdit;
};

}

#endif
