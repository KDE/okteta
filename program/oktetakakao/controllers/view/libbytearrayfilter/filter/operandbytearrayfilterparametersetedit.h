/***************************************************************************
                          operandbytearrayfilterparametersetedit.h  -  description
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


#ifndef OPERANDBYTEARRAYFILTERPARAMETERSETEDIT_H
#define OPERANDBYTEARRAYFILTERPARAMETERSETEDIT_H


// lib
#include "abstractbytearrayfilterparametersetedit.h"

class KByteArrayLineEdit;
class QCheckBox;

class OperandByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
  Q_OBJECT

  public:
    explicit OperandByteArrayFilterParameterSetEdit( QWidget *parent = 0 );
    virtual ~OperandByteArrayFilterParameterSetEdit();

  public: // AbstractByteArrayFilterParameterSetEdit API
    virtual void setValues( const AbstractByteArrayFilterParameterSet *parameterSet );
    virtual void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const;
    virtual bool isValid() const;

  protected Q_SLOTS:
    void onInputChanged( const QByteArray &data );

  protected:
    KByteArrayLineEdit *mOperandEdit;
    QCheckBox *mAlignAtEndCheckBox;
};

#endif
