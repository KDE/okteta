/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INFOTOOLVIEW_HPP
#define KASTEN_INFOTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class InfoView;
class InfoTool;

class InfoToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit InfoToolView(InfoTool* tool);
    ~InfoToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    InfoView* mWidget;
};

}

#endif
