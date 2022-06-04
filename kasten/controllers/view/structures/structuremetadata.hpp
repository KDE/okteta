/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTUREMETADATA_HPP
#define STRUCTUREMETADATA_HPP

// Qt
#include <QExplicitlySharedDataPointer>
#include <QMetaType>

class KAboutLicense;

class QString;

class StructureMetaDataPrivsate;

class StructureMetaData
{
public:
    StructureMetaData();
    explicit StructureMetaData(const QString& filename);
    StructureMetaData(const StructureMetaData& rhs);

    ~StructureMetaData();

    StructureMetaData& operator=(const StructureMetaData& rhs);

public:
    bool isValid() const;

    QString id() const;
    QString name() const;
    QString comment() const;
    QString categoryId() const;
    QString version() const;
    bool hasAuthorName() const;
    QString authorName() const;
    bool hasAuthorEmailAddress() const;
    QString authorEmailAddress() const;
    bool hasIconName() const;
    QString iconName() const;
    bool hasWebsite() const;
    QString website() const;
    QString entryPath() const;

    const KAboutLicense& license() const;

private:
    QExplicitlySharedDataPointer<StructureMetaDataPrivsate> d;
};

Q_DECLARE_TYPEINFO(StructureMetaData, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(StructureMetaData)

#endif
