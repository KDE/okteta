/***************************************************************************
                          partfactory.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef OKTETAPARTFACTORY_H
#define OKTETAPARTFACTORY_H


// KDE
#include <kparts/factory.h>

class KComponentData;


class OktetaPartFactory : public KParts::Factory
{
    Q_OBJECT

  public:
    OktetaPartFactory();
    virtual ~OktetaPartFactory();

  public:
    virtual KParts::Part* createPartObject( QWidget *parentWidget,
                                            QObject *parent,
                                            const char *classname, const QStringList &args );

  public:
    static const KComponentData &componentData();
};

#endif
