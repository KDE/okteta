/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "terminalview.hpp"

// lib
#include "terminaltool.hpp"
// KF5
#include <KLocalizedString>
#include <KServiceTypeTrader>
#include <KParts/ReadOnlyPart>
#include <kde_terminal_interface.h>
// Qt
#include <QLayout>
#include <QFrame>
#include <QUrl>
#include <QDir>

namespace Kasten {

TerminalView::TerminalView(TerminalTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);

    connect(mTool, &TerminalTool::currentUrlChanged, this, &TerminalView::onCurrentUrlChanged);
    QMetaObject::invokeMethod(this, "createTerminalPart", Qt::QueuedConnection);
}

TerminalView::~TerminalView()
{
    if (mTerminalPart) {
        disconnect(mTerminalPart, &QObject::destroyed,
                   this, &TerminalView::onTerminalPartDestroyed);
    }
}

void TerminalView::createTerminalPart()
{
    mTerminalPart =
        KServiceTypeTrader::createInstanceFromQuery<KParts::ReadOnlyPart>(
            QStringLiteral("TerminalEmulator"), this, this);

    if (mTerminalPart) {
        connect(mTerminalPart, &QObject::destroyed,
                this, &TerminalView::onTerminalPartDestroyed);

        QWidget* terminalWidget = mTerminalPart->widget();
        terminalWidget->setFocusPolicy(Qt::WheelFocus);
        terminalWidget->setFocus();
        setFocusProxy(terminalWidget);

        QFrame* frame = qobject_cast<QFrame*>(terminalWidget);
        if (frame) {
            frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        }

        QVBoxLayout* layout = static_cast<QVBoxLayout*>(this->layout());
        layout->addWidget(terminalWidget);
        terminalWidget->show();

        mTerminalInterface = qobject_cast<TerminalInterface*>(mTerminalPart);
        QUrl currentUrl = mTool->currentUrl();
        if (currentUrl.isEmpty()) {
            currentUrl = QUrl::fromLocalFile(QDir::homePath());
        }
        onCurrentUrlChanged(currentUrl);
    }
}

void TerminalView::onCurrentUrlChanged(const QUrl& currentUrl)
{
    if (mTerminalInterface && currentUrl.isLocalFile()) {
        mTerminalInterface->showShellInDir(currentUrl.path());
    }
    // TODO: Konsole currently seems to ignore this call if shell is running?
}

void TerminalView::onTerminalPartDestroyed()
{
    mTerminalPart = nullptr;
    mTerminalInterface = nullptr;

    createTerminalPart();
}

}
