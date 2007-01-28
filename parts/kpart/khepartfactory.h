/***************************************************************************
                          khepartfactory.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHEPARTFACTORY_H
#define KHEPARTFACTORY_H

// kde specific
#include <kparts/factory.h>

class KComponentData;
class KAboutData;


class KHexEditPartFactory : public KParts::Factory
{
    Q_OBJECT

  public:
    KHexEditPartFactory();
    virtual ~KHexEditPartFactory();

  public:
    virtual KParts::Part* createPartObject( QWidget *parentWidget,
                                            QObject *parent,
                                            const char *classname, const QStringList &args );
    static const KComponentData &componentData();


  private:
    static KComponentData *s_instance;
    static KAboutData* s_about;
};

#endif
