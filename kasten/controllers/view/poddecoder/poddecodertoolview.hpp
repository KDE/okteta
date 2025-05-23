/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODDECODERTOOLVIEW_HPP
#define KASTEN_PODDECODERTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

namespace Kasten {

class PODTableView;
class PODDecoderTool;
class AbstractUserMessagesHandler;

class PODDecoderToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit PODDecoderToolView(PODDecoderTool* tool, AbstractUserMessagesHandler* userMessagesHandler);
    ~PODDecoderToolView() override;

public: // AbstractToolView API
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<PODTableView> mWidget;
};

}

#endif
