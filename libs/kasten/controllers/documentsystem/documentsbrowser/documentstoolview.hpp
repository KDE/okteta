/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSTOOLVIEW_HPP
#define KASTEN_DOCUMENTSTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class DocumentsView;
class DocumentsTool;

class DocumentsToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit DocumentsToolView(DocumentsTool* tool);
    ~DocumentsToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    DocumentsView* mWidget;
};

}

#endif
