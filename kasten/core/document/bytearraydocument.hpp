/*
    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDOCUMENT_HPP
#define KASTEN_BYTEARRAYDOCUMENT_HPP

// lib
#include "oktetakastencore_export.hpp"
// #include "kbytearrayinterface.hpp"
// Kasten core
#include <Kasten/Versionable>
#include <Kasten/AbstractDocument>
// Qt
#include <QString>
// Std
#include <memory>

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
    ByteArrayDocument(std::unique_ptr<Okteta::PieceTableByteArrayModel>&& byteArray, const QString& initDescription);
    ~ByteArrayDocument() override;

public: // AbstractModel API
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    bool isModifiable() const override;
    [[nodiscard]]
    bool isReadOnly() const override;

    void setReadOnly(bool isReadOnly) override;

public: // AbstractDocument API
    [[nodiscard]]
    QString typeName() const override;
    [[nodiscard]]
    QString mimeType() const override;
    [[nodiscard]]
    ContentFlags contentFlags() const override;

public: // If::Versionable
    [[nodiscard]]
    int versionIndex() const override;
    [[nodiscard]]
    DocumentVersionData versionData(int versionIndex) const override;
    [[nodiscard]]
    int versionCount() const override;

    void revertToVersionByIndex(int versionIndex) override;

public: // If::ByteArray
    [[nodiscard]]
    virtual Okteta::AbstractByteArrayModel* content() const;

public:
    void setTitle(const QString& title);

Q_SIGNALS: // If::Versionable
    void revertedToVersionIndex(int versionIndex) override;
    void headVersionDataChanged(const Kasten::DocumentVersionData& versionData) override;
    void headVersionChanged(int newHeadVersionIndex) override;

private Q_SLOTS:
    OKTETAKASTENCORE_NO_EXPORT void onModelModified(bool isModified);
    OKTETAKASTENCORE_NO_EXPORT void onHeadVersionDescriptionChanged(const QString& newDescription);

private:
    const std::unique_ptr<Okteta::PieceTableByteArrayModel> mByteArray;

    mutable QString mTitle;

    const QString mInitDescription;
};

}

#endif
