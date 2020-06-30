/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfinddialog.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// Okteta core
#include <Okteta/OktetaCore>
// KF
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QGroupBox>
#include <QLayout>
#include <QIcon>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Kasten {

AbstractFindDialog::AbstractFindDialog(QWidget* parent)
    : QDialog(parent)
{
    // main widget
    QWidget* mainWidget = new QWidget;
    MainWidgetLayout = new QVBoxLayout(mainWidget);
    MainWidgetLayout->setContentsMargins(0, 0, 0, 0);

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    FindButton = new QPushButton;
    dialogButtonBox->addButton(FindButton, QDialogButtonBox::AcceptRole);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &AbstractFindDialog::forwardFindButtonClicked);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(mainWidget);
    layout->addStretch();
    layout->addWidget(dialogButtonBox);

    setLayout(layout);
    // TODO: setting ok button to disabled as before done gets overwritten to true
    // if setting the button gui with an inline KGuiItem in the subclass,
    // which has no parameter for enabled and defaults to true
}

AbstractFindDialog::~AbstractFindDialog() = default;

void AbstractFindDialog::setFindButton(const QString& buttonText, const QString& buttonIconName,
                                       const QString& buttonToolTip, const QString& buttonWhatsThis)
{
    FindButton->setText(buttonText);
    FindButton->setIcon(QIcon::fromTheme(buttonIconName));
    FindButton->setToolTip(buttonToolTip);
    FindButton->setWhatsThis(buttonWhatsThis);
}

void AbstractFindDialog::setFindButtonEnabled(bool enabled)
{
    FindButton->setEnabled(enabled);
}

void AbstractFindDialog::setupFindBox()
{
    // find term
    QGroupBox* findBox = new QGroupBox(i18nc("@title:window", "Find"));
    MainWidgetLayout->addWidget(findBox);

    auto* findBoxLayout = new QVBoxLayout;

    SearchDataEdit = new Okteta::ByteArrayComboBox(findBox);
    connect(SearchDataEdit, &Okteta::ByteArrayComboBox::byteArrayChanged,
            this, &AbstractFindDialog::onSearchDataChanged);
    connect(SearchDataEdit, &Okteta::ByteArrayComboBox::formatChanged,
            this, &AbstractFindDialog::onSearchDataFormatChanged);
    const QString toolTip =
        i18nc("@info:tooltip",
              "Enter the bytes to search for, or select bytes previously searched for from the list.");
    SearchDataEdit->setToolTip(toolTip);

    findBoxLayout->addWidget(SearchDataEdit);
    findBox->setLayout(findBoxLayout);
}

void AbstractFindDialog::setupOperationBox(QGroupBox* operationBox)
{
    // operation box
    if (operationBox) {
        MainWidgetLayout->addWidget(operationBox);
    }
}

void AbstractFindDialog::setupCheckBoxes(QCheckBox* optionCheckBox)
{
    // options
    QGroupBox* optionsBox = new QGroupBox(i18nc("@title:group", "Options"));
    MainWidgetLayout->addWidget(optionsBox);

    auto* optionsBoxLayout = new QGridLayout(optionsBox);

    CaseSensitiveCheckBox = new QCheckBox(i18nc("@option:check", "C&ase sensitive"), optionsBox);
    CaseSensitiveCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Perform a case sensitive search: "
                                              "entering the pattern 'Joe' will not match 'joe' or 'JOE', only 'Joe'."));
    WholeWordsCheckBox = new QCheckBox(i18nc("@option:check", "&Whole words only"), optionsBox);
    WholeWordsCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Require word boundaries in both ends of a match to succeed."));
    AtCursorCheckBox = new QCheckBox(i18nc("@option:check", "From c&ursor"), optionsBox);
    AtCursorCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Start searching at the current cursor location rather than at the top."));

    BackwardsCheckBox = new QCheckBox(i18nc("@option:check", "&Backwards"), optionsBox);
    BackwardsCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Replace backwards."));
    SelectedCheckBox = new QCheckBox(i18nc("@option:check", "&Selected bytes"), optionsBox);
    SelectedCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Only search within the current selection."));

    optionsBoxLayout->addWidget(CaseSensitiveCheckBox, 0, 0);
    optionsBoxLayout->addWidget(WholeWordsCheckBox, 1, 0);
    optionsBoxLayout->addWidget(AtCursorCheckBox, 2, 0);
    optionsBoxLayout->addWidget(BackwardsCheckBox, 0, 1);
    optionsBoxLayout->addWidget(SelectedCheckBox, 1, 1);
    if (optionCheckBox) {
        optionsBoxLayout->addWidget(optionCheckBox, 2, 1);
    }

    setTabOrder(CaseSensitiveCheckBox, WholeWordsCheckBox);
    setTabOrder(WholeWordsCheckBox, AtCursorCheckBox);
    setTabOrder(AtCursorCheckBox, BackwardsCheckBox);
    setTabOrder(BackwardsCheckBox, SelectedCheckBox);
