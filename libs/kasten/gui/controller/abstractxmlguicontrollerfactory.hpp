/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTXMLGUICONTROLLERFACTORY_HPP
#define KASTEN_ABSTRACTXMLGUICONTROLLERFACTORY_HPP

// lib
#include "kastengui_export.hpp"

class KXMLGUIClient;

namespace Kasten {

class AbstractXmlGuiController;

class KASTENGUI_EXPORT AbstractXmlGuiControllerFactory
{
public:
    virtual ~AbstractXmlGuiControllerFactory();

public: // API to be implemented
    virtual AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const = 0;
};

inline AbstractXmlGuiControllerFactory::~AbstractXmlGuiControllerFactory() = default;

}

#endif
