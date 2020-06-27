/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filterview.hpp"

//
#include "filtertool.hpp"
// filter
#include <bytearrayfilterparameterseteditfactory.hpp>
#include <abstractbytearrayfilterparametersetedit.hpp>
#include <abstractbytearrayfilterparameterset.hpp>
#include <abstractbytearrayfilter.hpp>
// KF
#include <KLocalizedString>
#include <KComboBox>
#include <KGuiItem>
// Qt
#include <QLabel>
#include <QLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QAbstractItemView>

namespace Kasten {

FilterView::FilterView(FilterTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // filter
    auto* operationLayout = new QHBoxLayout();
    QLabel* label = new QLabel(i18nc("@label:listbox operation to use by the filter", "Operation:"), this);
    mOperationComboBox = new KComboBox(this);
    connect(mOperationComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &FilterView::onOperationChange);

    label->setBuddy(mOperationComboBox);
    const QString operationToolTip =
        i18nc("@info:tooltip", "The operation to use for the filter.");
    label->setToolTip(operationToolTip);
    mOperationComboBox->setToolTip(operationToolTip);
    const QString operationWhatsThis =
        i18nc("@info:whatsthis", "Select the operation to use for the filter.");
    label->setWhatsThis(operationWhatsThis);
    mOperationComboBox->setWhatsThis(operationWhatsThis);

    operationLayout->addWidget(label);
    operationLayout->addWidget(mOperationComboBox, 10);
    baseLayout->addLayout(operationLayout);

    QGroupBox* parameterSetBox = new QGroupBox(i18nc("@title:group", "Parameters"), this);
    baseLayout->addWidget(parameterSetBox);

    auto* parameterSetLayout = new QVBoxLayout;

    parameterSetBox->setLayout(parameterSetLayout);

    mParameterSetEditStack = new QStackedWidget(parameterSetBox);

    parameterSetLayout->addWidget(mParameterSetEditStack);

    // filter button
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(10);
    mFilterButton = new QPushButton(this);
    KGuiItem::assign(mFilterButton, KGuiItem(i18nc("@action:button", "&Filter"),
                                             QStringLiteral("run-build"),
                                             i18nc("@info:tooltip", "Executes the filter for the bytes in the selected range."),
                                             xi18nc("@info:whatsthis",
                                                    "If you press the <interface>Filter</interface> button, the operation you selected "
                                                    "above is executed for the bytes in the selected range with the given options.")));
    mFilterButton->setEnabled(mTool->hasWriteable());
    connect(mTool, &FilterTool::hasWriteableChanged, this, &FilterView::onHasWriteableChanged);
    connect(mTool, &FilterTool::charCodecChanged, this, &FilterView::onCharCodecChanged);
    connect(mFilterButton, &QPushButton::clicked, this, &FilterView::onFilterClicked);
    addButton(mFilterButton, AbstractToolWidget::Default);
    buttonLayout->addWidget(mFilterButton);
    baseLayout->addLayout(buttonLayout);
    baseLayout->addStretch(10);

    // automatically set focus to the parameters if a operation has been selected
    QAbstractItemView* operationComboBoxListView = mOperationComboBox->view();
    QObject::connect(operationComboBoxListView, &QAbstractItemView::activated,
                     mParameterSetEditStack, QOverload<>::of(&QStackedWidget::setFocus));
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect(operationComboBoxListView, &QAbstractItemView::pressed,
                     mParameterSetEditStack, QOverload<>::of(&QStackedWidget::setFocus));
    // TODO: goto filter button if there are no parameters

    addFilters();
}

FilterView::~FilterView() = default;

void FilterView::addFilters()
{
    //
    const QVector<AbstractByteArrayFilter*> filterList = mTool->filterList();
    for (AbstractByteArrayFilter* filter : filterList) {
        mOperationComboBox->addItem(filter->name());

        const char* parameterSetId = filter->parameterSet()->id();
        AbstractByteArrayFilterParameterSetEdit* parameterEdit =
            ByteArrayFilterParameterSetEditFactory::createEdit(parameterSetId);

        mParameterSetEditStack->addWidget(parameterEdit);
    }

    onOperationChange(0);
}

void FilterView::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->getParameterSet(parameterSet);
    }
}

void FilterView::onFilterClicked()
{
    const int filterId = mOperationComboBox->currentIndex();

    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->rememberCurrentSettings();
    }

    AbstractByteArrayFilterParameterSet* parameterSet = mTool->parameterSet(filterId);
    if (parameterSet) {
        getParameterSet(parameterSet);
    }

    mTool->filter(filterId);
}

void FilterView::onOperationChange(int index)
{
    QWidget* oldWidget = mParameterSetEditStack->currentWidget();
    if (oldWidget) {
        oldWidget->disconnect(this);
    }

    mParameterSetEditStack->setCurrentIndex(index);

    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        connect(parametersetEdit, &AbstractByteArrayFilterParameterSetEdit::validityChanged,
                this, &FilterView::onValidityChanged);
        onValidityChanged(parametersetEdit->isValid());
    }
}

void FilterView::onHasWriteableChanged(bool hasWriteable)
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    const bool isValid = (parametersetEdit ? parametersetEdit->isValid() : false);

    mFilterButton->setEnabled(hasWriteable && isValid);
}

void FilterView::onCharCodecChanged(const QString& charCodecName)
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->setCharCodec(charCodecName);
    }
}

void FilterView::onValidityChanged(bool isValid)
{
    mFilterButton->setEnabled(mTool->hasWriteable() && isValid);
}

}
