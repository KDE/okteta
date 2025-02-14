/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionviewtool.hpp"

// Kasten core
#include <Kasten/Versionable>
#include <Kasten/AbstractDocument>
// KF
#include <KLocalizedString>

namespace Kasten {

VersionViewTool::VersionViewTool()
{
    setObjectName(QStringLiteral("Versions"));
}

VersionViewTool::~VersionViewTool() = default;

QString VersionViewTool::title() const { return i18nc("@title:window", "Versions"); }

void VersionViewTool::setTargetModel(AbstractModel* model)
{
    AbstractModel* const versionableModel = model ? model->findBaseModelWithInterface<If::Versionable*>() : nullptr;
    if (versionableModel == mModel) {
        return;
    }

    mModel = versionableModel;

    Q_EMIT modelChanged(mModel);
}

}

#include "moc_versionviewtool.cpp"
