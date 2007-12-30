/***************************************************************************
                          stringsextracttool.h  -  description
                             -------------------
    begin                : Wed Dec 26 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef STRINGSEXTRACTTOOL_H
#define STRINGSEXTRACTTOOL_H


// Qt
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>

class KByteArrayDocument;
namespace KHECore {
class KCharCodec;
}

class ContainedString
{
  public:
    ContainedString( const QString &string, int offset );

  public:
    void move( int offset );

  public:
    QString string() const;
    int offset() const;

  protected:
    QString mString;
    int mOffset;
};
inline QString ContainedString::string() const { return mString; }
inline int ContainedString::offset() const { return mOffset; }


/**
*/
class StringsExtractTool : public QObject
{
  Q_OBJECT

  public:
    StringsExtractTool();
    ~StringsExtractTool();

  public:
    void setDocument( KByteArrayDocument *document );
    void setCharCodec( const QString &codecName );

    void extract();

  public:
    QList<ContainedString> containedStringList() const;

  Q_SIGNALS:
    void stringsChanged();

  protected:
    KByteArrayDocument *mDocument;
    KHECore::KCharCodec *mCharCodec;

    QList<ContainedString> mContainedStringList;
    int mMinLength;
};
#endif
