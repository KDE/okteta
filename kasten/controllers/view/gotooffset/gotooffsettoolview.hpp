/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETTOOLVIEW_HPP
#define KASTEN_GOTOOFFSETTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolInlineView>
// Std
#include <memory>

namespace Kasten {

class GotoOffsetView;
class GotoOffsetTool;

class GotoOffsetToolView : public AbstractToolInlineView
{
    Q_OBJECT

public:
    explicit GotoOffsetToolView(GotoOffsetTool* tool);
    ~GotoOffsetToolView() override;

public: // AbstractToolView API
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<GotoOffsetView> mWidget;
};

}

#endif
