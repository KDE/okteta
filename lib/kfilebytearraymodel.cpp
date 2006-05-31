/***************************************************************************
                          kfilebytearraymodel.cpp  -  description
                             -------------------
    begin                : Mit Jun 02 2003
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


// c specific
#include <stdlib.h>
// lib specific
#include "kfilebytearraymodel.h"

using namespace KHECore;

KFileByteArrayModel::KFileByteArrayModel( int NP, int PS )
 : NoOfUsedPages( NP ),
   NoOfFreePages( NP ),
   PageSize( PS ),
   FirstPage( -1 ),
   LastPage( -1 ),
   Size( 0 )
{
  IsOpen = false;

//   if( !filename.empty() )
//     open(filename);
}


KFileByteArrayModel::~KFileByteArrayModel()
{
  if( File.isOpen() )
    close();
}



const char *KFileByteArrayModel::dataSet( const KSection &/*Section*/ ) const
{
  return 0;
}


char KFileByteArrayModel::datum( unsigned int DatumOffset ) const
{
//   std::cout << "reading datum " << DatumOffset << std::endl;
  int OffsetInPage = DatumOffset - OffsetOfActualPage;
  // there shouldn't be any need to check l
  if( OffsetInPage >= 0 && OffsetInPage < (int)PageSize )
    return ActualPage[OffsetInPage];

  // load the page
  unsigned int PageIndex = DatumOffset / PageSize;
  ensurePageLoaded( PageIndex );
  return ActualPage[DatumOffset-OffsetOfActualPage];
}




int KFileByteArrayModel::insert( int /*Pos*/, const char*, int /*Length*/ )
{
  return 0;
}

int KFileByteArrayModel::remove( const KSection &/*Section*/ )
{
  return 0;
}

unsigned int KFileByteArrayModel::replace( const KSection &/*Section*/, const char*, unsigned int /*Length*/ )
{
  return 0;
}

int KFileByteArrayModel::fill( char /*FillChar*/, unsigned int /*Pos*/, int /*Length*/ )
{
  return 0;
}


int KFileByteArrayModel::move( int /*DestPos*/, const KSection &/*SourceSection*/ ) { return 0; }
//int KFileByteArrayModel::find( const char*, int /*Length*/, int /*Pos*/ ) const  { return 0; }
int KFileByteArrayModel::find( const char*/*KeyData*/, int /*Length*/, const KSection &/*Section*/ ) const { return 0; }

int KFileByteArrayModel::rfind( const char*, int /*Length*/, int /*Pos*/ ) const { return 0; }



bool KFileByteArrayModel::open( const QString& FileName )
{
  // clear old data
  if( isOpen() && !close() ) // only occurs if close somehow fails.
    return false;

  File.setFileName( FileName );
  if( !File.open(QIODevice::ReadOnly) )
    return false;

//   std::cout << "loading file " << FileName << std::endl;

  int FileSize = File.size();
  Size = FileSize;

  // calculate necessary number of pages
  int NoOfPages = FileSize/PageSize + 1;

  // initialize Page pointers
  Data.resize( NoOfPages );
  for( KPageOfChar::iterator D=Data.begin(); D!=Data.end(); ++D )
    *D = 0;

  FirstPage = LastPage = 0;

  return ensurePageLoaded( 0 );
}


bool KFileByteArrayModel::close()
{
  if( !isOpen() )
    return false;

  File.close();

  if( File.error() != QFile::NoError )
    return false;

//   std::cout << "closing file " << std::endl;

  // free pages
  for( KPageOfChar::iterator D=Data.begin(); D!=Data.end(); ++D )
    delete [] *D;

  FirstPage = LastPage = -1;
  NoOfFreePages = NoOfUsedPages;

  return true;
}


bool KFileByteArrayModel::ensurePageLoaded( unsigned int PageIndex ) const
{
  if( !isOpen() )
    return false;
  // page loaded?
  if( Data[PageIndex] != 0 )
  {
    ActualPage = Data[PageIndex];
    OffsetOfActualPage = PageIndex * PageSize;
    return true;
  }

  // no page available?
  if( NoOfFreePages < 1 )
  {
    // free the page which is the furthest away from the page we are loading
    if( abs(FirstPage-PageIndex) > abs(LastPage-PageIndex) )
      while( !freePage(FirstPage++) );
    else
      while( !freePage(LastPage--) );
  }

//   std::cout << "loading page " << PageIndex << std::endl;
  // create Page
  Data[PageIndex] = new char[PageSize];
  --NoOfFreePages;

  // jump to position and read the page's data in
  bool Success = File.seek( (unsigned long)(PageIndex*PageSize) );
  if( Success )
    Success = File.read( Data[PageIndex], PageSize ) > 0;

  if( Success )
  {
    // correct bounds
    if( (int)PageIndex < FirstPage )
      FirstPage = PageIndex;

    if( (int)PageIndex > LastPage )
      LastPage = PageIndex;

    ActualPage = Data[PageIndex];
    OffsetOfActualPage = PageIndex * PageSize;
  }

  return Success;
}


bool KFileByteArrayModel::freePage( unsigned int PageIndex ) const
{
  // check range and if is loaded at all
  if( (int)PageIndex >= Data.size() || !Data[PageIndex] )
    return false;
//   std::cout << "freeing page " << PageIndex << std::endl;
  delete [] Data[PageIndex];
  Data[PageIndex] = 0;
  ++NoOfFreePages;
  return true;
}
