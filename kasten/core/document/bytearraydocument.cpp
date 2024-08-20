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
    : mByteArray(std::make_unique<Okteta::PieceTableByteArrayModel>())
    , mInitDescription(initDescription)
{
    connect(mByteArray.get(), &PieceTableByteArrayModel::modifiedChanged, this, &ByteArrayDocument::onModelModified);
    connect(mByteArray.get(), &PieceTableByteArrayModel::readOnlyChanged, this, &ByteArrayDocument::readOnlyChanged);
    connect(mByteArray.get(), &PieceTableByteArrayModel::revertedToVersionIndex, this, &ByteArrayDocument::revertedToVersionIndex);
    connect(mByteArray.get(), &PieceTableByteArrayModel::headVersionChanged, this, &ByteArrayDocument::headVersionChanged);
    connect(mByteArray.get(), &PieceTableByteArrayModel::headVersionDescriptionChanged,
            this, &ByteArrayDocument::onHeadVersionDescriptionChanged);
}

ByteArrayDocument::~ByteArrayDocument() = default;

ByteArrayDocument::ByteArrayDocument(Okteta::PieceTableByteArrayModel* byteArray, const QString& initDescription)
    : mByteArray(byteArray)
    , mInitDescription(initDescription)
{
    connect(mByteArray.get(), &PieceTableByteArrayModel::modifiedChanged, this, &ByteArrayDocument::onModelModified);
    connect(mByteArray.get(), &PieceTableByteArrayModel::readOnlyChanged, this, &ByteArrayDocument::readOnlyChanged);
    connect(mByteArray.get(), &PieceTableByteArrayModel::revertedToVersionIndex, this, &ByteArrayDocument::revertedToVersionIndex);
    connect(mByteArray.get(), &PieceTableByteArrayModel::headVersionChanged, this, &ByteArrayDocument::headVersionChanged);
    connect(mByteArray.get(), &PieceTableByteArrayModel::headVersionDescriptionChanged,
            this, &ByteArrayDocument::onHeadVersionDescriptionChanged);
}

Okteta::AbstractByteArrayModel* ByteArrayDocument::content() const { return mByteArray.get(); }

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
    Q_EMIT titleChanged(mTitle);
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
    Q_EMIT contentFlagsChanged((isModified ? ContentHasUnstoredChanges : ContentStateNormal));
}

void ByteArrayDocument::onHeadVersionDescriptionChanged(const QString& newDescription)
{
    const DocumentVersionData data(mByteArray->versionIndex(), newDescription);
    Q_EMIT headVersionDataChanged(data);
}

}

#include "moc_bytearraydocument.cpp"
