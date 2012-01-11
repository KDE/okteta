/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "filterview.h"

// 
#include "filtertool.h"
// filter
#include <bytearrayfilterparameterseteditfactory.h>
#include <abstractbytearrayfilterparametersetedit.h>
#include <abstractbytearrayfilterparameterset.h>
#include <abstractbytearrayfilter.h>
// KDE
#include <KPushButton>
#include <KLocale>
#include <KComboBox>
#include <KGuiItem>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QStackedWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QAbstractItemView>


namespace Kasten2
{

FilterView::FilterView( FilterTool *tool, QWidget* parent )
  : AbstractToolWidget( parent ),
    mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // filter
    QHBoxLayout *operationLayout = new QHBoxLayout();
    QLabel *label = new QLabel( i18nc("@label:listbox operation to use by the filter","Operation:"), this );
    mOperationComboBox = new KComboBox( this );
    connect( mOperationComboBox, SIGNAL(activated(int)),
             SLOT(onOperationChange(int)) );

    label->setBuddy( mOperationComboBox );
    const QString operationToolTip =
        i18nc("@info:tooltip","The operation to use for the filter.");
    label->setToolTip( operationToolTip );
    mOperationComboBox->setToolTip( operationToolTip );
    const QString operationWhatsThis =
        i18nc("@info:whatsthis","Select the operation to use for the filter.");
    label->setWhatsThis( operationWhatsThis );
    mOperationComboBox->setWhatsThis( operationWhatsThis );

    operationLayout->addWidget( label );
    operationLayout->addWidget( mOperationComboBox, 10 );
    baseLayout->addLayout( operationLayout );

    QGroupBox *parameterSetBox = new QGroupBox( i18nc("@title:group","Parameters"), this );
    baseLayout->addWidget( parameterSetBox );

    QVBoxLayout *parameterSetLayout = new QVBoxLayout;

    parameterSetBox->setLayout( parameterSetLayout );

    mParameterSetEditStack = new QStackedWidget( parameterSetBox );

    parameterSetLayout->addWidget( mParameterSetEditStack );

    // filter button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 10 );
    mFilterButton = new KPushButton( KGuiItem(i18nc("@action:button","&Filter"),
                      QLatin1String("run-build"),
                      i18nc("@info:tooltip","Executes the filter for the bytes in the selected range."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Filter</interface> button, the operation you selected "
                            "above is executed for the bytes in the selected range with the given options.")), this );
    mFilterButton->setEnabled( mTool->hasWriteable() );
    connect( mTool, SIGNAL(hasWriteableChanged(bool)), SLOT(onHasWriteableChanged(bool)) );
    connect( mTool, SIGNAL(charCodecChanged(QString)), SLOT(onCharCodecChanged(QString)) );
    connect( mFilterButton, SIGNAL(clicked(bool)), SLOT(onFilterClicked()) );
    addButton( mFilterButton, AbstractToolWidget::Default );
    buttonLayout->addWidget( mFilterButton );
    baseLayout->addLayout( buttonLayout );
    baseLayout->addStretch( 10 );

    // automatically set focus to the parameters if a operation has been selected
    QAbstractItemView* operationComboBoxListView = mOperationComboBox->view();
    QObject::connect( operationComboBoxListView, SIGNAL(activated(QModelIndex)),
             mParameterSetEditStack, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect( operationComboBoxListView, SIGNAL(pressed(QModelIndex)),
             mParameterSetEditStack, SLOT(setFocus()) );
    // TODO: goto filter button if there are no parameters

    addFilters();
}

void FilterView::addFilters()
{
    // 
    const QList<AbstractByteArrayFilter*> filterList = mTool->filterList();
    foreach( AbstractByteArrayFilter* filter, filterList )
    {
        mOperationComboBox->addItem( filter->name() );

        const char* parameterSetId = filter->parameterSet()->id();
        AbstractByteArrayFilterParameterSetEdit* parameterEdit =
            ByteArrayFilterParameterSetEditFactory::createEdit( parameterSetId );

        mParameterSetEditStack->addWidget( parameterEdit );
    }

    onOperationChange( 0 );
}

void FilterView::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
        parametersetEdit->getParameterSet( parameterSet );
}

void FilterView::onFilterClicked()
{
    const int filterId = mOperationComboBox->currentIndex();

    AbstractByteArrayFilterParameterSetEdit* parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit*>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
        parametersetEdit->rememberCurrentSettings();

    AbstractByteArrayFilterParameterSet *parameterSet = mTool->parameterSet( filterId );
    if( parameterSet )
        getParameterSet( parameterSet );

    mTool->filter( filterId );
}


void FilterView::onOperationChange( int index )
{
    QWidget* oldWidget = mParameterSetEditStack->currentWidget();
    if( oldWidget )
        oldWidget->disconnect( this );

    mParameterSetEditStack->setCurrentIndex( index );

    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
    {
        connect( parametersetEdit, SIGNAL(validityChanged(bool)),
                 SLOT(onValidityChanged(bool)) );
        onValidityChanged( parametersetEdit->isValid() );
    }
}

void FilterView::onHasWriteableChanged( bool hasWriteable )
{
    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    const bool isValid = ( parametersetEdit ? parametersetEdit->isValid() : false );

    mFilterButton->setEnabled( hasWriteable && isValid );
}

void FilterView::onCharCodecChanged( const QString &charCodecName )
{
    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
        parametersetEdit->setCharCodec( charCodecName );
}

void FilterView::onValidityChanged( bool isValid )
{
    mFilterButton->setEnabled( mTool->hasWriteable() && isValid );
}

FilterView::~FilterView() {}

}
