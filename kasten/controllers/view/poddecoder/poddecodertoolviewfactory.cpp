/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddecodertoolviewfactory.hpp"

// lib
#include "poddecodertoolview.hpp"
#include "poddecodertool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

PodDecoderToolViewFactory::PodDecoderToolViewFactory(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
}

PodDecoderToolViewFactory::~PodDecoderToolViewFactory() = default;

QString PodDecoderToolViewFactory::iconName() const { return QStringLiteral("okteta"); }
QString PodDecoderToolViewFactory::title()       const { return i18nc("@title:window", "Decoding Table"); }
QString PodDecoderToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.PodDecoderToolView"); }
SidePosition PodDecoderToolViewFactory::defaultPosition() const { return RightSidePosition; }

std::unique_ptr<AbstractToolView> PodDecoderToolViewFactory::create(AbstractTool* tool) const
{
    return std::make_unique<PODDecoderToolView>(qobject_cast<PODDecoderTool*>(tool), m_userMessagesHandler);
}

}
