/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytetabletool.hpp"

// controller
#include "bytetablemodel.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Okteta core
#include <Okteta/Character>
#include <Okteta/CharCodec>
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ChangesDescribable>
// KF
#include <KLocalizedString>

namespace Kasten {

ByteTableTool::ByteTableTool()
    : mByteTableModel(new ByteTableModel(this))
{
    setObjectName(QStringLiteral("ByteTable"));
}

ByteTableTool::~ByteTableTool() = default;

QString ByteTableTool::title() const { return i18nc("@title:window", "Value/Char Table"); }
ByteTableModel* ByteTableTool::byteTableModel() const { return mByteTableModel; }
bool ByteTableTool::hasWriteable() const
{
    return (mByteArrayView && mByteArrayModel) ? !mByteArrayView->isReadOnly() : false;
}

void ByteTableTool::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(mByteTableModel);
        mByteArrayView->disconnect(this);
    }

    mByteArrayView = model ? qobject_cast<ByteArrayView*>(model) : nullptr;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>(mByteArrayView->baseModel()) : nullptr;
    mByteArrayModel = document ? document->content() : nullptr;

    const bool hasView = (mByteArrayView && mByteArrayModel);
    if (hasView) {
        mByteTableModel->setCharCodec(mByteArrayView->charCodingName());
        mByteTableModel->setSubstituteChar(mByteArrayView->substituteChar());
        mByteTableModel->setUndefinedChar(mByteArrayView->undefinedChar());
        connect(mByteArrayView,  &ByteArrayView::charCodecChanged,
                mByteTableModel, &ByteTableModel::setCharCodec);
        connect(mByteArrayView, &ByteArrayView::substituteCharChanged,
                mByteTableModel, &ByteTableModel::setSubstituteChar);
        connect(mByteArrayView, &ByteArrayView::undefinedCharChanged,
                mByteTableModel, &ByteTableModel::setUndefinedChar);

        connect(mByteArrayView, &ByteArrayView::readOnlyChanged,
                this, &ByteTableTool::onReadOnlyChanged);
    } else {
        // TODO: use default view profile chars and char codec
        mByteTableModel->setSubstituteChar(QChar());
        mByteTableModel->setUndefinedChar(QChar());
    }

    const bool isWriteable = (hasView && !mByteArrayView->isReadOnly());

    emit hasWriteableChanged(isWriteable);
}

void ByteTableTool::insert(unsigned char byte, int count)
{
    const QByteArray data(count, byte);

    Okteta::ChangesDescribable* changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

    if (changesDescribable) {
        // TODO: how to note the byte? charcoding might change...
        const QString changeDescription =
            i18np("Inserted 1 Byte", "Inserted %1 Bytes", count);

        changesDescribable->openGroupedChange(changeDescription);
    }

    mByteArrayView->insert(data);

    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
// void ByteTableController::fill( const QByteArray &Data )
// {
//     if( HexEdit && ByteArray )
//         ByteArray->insert( HexEdit->cursorPosition(), Data );
// }
    mByteArrayView->setFocus();
}

void ByteTableTool::onReadOnlyChanged(bool isReadOnly)
{
    const bool isWriteable = !isReadOnly;

    emit hasWriteableChanged(isWriteable);
}

}
