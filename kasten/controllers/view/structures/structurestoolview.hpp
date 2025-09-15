/*
    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESTOOLVIEW_HPP
#define KASTEN_STRUCTURESTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

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
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<StructureView> mWidget;
};

}

#endif
