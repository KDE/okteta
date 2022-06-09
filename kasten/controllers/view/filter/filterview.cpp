/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// utils
#include <labelledtoolbarwidget.hpp>
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QAction>
#include <QGroupBox>
#include <QAbstractItemView>
#include <QIcon>

namespace Kasten {

FilterView::FilterView(FilterTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // filter selection
    auto* filterSelectionToolBar = new QToolBar(this);
    auto* label = new QLabel(i18nc("@label:listbox operation to use by the filter", "Operation:"), this);
    mOperationComboBox = new KComboBox(this);
    connect(mOperationComboBox, qOverload<int>(&KComboBox::activated),
            mTool, &FilterTool::setFilter);

    auto* labelledAlgorithmComboBox = new LabelledToolBarWidget(label, mOperationComboBox, this);
    const QString operationToolTip =
        i18nc("@info:tooltip", "The operation to use for the filter.");
    label->setToolTip(operationToolTip);
    mOperationComboBox->setToolTip(operationToolTip);
    const QString operationWhatsThis =
        i18nc("@info:whatsthis", "Select the operation to use for the filter.");
    label->setWhatsThis(operationWhatsThis);
    mOperationComboBox->setWhatsThis(operationWhatsThis);
    mOperationComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    filterSelectionToolBar->addWidget(labelledAlgorithmComboBox);
    baseLayout->addWidget(filterSelectionToolBar);

    auto* parameterSetBox = new QGroupBox(i18nc("@title:group", "Parameters"), this);
    baseLayout->addWidget(parameterSetBox);

    auto* parameterSetLayout = new QVBoxLayout;

    parameterSetBox->setLayout(parameterSetLayout);

    mParameterSetEditStack = new QStackedWidget(parameterSetBox);

    parameterSetLayout->addWidget(mParameterSetEditStack);

    // filter button
    auto* actionToolBar = new QToolBar(this);
    actionToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionToolBar->addWidget(stretcher);

    mFilterAction =
        actionToolBar->addAction(QIcon::fromTheme(QStringLiteral("run-build")),
                                 i18nc("@action:button", "&Filter"),
                                 this, &FilterView::onFilterClicked);
    mFilterAction->setToolTip(i18nc("@info:tooltip",
                                    "Executes the filter for the bytes in the selected range."));
    mFilterAction->setWhatsThis(xi18nc("@info:whatsthis",
                                       "If you press the <interface>Filter</interface> button, the operation you selected "
                                       "above is executed for the bytes in the selected range with the given options."));
    mFilterAction->setEnabled(mTool->hasWriteable());
    connect(mTool, &FilterTool::filterChanged, this, &FilterView::onFilterChanged);
    connect(mTool, &FilterTool::hasWriteableChanged, this, &FilterView::onHasWriteableChanged);
    connect(mTool, &FilterTool::charCodecChanged, this, &FilterView::onCharCodecChanged);
    baseLayout->addWidget(actionToolBar);
    baseLayout->addStretch(10);

    // automatically set focus to the parameters if a operation has been selected
    QAbstractItemView* operationComboBoxListView = mOperationComboBox->view();
    QObject::connect(operationComboBoxListView, &QAbstractItemView::activated,
                     mParameterSetEditStack, qOverload<>(&QStackedWidget::setFocus));
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect(operationComboBoxListView, &QAbstractItemView::pressed,
                     mParameterSetEditStack, qOverload<>(&QStackedWidget::setFocus));
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

    onFilterChanged(mTool->filterId());
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
    const int filterId = mTool->filterId();

    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->rememberCurrentSettings();
    }

    AbstractByteArrayFilterParameterSet* parameterSet = mTool->parameterSet(filterId);
    if (parameterSet) {
        getParameterSet(parameterSet);
    }

    mTool->filter();
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
        AbstractByteArrayFilterParameterSet* parameterSet = mTool->parameterSet(index);
        if (parameterSet) {
            parametersetEdit->setValues(parameterSet);
        }
        connect(parametersetEdit, &AbstractByteArrayFilterParameterSetEdit::validityChanged,
                this, &FilterView::onValidityChanged);
        connect(parametersetEdit, &AbstractByteArrayFilterParameterSetEdit::valuesChanged,
                this, &FilterView::onValuesChanged);
        onValidityChanged(parametersetEdit->isValid());
    }
}

void FilterView::onFilterChanged(int filterId)
{
    mOperationComboBox->setCurrentIndex(filterId);
    onOperationChange(filterId);
}

void FilterView::onHasWriteableChanged(bool hasWriteable)
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    const bool isValid = (parametersetEdit ? parametersetEdit->isValid() : false);

    mFilterAction->setEnabled(hasWriteable && isValid);
}

void FilterView::onCharCodecChanged(const QString& charCodecName)
{
    auto* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>(mParameterSetEditStack->currentWidget());
    if (parametersetEdit) {
        parametersetEdit->setCharCodec(charCodecName);
    }
}

void FilterView::onValuesChanged()
{
    const int filterId = mOperationComboBox->currentIndex();
    AbstractByteArrayFilterParameterSet* parameterSet = mTool->parameterSet(filterId);
    if (!parameterSet) {
        return;
    }
    getParameterSet(parameterSet);
    mTool->saveParameterSet(filterId);
}

void FilterView::onValidityChanged(bool isValid)
{
    mFilterAction->setEnabled(mTool->hasWriteable() && isValid);
}

}
