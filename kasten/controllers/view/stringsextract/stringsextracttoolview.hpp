/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTTOOLVIEW_HPP
#define KASTEN_STRINGSEXTRACTTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class StringsExtractView;
class StringsExtractTool;

class StringsExtractToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit StringsExtractToolView(StringsExtractTool* tool);
    ~StringsExtractToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    StringsExtractView* mWidget;
};

}

#endif
