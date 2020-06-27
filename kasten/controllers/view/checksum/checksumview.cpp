/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumview.hpp"

// tool
#include "checksumtool.hpp"
// lib
#include <bytearraychecksumparameterseteditfactory.hpp>
#include <abstractbytearraychecksumparametersetedit.hpp>
#include <abstractbytearraychecksumparameterset.hpp>
#include <abstractbytearraychecksumalgorithm.hpp>
// KF
#include <KComboBox>
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QLabel>
#include <QGroupBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QClipboard>
#include <QApplication>
#include <QAbstractItemView>

namespace Kasten {

ChecksumView::ChecksumView(ChecksumTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // algorithm
    auto* algorithmLayout = new QHBoxLayout();
    QLabel* label = new QLabel(i18nc("@label:listbox algorithm to use for the checksum", "Algorithm:"), this);
    mAlgorithmComboBox = new KComboBox(this);
    connect(mAlgorithmComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &ChecksumView::onOperationChange);

    label->setBuddy(mAlgorithmComboBox);
    const QString algorithmWhatsThis =
        i18nc("@info:whatsthis", "Select the algorithm to use for the checksum.");
    label->setWhatsThis(algorithmWhatsThis);
    mAlgorithmComboBox->setWhatsThis(algorithmWhatsThis);

    algorithmLayout->addWidget(label);
    algorithmLayout->addWidget(mAlgorithmComboBox, 10);
    baseLayout->addLayout(algorithmLayout);

    // parameter
    QGroupBox* parameterSetBox = new QGroupBox(i18nc("@title:group", "Parameters"), this);
    baseLayout->addWidget(parameterSetBox);

    auto* parameterSetLayout = new QVBoxLayout(parameterSetBox);

    mParameterSetEditStack = new QStackedWidget(parameterSetBox);
    parameterSetLayout->addWidget(mParameterSetEditStack);

    // calculate
    auto* calculateLayout = new QHBoxLayout();

    calculateLayout->addStretch();
    const KGuiItem updateGuiItem =
        KGuiItem(i18nc("@action:button calculate the checksum", "&Calculate"),
                 QStringLiteral("run-build"),
                 i18nc("@info:tooltip",
                       "Calculate the checksum for the bytes in the selected range."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Calculate</interface> button, the list will be updated "
                        "to all strings which are contained in the selected range and have the set minimum length."));
    mCalculateButton = new QPushButton(this);
    KGuiItem::assign(mCalculateButton, updateGuiItem);
    mCalculateButton->setEnabled(mTool->isApplyable());
    connect(mCalculateButton, &QPushButton::clicked, this, &ChecksumView::onCalculateClicked);
    addButton(mCalculateButton, AbstractToolWidget::Default);
    calculateLayout->addWidget(mCalculateButton);
    baseLayout->addLayout(calculateLayout);

    mChecksumLabel = new QLineEdit(this);
    mChecksumLabel->setReadOnly(true);
    mChecksumLabel->setText(mTool->checkSum());
    connect(mTool, &ChecksumTool::checksumChanged, mChecksumLabel, &QLineEdit::setText);
    baseLayout->addWidget(mChecksumLabel, 10);

    baseLayout->addStretch(10);

    connect(mTool, &ChecksumTool::uptodateChanged, this, &ChecksumView::onChecksumUptodateChanged);
    connect(mTool, &ChecksumTool::isApplyableChanged, this, &ChecksumView::onApplyableChanged);

    // automatically set focus to the parameters if a operation has been selected
    QAbstractItemView* algorithmComboBoxListView = mAlgorithmComboBox->view();
    QObject::connect(algorithmComboBoxListView, &QAbstractItemView::activated,
                     mParameterSetEditStack, QOverload<>::of(&QStackedWidget::setFocus));
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect(algorithmComboBoxListView, &QAbstractItemView::pressed,
                     mParameterSetEditStack, QOverload<>::of(&QStackedWidget::setFocus));
    // TODO: goto filter button if there are no parameters

    addAlgorithms();
}

ChecksumView::~ChecksumView() = default;

void ChecksumView::addAlgorithms()
{
    //
    const QVector<AbstractByteArrayChecksumAlgorithm*> algorithmList = mTool->algorithmList();
    for (AbstractByteArrayChecksumAlgorithm* algorithm : algorithmList) {
        mAlgorithmComboBox->addItem(algorithm->name());

        const char* const parameterSetId = algorithm->parameterSet()->id();
        AbstractByteArrayChecksumParameterSetEdit* parameterEdit =
            ByteArrayChecksumParameterSetEditFactory::createEdit(parameterSetId);

        mParameterSetEditStack->addWidget(parameterEdit);
    }

    onOperationChange(mTool->algorithmId());
}

void ChecksumView::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayChecksumParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->getParameterSet(parameterSet);
    }
}

void ChecksumView::onCalculateClicked()
{
    AbstractByteArrayChecksumParameterSet* parameterSet = mTool->parameterSet();
    if (parameterSet) {
        getParameterSet(parameterSet);
    }

    mTool->calculateChecksum();
}

void ChecksumView::onOperationChange(int index)
{
    QWidget* oldWidget = mParameterSetEditStack->currentWidget();
    if (oldWidget) {
        oldWidget->disconnect(this);
        oldWidget->disconnect(mTool);
    }

    mTool->setAlgorithm(index);
    mParameterSetEditStack->setCurrentIndex(index);

    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayChecksumParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        connect(parametersetEdit, &AbstractByteArrayChecksumParameterSetEdit::validityChanged,
                this, &ChecksumView::onValidityChanged);
        // TODO: hack, see checksum source
        connect(parametersetEdit, &AbstractByteArrayChecksumParameterSetEdit::valuesChanged,
                mTool, &ChecksumTool::resetSourceTool);
        onValidityChanged(parametersetEdit->isValid());
    }
}

void ChecksumView::onChecksumUptodateChanged(bool checksumUptodate)
{
    const bool isApplyable = mTool->isApplyable();
    mCalculateButton->setEnabled(!checksumUptodate && isApplyable);
}

void ChecksumView::onApplyableChanged(bool isApplyable)
{
    mCalculateButton->setEnabled(!mTool->isUptodate() && isApplyable);
}

void ChecksumView::onValidityChanged(bool isValid)
{
    mCalculateButton->setEnabled(mTool->isApplyable() && isValid);
}

}
