/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearraychecksumparametersetedit.h" to the name of the header of your filter,
//// e.g. "mybytearraychecksumparametersetedit.h"
#include "template_bytearraychecksumparametersetedit.h"
//// ADAPT(end)

// parameterset
//// ADAPT(start)
//// rename "template_bytearraychecksumparameterset.h" to the name of the header of your filter,
//// e.g. "mybytearraychecksumparameterset.h"
#include "template_bytearraychecksumparameterset.h"
//// ADAPT(end)
// KDE
#include <KLocale>
//// ADAPT(start)
//// add includes for all elements used in the widget
#include <KIntNumInput>
// Qt
#include <QtGui/QFormLayout>
//// ADAPT(end)


//// ADAPT(start)
//// rename "Template_ParameterSetId" to the id of your parameterset,
//// e.g. "MyParameterSet"
const char* const Template_ByteArrayChecksumParameterSetEdit::Id = "Template_ParameterSetId";
//// ADAPT(end)


Template_ByteArrayChecksumParameterSetEdit::Template_ByteArrayChecksumParameterSetEdit( QWidget* parent )
  : AbstractByteArrayChecksumParameterSetEdit( parent )
{
//// ADAPT(start)
//// setup the widget with all edit fields needed for the parameter set
//// if there can be invalid states connect the change signals of the edit fields to some slots
//// where you check if the validity changed
    QFormLayout* baseLayout = new QFormLayout( this );
    // margin is provided by the container for this widget
    baseLayout->setMargin( 0 );

    mBitNumberEdit = new KIntNumInput( this );
    // For demonstration purpose we start at 0, not 1, to show handling of an invalid state
    // Otherwise the range should start at 1 and there is no need to connect to the valueChanged signal
    mBitNumberEdit->setRange( 0, 8 );
    // start with the invalid number
    mBitNumberEdit->setValue( 0 );
    connect( mBitNumberEdit, SIGNAL(valueChanged(int)),
             SLOT(onBitNumberChanged(int)) );

    const QString levelLabelText =
         i18nc( "@label:spinbox number of the bit to use",
                "Number of bit:" );
    const QString levelToolTip =
        i18nc( "@info:tooltip",
               "The number of the bit to use for the parity calculation. 1 means the LSB, 8 the MSB." );
    mBitNumberEdit->setToolTip( levelToolTip );
    const QString levelWhatsThis =
        i18nc( "@info:whatsthis",
               "Select the bit which should be used for the parity calculation. And more explanation." );
    mBitNumberEdit->setWhatsThis( levelWhatsThis );

    baseLayout->addRow( levelLabelText, mBitNumberEdit );

    // note start state
    mIsValid = isValid();
//// ADAPT(end)
}

//// ADAPT(start)
//// if invalid states are possible implement here the check
bool Template_ByteArrayChecksumParameterSetEdit::isValid() const { return mBitNumberEdit->value() != 0; }
//// ADAPT(end)


//// ADAPT(start)
//// change "Template_ByteArrayFilterParameterSet" to the class of the parameter set which this widget should edit
//// e.g. "MyByteArrayFilterParameterSet"
//// also adapt the passing of the values between the parameter set and the edit fields
void Template_ByteArrayChecksumParameterSetEdit::setParameterSet( const AbstractByteArrayChecksumParameterSet* parameterSet )
{
    const Template_ByteArrayChecksumParameterSet* template_ParameterSet =
        static_cast<const Template_ByteArrayChecksumParameterSet *>( parameterSet );

    mBitNumberEdit->setValue( template_ParameterSet->bitNumber() );
}

void Template_ByteArrayChecksumParameterSetEdit::getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const
{
    Template_ByteArrayChecksumParameterSet* template_ParameterSet =
        static_cast<Template_ByteArrayChecksumParameterSet *>( parameterSet );

    template_ParameterSet->setBitNumber( mBitNumberEdit->value() );
}
//// ADAPT(end)


//// ADAPT(start)
//// define the slots to catch changes in the values to check if the current state is valid or not
//// not needed if there cannot be invalid states
void Template_ByteArrayChecksumParameterSetEdit::onBitNumberChanged( int value )
{
    const bool isValid = ( value != 0 );

    if( mIsValid == isValid )
        return;

    mIsValid = isValid;
    emit validityChanged( isValid );
}
//// ADAPT(end)

Template_ByteArrayChecksumParameterSetEdit::~Template_ByteArrayChecksumParameterSetEdit() {}
