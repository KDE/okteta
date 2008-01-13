/***************************************************************************
                          abstractbytearrayfilter.h  -  description
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


#ifndef ABSTRACTBYTEARRAYFILTER_H
#define ABSTRACTBYTEARRAYFILTER_H

class AbstractByteArrayFilterParameterSet;
namespace KHECore {
class KAbstractByteArrayModel;
}
namespace KHE {
class KSection;
}
class QString;


class AbstractByteArrayFilter
{
  protected:
    explicit AbstractByteArrayFilter( const QString &name );
  public:
    virtual ~AbstractByteArrayFilter();

  public: // API to be implemented
    virtual bool filter( char *result, KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const = 0;
    /** used by the editor to get write access to the parameters */
    virtual AbstractByteArrayFilterParameterSet *parameterSet() = 0;

  public:
    QString name() const;

  protected:
    class Private;
    Private * const d;
};

#endif
