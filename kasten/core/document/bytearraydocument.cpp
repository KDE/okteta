/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydocument.hpp"

// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// KF
#include <KLocalizedString>

using Okteta::PieceTableByteArrayModel;

namespace Kasten {

ByteArrayDocument::ByteArrayDocument(const QString& initDescription)
    : mByteArray(new Okteta::PieceTableByteArrayModel())
    , mInitDescription(initDescription)
{
    connect(mByteArray, &PieceTableByteArrayModel::modifiedChanged, this, &ByteArrayDocument::onModelModified);
    connect(mByteArray, &PieceTableByteArrayModel::readOnlyChanged, this, &ByteArrayDocument::readOnlyChanged);
    connect(mByteArray, &PieceTableByteArrayModel::revertedToVersionIndex, this, &ByteArrayDocument::revertedToVersionIndex);
    connect(mByteArray, &PieceTableByteArrayModel::headVersionChanged, this, &ByteArrayDocument::headVersionChanged);
    connect(mByteArray, &PieceTableByteArrayModel::headVersionDescriptionChanged,
            this, &ByteArrayDocument::onHeadVersionDescriptionChanged);
}

ByteArrayDocument::~ByteArrayDocument()
{
    delete mByteArray;
}

ByteArrayDocument::ByteArrayDocument(Okteta::PieceTableByteArrayModel* byteArray, const QString& initDescription)
    : mByteArray(byteArray)
    , mInitDescription(initDescription)
{
    connect(mByteArray, &PieceTableByteArrayModel::modifiedChanged, this, &ByteArrayDocument::onModelModified);
    connect(mByteArray, &PieceTableByteArrayModel::readOnlyChanged, this, &ByteArrayDocument::readOnlyChanged);
    connect(mByteArray, &PieceTableByteArrayModel::revertedToVersionIndex, this, &ByteArrayDocument::revertedToVersionIndex);
    connect(mByteArray, &PieceTableByteArrayModel::headVersionChanged, this, &ByteArrayDocument::headVersionChanged);
    connect(mByteArray, &PieceTableByteArrayModel::headVersionDescriptionChanged,
            this, &ByteArrayDocument::onHeadVersionDescriptionChanged);
}

Okteta::AbstractByteArrayModel* ByteArrayDocument::content() const { return mByteArray; }

QString ByteArrayDocument::title() const { return mTitle; }
QString ByteArrayDocument::mimeType() const { return QStringLiteral("ByteArrayDocument"); }
QString ByteArrayDocument::typeName() const { return i18nc("name of the data type", "Byte Array"); }

bool ByteArrayDocument::isModifiable() const { return true; }
bool ByteArrayDocument::isReadOnly()   const { return mByteArray->isReadOnly(); }
void ByteArrayDocument::setReadOnly(bool isReadOnly) { mByteArray->setReadOnly(isReadOnly); }

ContentFlags ByteArrayDocument::contentFlags() const
{
    return (mByteArray->isModified() ? ContentHasUnstoredChanges : ContentStateNormal);
}

void ByteArrayDocument::setTitle(const QString& title)
{
    mTitle = title;
    emit titleChanged(mTitle);
}

int ByteArrayDocument::versionIndex() const { return mByteArray->versionIndex(); }
int ByteArrayDocument::versionCount() const { return mByteArray->versionCount(); }
DocumentVersionData ByteArrayDocument::versionData(int versionIndex) const
{
    const QString changeComment = (versionIndex == 0) ? mInitDescription : mByteArray->versionDescription(versionIndex);
    return DocumentVersionData(versionIndex, changeComment);
}

void ByteArrayDocument::revertToVersionByIndex(int versionIndex) { mByteArray->revertToVersionByIndex(versionIndex); }

void ByteArrayDocument::onModelModified(bool isModified)
{
    emit contentFlagsChanged((isModified ? ContentHasUnstoredChanges : ContentStateNormal));
}

void ByteArrayDocument::onHeadVersionDescriptionChanged(const QString& newDescription)
{
    const DocumentVersionData data(mByteArray->versionIndex(), newDescription);
    emit headVersionDataChanged(data);
}

}
