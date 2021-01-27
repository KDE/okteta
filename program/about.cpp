/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "about.hpp"

// KF
#include <KLocalizedString>

OktetaAboutData::OktetaAboutData()
    : KAboutData(QStringLiteral("okteta"),
                 i18n("Okteta"), // name
                 QStringLiteral(OKTETA_VERSION),
                 i18n("Hex editor"), // description
                 KAboutLicense::GPL_V2,
                 i18n("2006-%1 Friedrich W. H. Kossebau", QStringLiteral("2021")), // copyright
                 i18n("Edit the raw data of files"), // comment
                 QStringLiteral("https://userbase.kde.org/Okteta"))
{
    addLicense(KAboutLicense::GPL_V3);
    setOrganizationDomain("kde.org");
    addAuthor(i18n("Friedrich W. H. Kossebau"), // name
              i18n("Author"), // task
              QStringLiteral("kossebau@kde.org"));
    addAuthor(i18nc("Author", "Alexander Richardson"), // name
              i18nc("Task description", "Structures tool"), // task
              QStringLiteral("alex.richardson@gmx.de"));
    setDesktopFileName(QStringLiteral("org.kde.okteta"));
}
