/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_XMLGUIACTIONLISTSREPLUGREQUIRING_HPP
#define KASTEN_IF_XMLGUIACTIONLISTSREPLUGREQUIRING_HPP

// Qt
#include <QtPlugin>

namespace Kasten {

namespace If {

class XmlGuiActionListsReplugRequiring
{
public:
    virtual ~XmlGuiActionListsReplugRequiring();

public: // enact
    virtual void replugActionLists() = 0;

};

inline XmlGuiActionListsReplugRequiring::~XmlGuiActionListsReplugRequiring() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::XmlGuiActionListsReplugRequiring, "org.kde.kasten.if.xmlguiactionlistsreplugrequiring/1.0")

#endif
