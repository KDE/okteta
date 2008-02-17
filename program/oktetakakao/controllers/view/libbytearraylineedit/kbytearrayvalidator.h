/***************************************************************************
                          kbytearrayvalidator.h  -  description
                             -------------------
    begin                : Tue Nov 14 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTEARRAYVALIDATOR_H
#define KBYTEARRAYVALIDATOR_H


// Okteta core
#include <khe.h>
// Qt
#include <QtGui/QValidator>

namespace KHECore {
class ValueCodec;
class KCharCodec;
}


class KByteArrayValidator : public QValidator
{
  Q_OBJECT

  public:
    static const QStringList &codingNames();

  public:
    KByteArrayValidator( QObject *parent, int Coding, int CharCoding = KHECore::LocalEncoding );
    virtual ~KByteArrayValidator();

  public: // QValidator API
    virtual QValidator::State validate( QString &input, int &pos ) const;

  public:
    void setCoding( int Coding );
    void setCharCode( const QString &CodeName );

  public:
    QByteArray toByteArray( const QString &src ) const;
    QString toString( const QByteArray &src ) const;

  private:
    /**
     * Returns a string that is at least as long as @p destLen number of characters,
     * by adding zeroes to the left as necessary.
     *
     * e.g. zeroExtend( "32", 3 ) => "032"
     */
//     QString zeroExtend( const QString &src, int destLen ) const;

    KHECore::KCoding Coding;
    KHECore::ValueCodec *ValueCodec;
    KHECore::KCharCodec *CharCodec;
};

#endif
