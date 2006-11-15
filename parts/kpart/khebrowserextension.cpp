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


// kde specific
#include <kaction.h>
// uilib specific
#include <khexedit.h>
// part specific
#include "khepart.h"
#include "khebrowserextension.h"


KHexEditBrowserExtension::KHexEditBrowserExtension( KHexEditPart *P )
  : KParts::BrowserExtension( P ),
  HexEditPart( P )
{
  setObjectName( "khexeditpartbrowserextension" );
  connect( HexEditPart->HexEdit, SIGNAL( selectionChanged( bool ) ), SLOT( onSelectionChanged( bool ) ) );
}

void KHexEditBrowserExtension::copy()
{
  HexEditPart->HexEdit->copy();
}


void KHexEditBrowserExtension::onSelectionChanged( bool HasSelection )
{
  emit enableAction( "copy", HasSelection );
}


void KHexEditBrowserExtension::saveState( QDataStream &stream )
{
  KParts::BrowserExtension::saveState( stream );

  KHEUI::KHexEdit *HexEdit = HexEditPart->HexEdit;

  stream << (int)HexEdit->offsetColumnVisible() << HexEdit->visibleBufferColumns()
      << (int)HexEdit->resizeStyle() << (int)HexEdit->coding() 
      << HexEdit->encodingName() << (int)HexEdit->showUnprintable()
      << HexEdit->xOffset() << HexEdit->yOffset()
      << HexEdit->cursorPosition() << (int)HexEdit->isCursorBehind()
      << HexEdit->cursorColumn();
}


void KHexEditBrowserExtension::restoreState( QDataStream &stream )
{
  KParts::BrowserExtension::restoreState( stream );

  int OffsetColumnVisible;
  int VisibleBufferColumns;
  int ResizeStyle;
  int Coding;
  QString EncodingName;
  int ShowUnprintable;
  int x, y;
  int Position;
  int CursorBehind;
  int CursorColumn;

  stream >> OffsetColumnVisible >> VisibleBufferColumns >> ResizeStyle >> Coding >> EncodingName >> ShowUnprintable 
         >> x >> y >> Position >> CursorBehind >> CursorColumn;

  KHEUI::KHexEdit *HexEdit = HexEditPart->HexEdit;

  HexEdit->toggleOffsetColumn( OffsetColumnVisible );
  HexEdit->showBufferColumns( VisibleBufferColumns );
  HexEdit->setResizeStyle( (KHEUI::KHexEdit::KResizeStyle)ResizeStyle );
  HexEdit->setCoding( (KHEUI::KHexEdit::KCoding)Coding );
  HexEdit->setEncoding( EncodingName );
  HexEdit->setShowUnprintable( ShowUnprintable );
  HexEdit->setColumnsPos( x, y );
  HexEdit->setCursorPosition( Position, CursorBehind );
  HexEdit->setCursorColumn( (KHEUI::KHexEdit::KBufferColumnId)CursorColumn );

  HexEditPart->fitActionSettings();
}

#include "khebrowserextension.moc"
