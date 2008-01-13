/***************************************************************************
                          abstractbytearrayfilterparameterset.h  -  description
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


#ifndef ABSTRACTBYTEARRAYFILTERPARAMETERSET_H
#define ABSTRACTBYTEARRAYFILTERPARAMETERSET_H


class AbstractByteArrayFilterParameterSet
{
  protected:
    AbstractByteArrayFilterParameterSet();
  public:
    virtual ~AbstractByteArrayFilterParameterSet();

  protected:
    class Private;
    Private * const d;
};

#endif
