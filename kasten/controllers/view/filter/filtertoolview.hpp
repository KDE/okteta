/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERTOOLVIEW_HPP
#define KASTEN_FILTERTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

namespace Kasten {

class FilterView;
class FilterTool;

class FilterToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit FilterToolView(FilterTool* tool);
    ~FilterToolView() override;

public: // AbstractToolView API
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<FilterView> mWidget;
};

}

#endif
