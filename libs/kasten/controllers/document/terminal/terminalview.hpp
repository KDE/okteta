/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TERMINALVIEW_HPP
#define KASTEN_TERMINALVIEW_HPP

// Qt
#include <QWidget>

class TerminalInterface;
namespace KParts {
class ReadOnlyPart;
}
class QUrl;

namespace Kasten {

class TerminalTool;

class TerminalView : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalView(TerminalTool* tool, QWidget* parent = nullptr);
    ~TerminalView() override;

public:
    TerminalTool* tool() const;

private Q_SLOTS:
    void createTerminalPart();

    void onCurrentUrlChanged(const QUrl& currentUrl);

    void onTerminalPartDestroyed();

private:
    TerminalTool* const mTool;

    KParts::ReadOnlyPart* mTerminalPart = nullptr;
    TerminalInterface* mTerminalInterface = nullptr;
};

inline TerminalTool* TerminalView::tool() const { return mTool; }

}

#endif
