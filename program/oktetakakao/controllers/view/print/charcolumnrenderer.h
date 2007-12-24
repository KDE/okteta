/***************************************************************************
                          charcolumnrenderer.h  -  description
                             -------------------
    begin                : Mit Sep 3 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef CHARCOLUMNRENDERER_H
#define CHARCOLUMNRENDERER_H


// lib
#include "bytecolumnrenderer.h"
// Qt
#include <QtCore/QString>

class QColor;


/** buffer column that interprets the bytes as chars
  *
  *@author Friedrich W. H. Kossebau
  */
class CharColumnRenderer : public ByteColumnRenderer
{
  public:
    CharColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                        const  KHECore::KAbstractByteArrayModel *byteArrayModel,
                        const KHEUI::KDataLayout *layout );
    virtual ~CharColumnRenderer();


  public: // modification access
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    bool setShowsNonprinting( bool showsNonprinting = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    bool setSubstituteChar( QChar substituteChar );
    /** sets the undefined character for "undefined" chars
      * returns true if there was a change
      */
    bool setUndefinedChar( QChar undefinedChar );


  public: // value access
    /** returns true if "unprintable" chars (>32) are displayed in the char column
      * with their corresponding character, default is false
      */
    bool showsNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;


  protected: // ByteColumnRenderer API
    virtual void drawByte( QPainter *painter, char byte, KHECore::KChar byteChar, const QColor &color ) const;

  protected:
    /** */
    bool mShowsNonprinting;
    /** */
    QChar mSubstituteChar;
    /** */
    QChar mUndefinedChar;
};


inline bool CharColumnRenderer::showsNonprinting()  const { return mShowsNonprinting; }
inline QChar CharColumnRenderer::substituteChar()  const { return mSubstituteChar; }
inline QChar CharColumnRenderer::undefinedChar()   const { return mUndefinedChar; }

inline bool CharColumnRenderer::setSubstituteChar( QChar substituteChar )
{
    if( mSubstituteChar == substituteChar )
        return false;
    mSubstituteChar = substituteChar;
    return true;
}

inline bool CharColumnRenderer::setUndefinedChar( QChar undefinedChar )
{
    if( mUndefinedChar == undefinedChar )
        return false;
    mUndefinedChar = undefinedChar;
    return true;
}

inline bool CharColumnRenderer::setShowsNonprinting( bool showsNonprinting )
{
    if( mShowsNonprinting == showsNonprinting )
        return false;
    mShowsNonprinting = showsNonprinting;
    return true;
}

#endif
