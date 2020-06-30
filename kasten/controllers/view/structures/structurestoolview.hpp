/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESTOOLVIEW_HPP
#define KASTEN_STRUCTURESTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {
class StructureView;
class StructuresTool;

class StructuresToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit StructuresToolView(StructuresTool* tool);
    ~StructuresToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    StructureView* mWidget;
};

}

#endif
