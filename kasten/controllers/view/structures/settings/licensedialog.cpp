/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "licensedialog.hpp"

// KF
#include <KAboutData>
#include <KLocalizedString>
// Qt
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QFontMetrics>
// Std
#include <cmath>


LicenseDialog::LicenseDialog(const KAboutLicense& license, QWidget* parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(i18nc("@title:window", "License Agreement"));

    auto* mainLayout = new QVBoxLayout(this);

    const QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    const QString licenseText = license.text();

    auto* licenseBrowser = new QTextBrowser(this);
    licenseBrowser->setFont(font);
    licenseBrowser->setLineWrapMode(QTextEdit::NoWrap);
    licenseBrowser->setText(licenseText);
    mainLayout->addWidget(licenseBrowser);

    auto* buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    // Resize initially to a width without a horizontal scrollbar
    const QMargins mainLayoutMargine = mainLayout->contentsMargins();
    const int idealDialogWidth = std::ceil(licenseBrowser->document()->idealWidth()) + mainLayoutMargine.left() + mainLayoutMargine.right() + licenseBrowser->verticalScrollBar()->width() + 2 * licenseBrowser->frameWidth();

    // Some rough initial number of lines (matching 5.95 KXmlGui's license dialog), ignoring dialogbutton box details
    QFontMetrics metrics(font);
    const int someDialogHeight = metrics.height() * 30;

    resize(sizeHint().expandedTo(QSize(idealDialogWidth, someDialogHeight)));
}
