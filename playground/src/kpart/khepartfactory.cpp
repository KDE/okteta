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

// Part
static const char *PartId =          "khexeditpart";
static const char *PartName =        I18N_NOOP("KHexEditPart");
static const char *PartDescription = I18N_NOOP("Embedded hex editor");
static const char *PartVersion =     "0.1.0";
static const char *PartCopyright =   "(C) 2003 Friedrich W. H.  Kossebau";
// Author
static const char *FWHKName =         "Friedrich W. H. Kossebau";
static const char *FWHKTask =         I18N_NOOP("Author");
static const char *FWHKEmailAddress = "Friedrich.W.H@Kossebau.de";
// static const char *FWHKWebAddress = "http://www.kossebau.de";
// Credit to
static const char *ESName =         "Espen Sand";
static const char *ESEmailAddress = "espensa@online.no";
// static const char *ESWebAddress = "http://home.sol.no/~espensa/khexedit/";
static const char *ESCreditText =   I18N_NOOP( "wrote the KHexEdit app" );



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

  s_instance = 0L;
}


KParts::Part* KHexEditPartFactory::createPartObject( QWidget *ParentWidget, const char *WidgetName,
                                                     QObject *Parent, const char *Name,
                                                     const char *ClassName, const QStringList &/*args*/ )
{
  // Create an instance of our Part
  KHexEditPart* HexEditPart = new KHexEditPart( ParentWidget, WidgetName, Parent, Name );

  // See if we are to be read-write or not
  if( QCString(ClassName) == "KParts::ReadOnlyPart" )
    HexEditPart->setReadWrite( false );

  return HexEditPart;
}


KInstance* KHexEditPartFactory::instance()
{
  if( !s_instance )
  {
    s_about = new KAboutData( PartId, PartName, PartVersion, PartDescription,
                              KAboutData::License_GPL_V2, PartCopyright, 0, 0, FWHKEmailAddress );
    s_about->addAuthor( FWHKName, FWHKTask, FWHKEmailAddress );
    s_about->addCredit( ESName, ESCreditText, ESEmailAddress );
    s_instance = new KInstance( s_about );
  }
  return s_instance;
}


extern "C"
{
  void* init_libkhexeditpart()
  {
    return new KHexEditPartFactory;
  }
};
