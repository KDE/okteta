/***************************************************************************
                          partfactory.h  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "partfactory.h"

// part
#include "part.h"
// KDE
#include <kcomponentdata.h>
#include <kaboutdata.h>
#include <klocale.h>


// Part
static const char PartId[] =          "oktetapart";
static const char PartName[] =        I18N_NOOP("OktetaPart");
static const char PartDescription[] = I18N_NOOP("Embedded hex editor");
static const char PartVersion[] =     "0.4.0";
static const char PartCopyright[] =   "2003-2007 Friedrich W. H. Kossebau";
// Author
static const char FWHKName[] =         "Friedrich W. H. Kossebau";
static const char FWHKTask[] =         I18N_NOOP("Author");
static const char FWHKEmailAddress[] = "kossebau@kde.org";


static KComponentData *_componentData = 0;
static KAboutData* _aboutData = 0;


OktetaPartFactory::OktetaPartFactory()
 : KParts::Factory()
{
}


OktetaPartFactory::~OktetaPartFactory()
{
    delete _componentData;
    delete _aboutData;

    _componentData = 0;
}


KParts::Part* OktetaPartFactory::createPartObject( QWidget *parentWidget,
                                                   QObject *parent,
                                                   const char *cn, const QStringList &/*args*/ )
{
    const QByteArray classname( cn );
    const bool browserViewWanted = ( classname == "Browser/View" );
    //bool ReadOnlyWanted = (BrowserViewWanted || ( Classname == "KParts::ReadOnlyPart" ));

    OktetaPart* part = new OktetaPart( parentWidget, parent, browserViewWanted );

    return part;
}


const KComponentData &OktetaPartFactory::componentData()
{
    if( !_componentData )
    {
        _aboutData = new KAboutData( PartId, 0, ki18n(PartName), PartVersion, ki18n(PartDescription),
                                    KAboutData::License_GPL_V2, ki18n(PartCopyright), KLocalizedString(), 0, FWHKEmailAddress );
        _aboutData->addAuthor( ki18n(FWHKName), ki18n(FWHKTask), FWHKEmailAddress );
        _componentData = new KComponentData( _aboutData );
    }
    return *_componentData;
}


K_EXPORT_COMPONENT_FACTORY( liboktetapart, OktetaPartFactory )

#include "partfactory.moc"
