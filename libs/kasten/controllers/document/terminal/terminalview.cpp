/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "terminalview.hpp"

// lib
#include "terminaltool.hpp"
// KF
#include <kcoreaddons_version.h>
#include <KLocalizedString>
#if KCOREADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
#include <KPluginFactory>
#include <KPluginMetaData>
#else
#include <KServiceTypeTrader>
#endif
#include <KParts/ReadOnlyPart>
#include <kde_terminal_interface.h>
// Qt
#include <QVBoxLayout>
#include <QFrame>
#include <QUrl>
#include <QDir>

namespace Kasten {

TerminalView::TerminalView(TerminalTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

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
#if KCOREADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
    KPluginFactory *factory = KPluginFactory::loadFactory(KPluginMetaData(QStringLiteral("konsolepart"))).plugin;
    mTerminalPart = factory ? (factory->create<KParts::ReadOnlyPart>(this)) : nullptr;
#else
    mTerminalPart =
        KServiceTypeTrader::createInstanceFromQuery<KParts::ReadOnlyPart>(
            QStringLiteral("TerminalEmulator"), this, this);
#endif

    if (mTerminalPart) {
        connect(mTerminalPart, &QObject::destroyed,
                this, &TerminalView::onTerminalPartDestroyed);

        QWidget* terminalWidget = mTerminalPart->widget();
        terminalWidget->setFocusPolicy(Qt::WheelFocus);
        terminalWidget->setFocus();
        setFocusProxy(terminalWidget);

        auto* frame = qobject_cast<QFrame*>(terminalWidget);
        if (frame) {
            frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        }

        auto* layout = static_cast<QVBoxLayout*>(this->layout());
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

#include "moc_terminalview.cpp"
