/***************************************************************************
                          khebrowserextension.cpp  -  description
                             -------------------
    begin                : Di Nov 16 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// app specific
#include "khexedit.h"
#include "khepart.h"
#include "khebrowserextension.h"

using namespace KHE;


KHexEditBrowserExtension::KHexEditBrowserExtension( KHexEditPart *P, KHexEdit* HE )
  : KParts::BrowserExtension( P, "khexeditpartbrowserextension" ),
  HexEdit( HE )
{
  connect( HexEdit, SIGNAL( selectionChanged() ), this, SLOT( slotSelectionChanged() ) );
}

void KHexEditBrowserExtension::copy()
{
  HexEdit->copy();
}


void KHexEditBrowserExtension::slotSelectionChanged()
{
  emit enableAction( "copy", HexEdit->hasSelectedData() );
}
