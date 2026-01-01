/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREMETADATA_HPP
#define KASTEN_STRUCTUREMETADATA_HPP

// Qt
#include <QExplicitlySharedDataPointer>
#include <QMetaType>

class KAboutLicense;

class QString;

namespace Kasten {

class StructureMetaDataPrivate;

class StructureMetaData
{
public:
    StructureMetaData();
    explicit StructureMetaData(const QString& filename);
    StructureMetaData(const StructureMetaData& rhs);
    StructureMetaData(StructureMetaData&& rhs);

    ~StructureMetaData();

    StructureMetaData& operator=(const StructureMetaData& rhs);
    StructureMetaData& operator=(StructureMetaData&& rhs);

public:
    [[nodiscard]]
    bool isValid() const;

    [[nodiscard]]
    QString id() const;
    [[nodiscard]]
    QString name() const;
    [[nodiscard]]
    QString comment() const;
    [[nodiscard]]
    QString categoryId() const;
    [[nodiscard]]
    QString version() const;
    [[nodiscard]]
    bool hasAuthorName() const;
    [[nodiscard]]
    QString authorName() const;
    [[nodiscard]]
    bool hasAuthorEmailAddress() const;
    [[nodiscard]]
    QString authorEmailAddress() const;
    [[nodiscard]]
    bool hasIconName() const;
    [[nodiscard]]
    QString iconName() const;
    [[nodiscard]]
    bool hasWebsite() const;
    [[nodiscard]]
    QString website() const;
    [[nodiscard]]
    QString entryPath() const;

    [[nodiscard]]
    const KAboutLicense& license() const;

private:
    QExplicitlySharedDataPointer<StructureMetaDataPrivate> d;
};

}

Q_DECLARE_TYPEINFO(Kasten::StructureMetaData, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Kasten::StructureMetaData)

#endif
