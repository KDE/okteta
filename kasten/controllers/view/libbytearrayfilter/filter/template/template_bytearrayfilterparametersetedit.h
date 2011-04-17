/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTERWITHPARAMETERSETEDIT_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTERWITHPARAMETERSETEDIT_H"
#ifndef TEMPLATE_BYTEARRAYFILTERPARAMETERSETEDIT_H
#define TEMPLATE_BYTEARRAYFILTERPARAMETERSETEDIT_H
//// ADAPT(end)

// lib
#include <abstractbytearrayfilterparametersetedit.h>

//// ADAPT(start)
//// add here all needed headers and forward declarations for the members of the widget
class KIntNumInput;
//// ADAPT(end)


//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilterParameterSetEdit to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilterParameterSetEdit
class Template_ByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
//// ADAPT(end)
{
  Q_OBJECT

  public:
    static const char* const Id;

  public:
    Template_ByteArrayFilterParameterSetEdit( QWidget* parent = 0 );

    virtual ~Template_ByteArrayFilterParameterSetEdit();

  public: // AbstractByteArrayFilterParameterSetEdit API
    virtual void setValues( const AbstractByteArrayFilterParameterSet* parameterSet );
    virtual void getParameterSet( AbstractByteArrayFilterParameterSet* parameterSet ) const;
//// ADAPT(start)
//// if the values in the widget can reach invalid states, which should not be used for the parameters,
//// keep the declaration of "isValid()", otherwise remove the next line
    virtual bool isValid() const;
//// ADAPT(end)

//// ADAPT(start)
//// define slots to catch changes in the values to check if the current state is valid or not
//// not needed if there cannot be invalid states
  protected Q_SLOTS:
    void onLevelChanged( int value );
//// ADAPT(end)

//// ADAPT(start)
//// here list the UI elements as usual
  protected:
    KIntNumInput* mLevelEdit;

    bool mIsValid :1;
//// ADAPT(end)
};

#endif
