/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef BYTEARRAYTEXTSTREAMENCODERPREVIEW_H
#define BYTEARRAYTEXTSTREAMENCODERPREVIEW_H

// lib
#include "oktetakastengui_export.h"
// Kasten ui
#include <abstractselectionview.h>

class KTextEdit;


namespace Kasten2
{

class AbstractByteArrayStreamEncoder;
class AbstractModelSelection;
class AbstractModel;


class ByteArrayTextStreamEncoderPreview : public AbstractSelectionView
{
  Q_OBJECT

  public:
    explicit ByteArrayTextStreamEncoderPreview( AbstractByteArrayStreamEncoder* encoder );
    virtual ~ByteArrayTextStreamEncoderPreview();

  public: // AbstractSelectionView API
    virtual QWidget* widget() const;
    virtual void setData( AbstractModel* model, const AbstractModelSelection* selection );

  public Q_SLOTS:
    void update();

  private Q_SLOTS:
    void setFixedFontByGlobalSettings();

  private:
    AbstractByteArrayStreamEncoder* const mEncoder;

    AbstractModel* mModel;
    const AbstractModelSelection* mSelection;

    KTextEdit* mWidget;
};

}

#endif
