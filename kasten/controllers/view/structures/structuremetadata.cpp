/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuremetadata.hpp"

// tool
#include <structureslogging.hpp>
// KF
#include <KDesktopFile>
#include <KConfigGroup>
#include <KAboutData>
// Qt
#include <QSharedData>

class StructureMetaDataPrivsate : public QSharedData
{
public:
    StructureMetaDataPrivsate();

public:
    QString id;
    QString name;
    QString comment;
    QString categoryId;
    QString version;
    QString authorName;
    QString authorEmailAddress;
    QString iconName;
    QString website;
    QString entryPath;

    KAboutLicense license;
};

StructureMetaDataPrivsate::StructureMetaDataPrivsate() = default;


StructureMetaData::StructureMetaData(const QString& filename)
    : d(new StructureMetaDataPrivsate)
{
    KDesktopFile file(filename);

    KConfigGroup cg = file.desktopGroup();
    if (!cg.exists()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << filename << "has no [Desktop] group, cannot construct a StructureMetaData object from it.";
        return;
    }
    d->id = cg.readEntryUntranslated("X-KDE-PluginInfo-Name");
    if (d->id.isEmpty()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << filename << "has an empty X-KDE-PluginInfo-Name entry, cannot construct a StructureMetaData object from it.";
        return;
    }

    d->entryPath = filename;

    d->name = file.readName();
    d->comment = file.readComment();
    d->iconName = cg.readEntryUntranslated("Icon");
    d->authorName = cg.readEntryUntranslated("X-KDE-PluginInfo-Author");
    d->authorEmailAddress = cg.readEntryUntranslated("X-KDE-PluginInfo-Email");
    d->version = cg.readEntryUntranslated("X-KDE-PluginInfo-Version");
    d->website = cg.readEntryUntranslated("X-KDE-PluginInfo-Website");
    d->categoryId = cg.readEntryUntranslated("X-KDE-PluginInfo-Category");
    d->license = KAboutLicense::byKeyword(cg.readEntryUntranslated("X-KDE-PluginInfo-License"));
}

StructureMetaData::StructureMetaData()
    : d(new StructureMetaDataPrivsate())
{
}


StructureMetaData::StructureMetaData(const StructureMetaData &rhs) = default;

StructureMetaData::~StructureMetaData() = default;

StructureMetaData& StructureMetaData::operator=(const StructureMetaData& rhs) = default;

bool StructureMetaData::isValid() const
{
    return !d->id.isEmpty();
}

QString StructureMetaData::id() const
{
    return d->id;
}

QString StructureMetaData::name() const
{
    return d->name;
}

QString StructureMetaData::comment() const
{
    return d->comment;
}

QString StructureMetaData::categoryId() const
{
    return d->categoryId;
}

QString StructureMetaData::version() const
{
    return d->version;
}

bool StructureMetaData::hasAuthorName() const
{
    return !d->authorName.isEmpty();
}

QString StructureMetaData::authorName() const
{
    return d->authorName;
}

bool StructureMetaData::hasAuthorEmailAddress() const
{
    return !d->authorEmailAddress.isEmpty();
}

QString StructureMetaData::authorEmailAddress() const
{
    return d->authorEmailAddress;
}

bool StructureMetaData::hasIconName() const
{
    return !d->iconName.isEmpty();
}

QString StructureMetaData::iconName() const
{
    return d->iconName;
}

bool StructureMetaData::hasWebsite() const
{
    return !d->website.isEmpty();
}

QString StructureMetaData::website() const
{
    return d->website;
}

QString StructureMetaData::entryPath() const
{
    return d->entryPath;
}

const KAboutLicense& StructureMetaData::license() const
{
    return d->license;
}
