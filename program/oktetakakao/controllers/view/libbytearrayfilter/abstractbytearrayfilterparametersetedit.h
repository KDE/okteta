/***************************************************************************
                          abstractbytearrayfilterparametersetetedit.h  -  description
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


#ifndef ABSTRACTBYTEARRAYFILTERPARAMETERSETEDIT_H
#define ABSTRACTBYTEARRAYFILTERPARAMETERSETEDIT_H


// Qt
#include <QtGui/QWidget>

class AbstractByteArrayFilterParameterSet;

class AbstractByteArrayFilterParameterSetEdit : public QWidget
{
  Q_OBJECT

  protected:
    explicit AbstractByteArrayFilterParameterSetEdit( QWidget *parent = 0 );
  public:
    virtual ~AbstractByteArrayFilterParameterSetEdit();

  public: // API to be implemented
    virtual void setValues( const AbstractByteArrayFilterParameterSet *parameterSet ) = 0;
    virtual void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const = 0;
    virtual bool isValid() const;

  Q_SIGNALS:
    void validityChanged( bool isValid );

  protected:
    class Private;
    Private * const d;
};

#endif
