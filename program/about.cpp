/*
    SPDX-FileCopyrightText: 2006-2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "about.hpp"
#include "about-config.hpp"

// KF
#include <KLocalizedString>

OktetaAboutData::OktetaAboutData()
    : KAboutData(QStringLiteral("okteta"),
                 i18n("Okteta"), // name
                 QStringLiteral(OKTETA_VERSION),
                 i18nc("@info", "Hex editor"), // description
                 KAboutLicense::GPL_V2,
                 i18nc("@info", "2006-%1 Friedrich W. H. Kossebau", QStringLiteral("2026")), // copyright
                 i18nc("@info", "Edit the raw data of files"), // comment
                 QStringLiteral("https://userbase.kde.org/Okteta"))
{
    addLicense(KAboutLicense::GPL_V3);
    setOrganizationDomain("kde.org");
    addAuthor(i18nc("@info:credit", "Friedrich W. H. Kossebau"), // name
              i18nc("@info:credit", "Author"), // task
              QStringLiteral("kossebau@kde.org"));
    addAuthor(i18nc("@info:credit", "Alexander Richardson"), // name
              i18nc("@info:credit", "Structures tool"), // task
              QStringLiteral("alex.richardson@gmx.de"));
    setDesktopFileName(QStringLiteral("org.kde.okteta"));
}
