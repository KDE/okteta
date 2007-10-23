/***************************************************************************
                          kprimitivetypesview.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
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



#ifndef KPRIMITIVETYPESVIEW_H
#define KPRIMITIVETYPESVIEW_H


// Okteta core
#include <khe.h>
// Qt
#include <QtGui/QWidget>

class QComboBox;
class QLineEdit;
class QCheckBox;
namespace KHECore {
class KCharCodec;
}

class KPrimitiveTypesView : public QWidget
{
  Q_OBJECT

  public:
    KPrimitiveTypesView( QWidget *Parent = 0 );
    ~KPrimitiveTypesView();

  public: // QWidget API
//     virtual void setFont(
  public:
    void setUndefinedChar( const QChar &Char );
    void setCharCode( const QString &CodeName );

  public Q_SLOTS:
    void onByteArrayChange( const unsigned char *ByteArray );

    void fontChanged();
    void onEndianActivation( int );
    void onUnsignedAsHex();
    void onBitWidthChange( int );

  protected:
    const unsigned char *ByteArray;

    QLineEdit *Int8Int16Display[4];
    QLineEdit *Int32FloatDisplay[4];
    QLineEdit *ByteCodeDisplay[4];
    QComboBox *EndianSelection;
    QCheckBox *UnsignedAsHexCheck;
    QComboBox *BitWidthSelection;

    KHECore::KCharCodec *CharCodec;
    QChar UndefinedChar;
};

#endif
