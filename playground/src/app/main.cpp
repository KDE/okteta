/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Tue Jul 29 2003
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
#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
// app specific
#include "khexedit2.h"

// App
static const char *AppId =          "khexedit2";
static const char *AppName =        I18N_NOOP("KHexEdit2");
static const char *AppDescription = I18N_NOOP("KDE Hex editor");
static const char *AppVersion =     "0.1.5";
static const char *AppCopyright =   "(C) 2003 Friedrich W. H. Kossebau";
// Author
static const char *FWHKName =         "Friedrich W. H. Kossebau";
static const char *FWHKTask =         I18N_NOOP("Reimplementer");
static const char *FWHKEmailAddress = "Friedrich.W.H@Kossebau.de";
// static const char *FWHKWebAddress = "http://www.kossebau.de";
// Credit to
static const char *ESName =         "Espen Sand";
static const char *ESEmailAddress = "espensa@online.no";
// static const char *ESWebAddress = "http://home.sol.no/~espensa/khexedit/";
static const char *ESCreditText =   I18N_NOOP("wrote the first KHexEdit app (1999 until 200x)");

//     "This program uses modified code and techniques from other KDE programs,\n"
//     "specifically kwrite, kiconedit and ksysv. Credit goes to authors\n"
//     "and maintainers.\n"
//     "\n"
//     "Leon Lessing, leon@lrlabs.com, has made parts of the bit swapping\n"
//     "functionality.\n"
//     "\n"
//     "Craig Graham, c_graham@hinge.mistral.co.uk, has made parts of\n"
//     "the bit stream functionality of the conversion field.\n"
//     "\n"
//     "Dima Rogozin, dima@mercury.co.il, has extended the string dialog\n"
//     "list capabilities.\n"
//     "\n"
//     "Edward Livingston-Blade, sbcs@bigfoot.com, has given me very good\n"
//     "reports which removed some nasty bugs.\n"));


static KCmdLineOptions AppOptions[] =
{
  { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
  { 0, 0, 0 }
};


int main( int argc, char **argv )
{
  KAboutData AboutData( AppId, AppName, AppVersion, AppDescription,
                        KAboutData::License_GPL_V2, AppCopyright, 0, 0, FWHKEmailAddress );
  AboutData.addAuthor( FWHKName, FWHKTask, FWHKEmailAddress );

  AboutData.addCredit( ESName, ESCreditText, ESEmailAddress );

  KCmdLineArgs::init( argc, argv, &AboutData );
  KCmdLineArgs::addCmdLineOptions( AppOptions );
  KApplication App;

  // register ourselves as a dcop client
  App.dcopClient()->registerAs( App.name(), false );

  // see if we are starting with session management
  if( App.isRestored() )
  {
    RESTORE(KHexEdit2);
  }
  else
  {
    // no session.. just start up normally
    KCmdLineArgs *Args = KCmdLineArgs::parsedArgs();
    if( Args->count() == 0 )
    {
      KHexEdit2 *Widget = new KHexEdit2;
      Widget->show();
    }
    else
    {
      for( int i=0; i<Args->count(); i++ )
      {
        KHexEdit2 *Widget = new KHexEdit2;
        Widget->show();
        Widget->load( Args->url(i) );
      }
    }
    Args->clear();
  }

  return App.exec();
}
