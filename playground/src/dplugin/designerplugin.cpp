/***************************************************************************
                          designerplugin.cpp  -  description
                             -------------------
    begin                : Mon Jul 21 2003
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
#include <qwidgetplugin.h>
// kde specific
#include <kstandarddirs.h> // for pixmap search
// app specific
#include "kbytesedit.h"

class KBytesEditPlugin : public QWidgetPlugin
{
  public:
    KBytesEditPlugin();

    QStringList keys() const;
    QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 );
    QString group( const QString& ) const;
    QIconSet iconSet( const QString& ) const;
    QString includeFile( const QString& ) const;
    QString toolTip( const QString& ) const;
    QString whatsThis( const QString& ) const;
    bool isContainer( const QString& ) const;
};

static const char KBytesEditId[] = "KHE::KBytesEdit";
static const char Group[] =        "Views (KDE)";
static const char ToolTip[] =      "Hexadecimal Edit Widget (KDE)";
static const char WhatsThis[] =    "A widget to show and/or edit data both in hexadecimal and ASCII";
static const char IncludeFile[] =  "kbytesedit.h";
static const char PathToIcon[] =   "khexedit2/pics/kbytesedit.png";

static const char ExampleDataTemplate[] =
  "This is just some plain dummy data "
  "to fill the dummy widget. "
  "A tab:\t, a newline:\n, some other values \23\2\4\26... "
  "In the middle column you can see the hexadecimal values of the chars in this column. "
  "Great, isn't it? Blablabla... is this enough data? ;) "
  "Oh wait, I have got a good phrase: "
  "\"KDE - using, not attending!\" "
  "And another one: "
  "\"KDE - made by programmers and users, not sales men!\"";

static const int ExampleDataSize = sizeof(ExampleDataTemplate)/sizeof(char);


KBytesEditPlugin::KBytesEditPlugin()
{
}

// return a list of availables widget types
QStringList KBytesEditPlugin::keys() const
{
    QStringList list;
    list << KBytesEditId;
    return list;
}

// return an instance of the widget
QWidget* KBytesEditPlugin::create( const QString &Key, QWidget* Parent, const char* Name )
{
  if( Key == KBytesEditId )
  {
    char *ExampleData = new char[ExampleDataSize];
    memcpy( ExampleData, ExampleDataTemplate, ExampleDataSize );

    KHE::KBytesEdit *BytesEdit = new KHE::KBytesEdit( ExampleData, ExampleDataSize, Parent, Name );
    BytesEdit->setMaxDataSize( ExampleDataSize );
    BytesEdit->setReadOnly( false );
    BytesEdit->setAutoDelete( true );
    return BytesEdit;
  }
  return 0;
}

// returns the name of the include file needed for the widget
QString KBytesEditPlugin::includeFile( const QString& Feature ) const
{
  if( Feature == KBytesEditId )
    return IncludeFile;
  return QString::null;
}

// return to which group the widget belongs
QString KBytesEditPlugin::group( const QString& Feature ) const
{
  if ( Feature == KBytesEditId )
    return Group;
  return QString::null;
}

// returns an icon for the widget
QIconSet KBytesEditPlugin::iconSet( const QString& ) const
{
  QString Path = locate( "data", PathToIcon );
  return QIconSet( Path );
}

// returns a tooltip for the
QString KBytesEditPlugin::toolTip( const QString& Feature ) const
{
  if( Feature == KBytesEditId )
    return ToolTip;
  return QString::null;
}

// returns the
QString KBytesEditPlugin::whatsThis( const QString& Feature ) const
{
  if( Feature == KBytesEditId )
    return WhatsThis;
  return QString::null;
}

// return whether this is a container
bool KBytesEditPlugin::isContainer( const QString& ) const
{
  return false;
}


Q_EXPORT_PLUGIN( KBytesEditPlugin )
