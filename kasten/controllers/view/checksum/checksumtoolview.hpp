/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMTOOLVIEW_HPP
#define KASTEN_CHECKSUMTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class ChecksumView;
class ChecksumTool;

class ChecksumToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit ChecksumToolView(ChecksumTool* tool);
    ~ChecksumToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    ChecksumView* mWidget;
};

}

#endif
