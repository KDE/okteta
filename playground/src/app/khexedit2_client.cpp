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

// qt specific
#include <qdatastream.h>
#include <qstring.h>
// kde specific
#include <kapplication.h>
#include <dcopclient.h>

int main(int argc, char **argv)
{
  KApplication app( argc, argv, "khexedit2_client", false );

  // get our DCOP client and attach so that we may use it
  DCOPClient *client = app.dcopClient();
  client->attach();

  // do a 'send' for now
  QByteArray data;
  QDataStream ds( data, IO_WriteOnly );
  if( argc > 1 )
    ds << QString(argv[1]);
  else
    ds << QString("http://www.kde.org");
  client->send( "khexedit2", "KHexEdit2Iface", "openURL(QString)", data );

  return app.exec();
}
