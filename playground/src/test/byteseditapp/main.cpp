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

 
#include "byteseditapp.h"
#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char *description =
    I18N_NOOP("A KDE Application");

static const char *version = "0.1";


int main(int argc, char **argv)
{
    KAboutData about("byteseditapp", I18N_NOOP("BytesEditApp"), version, description,
                     KAboutData::License_GPL, "(C) 2003 Friedrich W. H.  Kossebau", 0, 0, "Friedrich.W.H@Kossebau.de");
    about.addAuthor( "Friedrich W. H.  Kossebau", 0, "Friedrich.W.H@Kossebau.de" );
    KCmdLineArgs::init(argc, argv, &about);
    KApplication app;

    // register ourselves as a dcop client
    app.dcopClient()->registerAs(app.name(), false);

    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(BytesEditApp);
    }
    else
    {
        // no session.. just start up normally
//         KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        BytesEditApp *widget = new BytesEditApp;
        widget->show();

//         args->clear();
    }

    return app.exec();
}