//     if( optionCheckBox )
//         setTabOrder( SelectedCheckBox, optionCheckBox );

    connect(SelectedCheckBox, &QAbstractButton::toggled,
            this, &AbstractFindDialog::onSelectedToggled);

    onSearchDataFormatChanged(SearchDataEdit->format());
}

bool AbstractFindDialog::fromCursor()            const { return AtCursorCheckBox->isChecked(); }
bool AbstractFindDialog::inSelection()           const { return SelectedCheckBox->isChecked(); }
FindDirection AbstractFindDialog::direction() const
{
    return BackwardsCheckBox->isChecked() ? FindBackward : FindForward;
}
Qt::CaseSensitivity AbstractFindDialog::caseSensitivity() const
{
    return (SearchDataEdit->format() == Okteta::ByteArrayComboBox::CharCoding)
           && !CaseSensitiveCheckBox->isChecked() ? Qt::CaseInsensitive : Qt::CaseSensitive;
}

QByteArray AbstractFindDialog::data()  const
{
    return SearchDataEdit->byteArray();
}

void AbstractFindDialog::setDirection(FindDirection Direction)
{
    BackwardsCheckBox->setChecked(Direction == FindBackward);
}

void AbstractFindDialog::setInSelection(bool InSelection)
{
    SelectedCheckBox->setChecked(InSelection);
}

void AbstractFindDialog::setInSelectionEnabled(bool inSelectionEnabled)
{
    if (!inSelectionEnabled) {
        SelectedCheckBox->setChecked(false);
    }
    SelectedCheckBox->setEnabled(inSelectionEnabled);
}

void AbstractFindDialog::setCharCodec(const QString& codecName)
{
    SearchDataEdit->setCharCodec(codecName);
}

void AbstractFindDialog::rememberCurrentSettings()
{
    SearchDataEdit->rememberCurrentByteArray();
}

void AbstractFindDialog::onSelectedToggled(bool checked)
{
    AtCursorCheckBox->setEnabled(!checked);
}

void AbstractFindDialog::onFindButtonClicked()
{
}

void AbstractFindDialog::onSearchDataFormatChanged(int index)
{
    const bool isCharCoding = (index == Okteta::ByteArrayComboBox::CharCoding);
    CaseSensitiveCheckBox->setEnabled(isCharCoding);
    WholeWordsCheckBox->setEnabled(false);  // isCharCoding ); TODO: not implemented!
}

void AbstractFindDialog::onSearchDataChanged(const QByteArray& data)
{
    FindButton->setEnabled(!data.isEmpty());
}

void AbstractFindDialog::forwardFindButtonClicked()
{
    onFindButtonClicked();
}

void AbstractFindDialog::showEvent(QShowEvent* showEvent)
{
    QDialog::showEvent(showEvent);
    SearchDataEdit->setFocus();
}

}
