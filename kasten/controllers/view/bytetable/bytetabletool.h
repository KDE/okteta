/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTETABLETOOL_H
#define BYTETABLETOOL_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten core
#include <abstracttool.h>

namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten2
{

class ByteTableModel;
class ByteArrayView;

/**
*/
class OKTETAKASTENCONTROLLERS_EXPORT ByteTableTool : public AbstractTool
{
  Q_OBJECT

  public:
    ByteTableTool();
    virtual ~ByteTableTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public:
    void insert( unsigned char byte, int count );

  public:
    ByteTableModel *byteTableModel() const;
    bool hasWriteable() const;

  Q_SIGNALS:
    void hasWriteableChanged( bool hasWriteable );

  private Q_SLOTS:
    void onReadOnlyChanged( bool isReadOnly );

  private:
    ByteTableModel *mByteTableModel;

    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel *mByteArrayModel;
};

}

#endif
