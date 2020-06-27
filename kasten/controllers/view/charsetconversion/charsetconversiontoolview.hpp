/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONTOOLVIEW_HPP
#define KASTEN_CHARSETCONVERSIONTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class CharsetConversionView;
class CharsetConversionTool;

class CharsetConversionToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit CharsetConversionToolView(CharsetConversionTool* tool);
    ~CharsetConversionToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    CharsetConversionView* mWidget;
};

}

#endif
