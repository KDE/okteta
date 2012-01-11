/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "checksumview.h"

// tool
#include "checksumtool.h"
// lib
#include <bytearraychecksumparameterseteditfactory.h>
#include <abstractbytearraychecksumparametersetedit.h>
#include <abstractbytearraychecksumparameterset.h>
#include <abstractbytearraychecksumalgorithm.h>
// KDE
#include <KComboBox>
#include <KPushButton>
#include <KLineEdit>
#include <KGuiItem>
#include <KLocale>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QLayout>
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
#include <QtGui/QAbstractItemView>


namespace Kasten2
{

ChecksumView::ChecksumView( ChecksumTool* tool, QWidget* parent )
  : AbstractToolWidget( parent ),
    mTool( tool )
{
    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // algorithm
    QHBoxLayout* algorithmLayout = new QHBoxLayout();
    QLabel* label = new QLabel( i18nc("@label:listbox algorithm to use for the checksum","Algorithm:"), this );
    mAlgorithmComboBox = new KComboBox( this );
    connect( mAlgorithmComboBox, SIGNAL(activated(int)),
             SLOT(onOperationChange(int)) );

    label->setBuddy( mAlgorithmComboBox );
    const QString algorithmWhatsThis =
        i18nc("@info:whatsthis","Select the algorithm to use for the checksum.");
    label->setWhatsThis( algorithmWhatsThis );
    mAlgorithmComboBox->setWhatsThis( algorithmWhatsThis );

    algorithmLayout->addWidget( label );
    algorithmLayout->addWidget( mAlgorithmComboBox, 10 );
    baseLayout->addLayout( algorithmLayout );

    // parameter
    QGroupBox* parameterSetBox = new QGroupBox( i18nc("@title:group","Parameters"), this );
    baseLayout->addWidget( parameterSetBox );

    QVBoxLayout* parameterSetLayout = new QVBoxLayout( parameterSetBox );

    mParameterSetEditStack = new QStackedWidget( parameterSetBox );
    parameterSetLayout->addWidget( mParameterSetEditStack );

    // calculate
    QHBoxLayout* calculateLayout = new QHBoxLayout();

    calculateLayout->addStretch();
    const KGuiItem updateGuiItem = 
        KGuiItem( i18nc("@action:button calculate the checksum","&Calculate"),
                  QLatin1String("run-build"),
                  i18nc("@info:tooltip",
                        "Calculate the checksum for the bytes in the selected range."),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Calculate</interface> button, the list will be updated "
                        "to all strings which are contained in the selected range and have the set minimum length.") );
    mCalculateButton = new KPushButton( updateGuiItem, this );
    mCalculateButton->setEnabled( mTool->isApplyable() );
    connect( mCalculateButton, SIGNAL(clicked(bool)), SLOT(onCalculateClicked()) );
    addButton( mCalculateButton, AbstractToolWidget::Default );
    calculateLayout->addWidget( mCalculateButton );
    baseLayout->addLayout( calculateLayout );

    mChecksumLabel = new KLineEdit( this );
    mChecksumLabel->setReadOnly( true );
    mChecksumLabel->setText( mTool->checkSum() );
    connect( mTool, SIGNAL(checksumChanged(QString)), mChecksumLabel, SLOT(setText(QString)) );
    baseLayout->addWidget( mChecksumLabel, 10 );

    baseLayout->addStretch( 10 );

    connect( mTool, SIGNAL(uptodateChanged(bool)), SLOT(onChecksumUptodateChanged(bool)) );
    connect( mTool, SIGNAL(isApplyableChanged(bool)), SLOT(onApplyableChanged(bool)) );

    // automatically set focus to the parameters if a operation has been selected
    QAbstractItemView* algorithmComboBoxListView = mAlgorithmComboBox->view();
    QObject::connect( algorithmComboBoxListView, SIGNAL(activated(QModelIndex)),
             mParameterSetEditStack, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect( algorithmComboBoxListView, SIGNAL(pressed(QModelIndex)),
             mParameterSetEditStack, SLOT(setFocus()) );
    // TODO: goto filter button if there are no parameters

    addAlgorithms();
}

void ChecksumView::addAlgorithms()
{
    // 
    const QList<AbstractByteArrayChecksumAlgorithm*> algorithmList = mTool->algorithmList();
    foreach( AbstractByteArrayChecksumAlgorithm* algorithm, algorithmList )
    {
        mAlgorithmComboBox->addItem( algorithm->name() );

        const char* const parameterSetId = algorithm->parameterSet()->id();
        AbstractByteArrayChecksumParameterSetEdit* parameterEdit =
            ByteArrayChecksumParameterSetEditFactory::createEdit( parameterSetId );

        mParameterSetEditStack->addWidget( parameterEdit );
    }

    onOperationChange( mTool->algorithmId() );
}

void ChecksumView::getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const
{
    AbstractByteArrayChecksumParameterSetEdit* parametersetEdit =
        qobject_cast<AbstractByteArrayChecksumParameterSetEdit*>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
        parametersetEdit->getParameterSet( parameterSet );
}

void ChecksumView::onCalculateClicked()
{
    AbstractByteArrayChecksumParameterSet* parameterSet = mTool->parameterSet();
    if( parameterSet )
        getParameterSet( parameterSet );

    mTool->calculateChecksum();
}

void ChecksumView::onOperationChange( int index )
{
    QWidget* oldWidget = mParameterSetEditStack->currentWidget();
    if( oldWidget )
    {
        oldWidget->disconnect( this );
        oldWidget->disconnect( mTool );
    }

    mTool->setAlgorithm( index );
    mParameterSetEditStack->setCurrentIndex( index );

    AbstractByteArrayChecksumParameterSetEdit* parametersetEdit =
        qobject_cast<AbstractByteArrayChecksumParameterSetEdit*>( mParameterSetEditStack->currentWidget() );
    if( parametersetEdit )
    {
        connect( parametersetEdit, SIGNAL(validityChanged(bool)),
                 SLOT(onValidityChanged(bool)) );
        // TODO: hack, see checksum source
        connect( parametersetEdit, SIGNAL(valuesChanged()),
                 mTool, SLOT(resetSourceTool()) );
        onValidityChanged( parametersetEdit->isValid() );
    }
}


void ChecksumView::onChecksumUptodateChanged( bool checksumUptodate )
{
    const bool isApplyable = mTool->isApplyable();
    mCalculateButton->setEnabled( ! checksumUptodate && isApplyable );
}

void ChecksumView::onApplyableChanged( bool isApplyable )
{
    mCalculateButton->setEnabled( !mTool->isUptodate() && isApplyable );
}

void ChecksumView::onValidityChanged( bool isValid )
{
    mCalculateButton->setEnabled( mTool->isApplyable() && isValid );
}

ChecksumView::~ChecksumView() {}

}
