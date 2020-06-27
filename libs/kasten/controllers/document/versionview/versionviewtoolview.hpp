/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONVIEWTOOLVIEW_HPP
#define KASTEN_VERSIONVIEWTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class VersionView;
class VersionViewTool;

class VersionViewToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit VersionViewToolView(VersionViewTool* tool);
    ~VersionViewToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    VersionView* mWidget;
};

}

#endif
