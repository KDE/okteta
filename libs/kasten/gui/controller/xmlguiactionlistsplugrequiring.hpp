/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_XMLGUIACTIONLISTSPLUGREQUIRING_HPP
#define KASTEN_IF_XMLGUIACTIONLISTSPLUGREQUIRING_HPP

// Qt
#include <QtPlugin>

namespace Kasten {

namespace If {

class XmlGuiActionListsPlugRequiring
{
public:
    virtual ~XmlGuiActionListsPlugRequiring();

public: // enact
    virtual void plugActionLists() = 0;

};

inline XmlGuiActionListsPlugRequiring::~XmlGuiActionListsPlugRequiring() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::XmlGuiActionListsPlugRequiring, "org.kde.kasten.if.xmlguiactionlistsplugrequiring/1.0")

#endif
