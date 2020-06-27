/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TERMINALTOOLVIEW_HPP
#define KASTEN_TERMINALTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class TerminalView;
class TerminalTool;

class TerminalToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit TerminalToolView(TerminalTool* tool);
    ~TerminalToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    TerminalView* mWidget;
};

}

#endif
