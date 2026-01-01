/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "aboutstructuredialog.hpp"

// tool
#include "licensedialog.hpp"
// KF
#include <KTitleWidget>
#include <KLocalizedString>
#include <KAboutData>
// Qt
#include <QGuiApplication>
#include <QTabWidget>
#include <QToolBar>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>

namespace Kasten {

AboutStructureDialog::AboutStructureDialog(const StructureMetaData& structureMetaData, QWidget* parent)
    : QDialog(parent)
    , m_structureMetaData(structureMetaData)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(i18nc("@title:window", "About %1", m_structureMetaData.name()));

    auto* const mainLayout = new QVBoxLayout(this);

    // title
    const QIcon titleIcon = m_structureMetaData.hasIconName() ? QIcon::fromTheme(m_structureMetaData.iconName()) : qApp->windowIcon();
    auto* const titleWidget = new KTitleWidget(this);

    const QString titleText = QLatin1String("<font size=\"5\">") + m_structureMetaData.name() + QLatin1String("</font><br />") + i18nc("Version version-number", "Version %1", m_structureMetaData.version());
    titleWidget->setIconSize(QSize(48, 48));
    titleWidget->setIcon(titleIcon, KTitleWidget::ImageLeft);
    titleWidget->setText(titleText);
    mainLayout->addWidget(titleWidget);

    // tab bar
    auto* const tabWidget = new QTabWidget;
    tabWidget->setUsesScrollButtons(false);

    // About page
    auto* const aboutWidget = new QWidget(this);
    auto* const aboutLayout = new QVBoxLayout(aboutWidget);

    aboutLayout->addStretch();

    QString aboutPageText = m_structureMetaData.comment().trimmed();

    if (m_structureMetaData.hasWebsite()) {
        if (!aboutPageText.isEmpty()) {
            aboutPageText += QLatin1String("<br /><br />");
        }
        aboutPageText += QLatin1String("<a href=\"%1\">") + m_structureMetaData.website() + QLatin1String("</a>");
    }

    if (!aboutPageText.isEmpty()) {
        auto* const aboutLabel = new QLabel;
        aboutLabel->setWordWrap(true);
        aboutLabel->setOpenExternalLinks(true);
        aboutLabel->setText(aboutPageText.replace(QLatin1Char('\n'), QStringLiteral("<br />")));
        aboutLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

        aboutLayout->addWidget(aboutLabel);
    }

    auto* const showLicenseLabel = new QLabel;
    if (m_structureMetaData.license().key() != KAboutLicense::Unknown) {
        const QString licenseLabelText = QLatin1String("<a href=\"showlicense\">") + i18n("License: %1", m_structureMetaData.license().name(KAboutLicense::FullName)) + QLatin1String("</a>");
        showLicenseLabel->setText(licenseLabelText);
        showLicenseLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        connect(showLicenseLabel, &QLabel::linkActivated, this, &AboutStructureDialog::showLicenseDialog);
    } else {
        showLicenseLabel->setText(i18n("License: %1", m_structureMetaData.license().name(KAboutLicense::FullName)));
    }
    aboutLayout->addWidget(showLicenseLabel);

    aboutLayout->addStretch();

    tabWidget->addTab(aboutWidget, i18nc("@title:tab", "About"));

    // author page
    if (m_structureMetaData.hasAuthorName())  {
        auto* const authorWidget = new QWidget(this);
        auto* const authorLayout = new QVBoxLayout(authorWidget);

        const QString nameMarkup = QLatin1String("<b>") + m_structureMetaData.authorName() + QLatin1String("</b>");
        auto* const nameLabel = new QLabel(nameMarkup);
        authorLayout->addWidget(nameLabel);

        if (m_structureMetaData.hasAuthorEmailAddress()) {
            auto* const contactToolBar = new QToolBar(this);
            auto* const emailAction = new QAction(this);
            emailAction->setIcon(QIcon::fromTheme(QStringLiteral("mail-send")));
            emailAction->setToolTip(i18nc("@info:tooltip Send an email to the author", "Email author\n%1", m_structureMetaData.authorEmailAddress()));
            connect(emailAction, &QAction::triggered, this, &AboutStructureDialog::emailAuthor);
            contactToolBar->addAction(emailAction);

            authorLayout->addWidget(contactToolBar);
        }

        authorLayout->addStretch();

        tabWidget->addTab(authorWidget, i18nc("@title:tab", "Author"));
    }
    mainLayout->addWidget(tabWidget);

    auto* const buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttonBox);
}

void AboutStructureDialog::showLicenseDialog()
{
    auto* const dialog = new LicenseDialog(m_structureMetaData.license(), this);
    dialog->show();
}

void AboutStructureDialog::emailAuthor()
{
    const QUrl mailToUrl(QLatin1String("mailto:") + m_structureMetaData.authorEmailAddress());
    QDesktopServices::openUrl(mailToUrl);
}

}

#include "moc_aboutstructuredialog.cpp"
