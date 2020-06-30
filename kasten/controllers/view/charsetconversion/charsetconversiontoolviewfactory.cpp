/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversiontoolviewfactory.hpp"

// lib
#include "charsetconversiontoolview.hpp"
#include "charsetconversiontool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

CharsetConversionToolViewFactory::CharsetConversionToolViewFactory() = default;

CharsetConversionToolViewFactory::~CharsetConversionToolViewFactory() = default;

QString CharsetConversionToolViewFactory::iconName() const { return QStringLiteral("okteta"); }
QString CharsetConversionToolViewFactory::title()    const { return i18nc("@title:window", "Charset Conversion"); }
QString CharsetConversionToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.CharsetConversionToolView"); }
SidePosition CharsetConversionToolViewFactory::defaultPosition() const { return RightSidePosition; }

AbstractToolView* CharsetConversionToolViewFactory::create(AbstractTool* tool) const
{
    return new CharsetConversionToolView(qobject_cast<CharsetConversionTool*>(tool));
}

}
