/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTINFOTOOLVIEW_HPP
#define KASTEN_DOCUMENTINFOTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

namespace Kasten {

class DocumentInfoView;
class DocumentInfoTool;

class DocumentInfoToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit DocumentInfoToolView(DocumentInfoTool* tool);
    ~DocumentInfoToolView() override;

public: // AbstractToolView API
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<DocumentInfoView> mWidget;
};

}

#endif
