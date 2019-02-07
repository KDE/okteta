/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYDOCUMENT_HPP
#define BYTEARRAYDOCUMENT_HPP

// lib
#include <kasten/okteta/oktetakastencore_export.hpp>
// #include "kbytearrayinterface.hpp"
// Kasten core
#include <Kasten/Versionable>
#include <Kasten/AbstractDocument>
// Qt
#include <QString>

namespace Okteta {
class AbstractByteArrayModel;
class PieceTableByteArrayModel;
}

namespace Kasten {

class OKTETAKASTENCORE_EXPORT ByteArrayDocument : public AbstractDocument
                                                , public If::Versionable
                                                // , public If::UserListable
                                                // ,public If::ByteArray
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::Versionable
//         Kasten::If::UserListable
//         Kasten::If::ByteArray
    )

public:
    explicit ByteArrayDocument(const QString& initDescription);
    ByteArrayDocument(Okteta::PieceTableByteArrayModel* byteArray, const QString& initDescription);
    ~ByteArrayDocument() override;

public: // AbstractModel API
    QString title() const override;
    bool isModifiable() const override;
    bool isReadOnly() const override;
    void setReadOnly(bool isReadOnly) override;

public: // AbstractDocument API
    QString typeName() const override;
    QString mimeType() const override;
    ContentFlags contentFlags() const override;

public: // If::Versionable
    int versionIndex() const override;
    DocumentVersionData versionData(int versionIndex) const override;
    int versionCount() const override;
    void revertToVersionByIndex(int versionIndex) override;

public: // If::ByteArray
    virtual Okteta::AbstractByteArrayModel* content() const;

public:
    void setTitle(const QString& title);

Q_SIGNALS: // If::Versionable
    void revertedToVersionIndex(int versionIndex) override;
    void headVersionDataChanged(const Kasten::DocumentVersionData& versionData) override;
    void headVersionChanged(int newHeadVersionIndex) override;

private Q_SLOTS:
    void onModelModified(bool newState);
    void onHeadVersionDescriptionChanged(const QString& newDescription);

private:
    Okteta::PieceTableByteArrayModel* mByteArray;

    mutable QString mTitle;

    const QString mInitDescription;
};

}

#endif
