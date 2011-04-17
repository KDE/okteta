/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSETEDIT_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYCHECKSUMPARAMETERSETEDIT_H"
#ifndef TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSETEDIT_H
#define TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSETEDIT_H
//// ADAPT(end)

// lib
#include "abstractbytearraychecksumparametersetedit.h"

//// ADAPT(start)
//// add here all needed headers and forward declarations for the members of the widget
class KIntNumInput;
//// ADAPT(end)


//// ADAPT(start)
//// rename the class name from Template_ByteArrayChecksumParameterSetEdit to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayChecksumParameterSetEdit
class Template_ByteArrayChecksumParameterSetEdit : public AbstractByteArrayChecksumParameterSetEdit
//// ADAPT(end)
{
  Q_OBJECT

  public:
    static const char* const Id;

  public:
    Template_ByteArrayChecksumParameterSetEdit( QWidget* parent = 0 );

    virtual ~Template_ByteArrayChecksumParameterSetEdit();

  public: // AbstractByteArrayFilterParameterSetEdit API
    virtual void setParameterSet( const AbstractByteArrayChecksumParameterSet* parameterSet );
    virtual void getParameterSet( AbstractByteArrayChecksumParameterSet* parameterSet ) const;
//// ADAPT(start)
//// if the values in the widget can reach invalid states, which should not be used for the parameters,
//// keep the declaration of "isValid()", otherwise remove the next line
    virtual bool isValid() const;
//// ADAPT(end)

//// ADAPT(start)
//// define slots to catch changes in the values to check if the current state is valid or not
//// not needed if there cannot be invalid states
  protected Q_SLOTS:
    void onBitNumberChanged( int value );
//// ADAPT(end)

//// ADAPT(start)
//// here list the UI elements as usual
  protected:
    KIntNumInput* mBitNumberEdit;

    bool mIsValid :1;
//// ADAPT(end)
};

#endif
