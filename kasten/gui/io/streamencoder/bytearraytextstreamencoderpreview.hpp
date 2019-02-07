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

#ifndef KASTEN_BYTEARRAYTEXTSTREAMENCODERPREVIEW_HPP
#define KASTEN_BYTEARRAYTEXTSTREAMENCODERPREVIEW_HPP

// lib
#include <kasten/okteta/oktetakastengui_export.hpp>
// Kasten ui
#include <Kasten/AbstractSelectionView>

class QTextEdit;

namespace Kasten {

class AbstractByteArrayStreamEncoder;
class AbstractModelSelection;
class AbstractModel;

class ByteArrayTextStreamEncoderPreview : public AbstractSelectionView
{
    Q_OBJECT

public:
    explicit ByteArrayTextStreamEncoderPreview(AbstractByteArrayStreamEncoder* encoder);
    ~ByteArrayTextStreamEncoderPreview() override;

public: // AbstractSelectionView API
    QWidget* widget() const override;
    void setData(AbstractModel* model, const AbstractModelSelection* selection) override;

public Q_SLOTS:
    void update();

private Q_SLOTS:
    void setFixedFontByGlobalSettings();

private:
    AbstractByteArrayStreamEncoder* const mEncoder;

    AbstractModel* mModel = nullptr;
    const AbstractModelSelection* mSelection = nullptr;

    QTextEdit* mWidget;
};

}

#endif
