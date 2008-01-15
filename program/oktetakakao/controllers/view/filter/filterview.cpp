/***************************************************************************
                          filterview.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "filterview.h"

// 
#include "filtertool.h"
#include <abstractbytearrayfilterparametersetedit.h>
// KDE
#include <KPushButton>
#include <KDialog>
#include <KLocale>
#include <KComboBox>
#include <KGuiItem>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QStackedWidget>
#include <QtGui/QGroupBox>



FilterView::FilterView( FilterTool *tool, QWidget *parent )
  : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setSpacing( KDialog::spacingHint() );
    baseLayout->setMargin( 0 );

    // filter
    QHBoxLayout *operationLayout = new QHBoxLayout();
    QLabel *label = new QLabel( i18n("Operation:"), this );
    mOperationComboBox = new KComboBox( this );
    connect( mOperationComboBox, SIGNAL(activated(int)),
             SLOT(onOperationChange(int)) );

    label->setBuddy( mOperationComboBox );
    const QString numberWhatsThis =
        i18n("Select the operation to use for the filter.");
    label->setWhatsThis( numberWhatsThis );
    mOperationComboBox->setWhatsThis( numberWhatsThis );

    operationLayout->addWidget( label );
    operationLayout->addWidget( mOperationComboBox, 10 );
    baseLayout->addLayout( operationLayout );

    QGroupBox *parameterSetBox = new QGroupBox( i18n("Parameters"), this );
    baseLayout->addWidget( parameterSetBox );

    QVBoxLayout *parameterSetLayout = new QVBoxLayout;
    parameterSetLayout->setSpacing( KDialog::spacingHint() );

    parameterSetBox->setLayout( parameterSetLayout );

    mParameterSetEditStack = new QStackedWidget( parameterSetBox );

    parameterSetLayout->addWidget( mParameterSetEditStack );

    // filter button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch( 10 );
    mFilterButton = new KPushButton( KGuiItem(i18n("&Filter"), QString(),
                      i18n("Executes the filter"),
                      i18n("<qt>If you press the <b>Filter</b> button, the operation you selected "
                           "above is executed on the document with the given options.</qt>")), this );
    mFilterButton->setEnabled( mTool->hasView() );
    connect( mTool, SIGNAL(viewChanged( bool )), SLOT(onViewChanged( bool )) );
    connect( mFilterButton, SIGNAL(clicked( bool )), SLOT(onFilterClicked()) );
    buttonLayout->addWidget( mFilterButton );
    baseLayout->addLayout( buttonLayout );
    baseLayout->addStretch( 10 );
}

void FilterView::addParameterEdit( const QString &name, QWidget *parameterEdit )
{
    mOperationComboBox->addItem( name );
    mParameterSetEditStack->addWidget( parameterEdit );
}

void FilterView::setCharCode( const QString &codeName )
{
//     mOperandEdit->setCharCode( codeName ); // TODO: simply tell all editors a view change?
}

int FilterView::filterId() const { return mOperationComboBox->currentIndex(); }
void FilterView::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
        parametersetEdit->getParameterSet( parameterSet );
}

void FilterView::onFilterClicked()
{
    int filterId = mOperationComboBox->currentIndex();

    AbstractByteArrayFilterParameterSet *parameterSet = mTool->parameterSet( filterId );
    if( parameterSet )
        getParameterSet( parameterSet );

    mTool->filter( filterId );
}


void FilterView::onOperationChange( int index )
{
    QWidget *oldWidget = mParameterSetEditStack->currentWidget();
    if( oldWidget )
        oldWidget->disconnect( this );

    mParameterSetEditStack->setCurrentIndex( index );

    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
    {
        connect( parametersetEdit, SIGNAL(validityChanged( bool )),
                 SLOT(onValidityChanged( bool )) );
        onValidityChanged( parametersetEdit->isValid() );
    }
}

void FilterView::onViewChanged( bool hasView )
{
    AbstractByteArrayFilterParameterSetEdit *parametersetEdit =
        qobject_cast<AbstractByteArrayFilterParameterSetEdit *>( mParameterSetEditStack->currentWidget() );
    const bool isValid = ( parametersetEdit ? parametersetEdit->isValid() : false );

    mFilterButton->setEnabled( hasView && isValid );
}

void FilterView::onValidityChanged( bool isValid )
{
    mFilterButton->setEnabled( mTool->hasView() && isValid );
}

FilterView::~FilterView() {}
