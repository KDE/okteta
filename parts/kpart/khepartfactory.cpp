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


// kde specific
#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>
// app specific
#include "khepart.h"
#include "khepartfactory.h"

using namespace KHE;

// Part
static const char PartId[] =          "khexedit2part";
static const char PartName[] =        I18N_NOOP("KHexEdit2Part");
static const char PartDescription[] = I18N_NOOP("Embedded hex editor");
static const char PartVersion[] =     "0.2.0";
static const char PartCopyright[] =   "(C) 2003-2004 Friedrich W. H. Kossebau";
// Author
static const char FWHKName[] =         "Friedrich W. H. Kossebau";
static const char FWHKTask[] =         I18N_NOOP("Author");
static const char FWHKEmailAddress[] = "Friedrich.W.H@Kossebau.de";
// static const char FWHKWebAddress[] = "http://www.kossebau.de";


KInstance*  KHexEditPartFactory::s_instance = 0L;
KAboutData* KHexEditPartFactory::s_about = 0L;


KHexEditPartFactory::KHexEditPartFactory()
 : KParts::Factory()
{
}


KHexEditPartFactory::~KHexEditPartFactory()
{
  delete s_instance;
  delete s_about;

  s_instance = 0;
}


KParts::Part* KHexEditPartFactory::createPartObject( QWidget *ParentWidget, const char *WidgetName,
                                                     QObject *Parent, const char *Name,
                                                     const char *CN, const QStringList &/*args*/ )
{
  QCString Classname( CN );
  bool BrowserViewWanted = ( Classname == "Browser/View" );
  //bool ReadOnlyWanted = (BrowserViewWanted || ( Classname == "KParts::ReadOnlyPart" ));

  // Create an instance of our Part
  KHexEditPart* HexEditPart = new KHexEditPart( ParentWidget, WidgetName, Parent, Name, BrowserViewWanted );

  return HexEditPart;
}


KInstance* KHexEditPartFactory::instance()
{
  if( !s_instance )
  {
    s_about = new KAboutData( PartId, PartName, PartVersion, PartDescription,
                              KAboutData::License_GPL_V2, PartCopyright, 0, 0, FWHKEmailAddress );
    s_about->addAuthor( FWHKName, FWHKTask, FWHKEmailAddress );
    s_instance = new KInstance( s_about );
  }
  return s_instance;
}


K_EXPORT_COMPONENT_FACTORY( libkhexedit2part, KHexEditPartFactory )

#include "khepartfactory.moc"
