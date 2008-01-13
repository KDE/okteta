/***************************************************************************
                          filtertool.h  -  description
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


#ifndef FILTERTOOL_H
#define FILTERTOOL_H


// Qt
#include <QtCore/QObject>
#include <QtCore/QList>

class AbstractByteArrayFilterParameterSet;
class AbstractByteArrayFilter;
class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}
class QString;
class QByteArray;


class FilterTool : public QObject
{
  Q_OBJECT

  public:
    FilterTool();
    virtual ~FilterTool();

  public:
    void setView( KAbstractView *view );

  public:
    void addFilter( AbstractByteArrayFilter *filter );

  public:
    void filter( int filterId ) const;
    bool hasView() const;

  public:
    QString charCodecName() const;
    AbstractByteArrayFilterParameterSet *parameterSet( int filterId );

  Q_SIGNALS:
    void viewChanged( bool hasView );

  protected:
    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArrayModel;

    QList<AbstractByteArrayFilter*> mFilterList;
};

#endif
