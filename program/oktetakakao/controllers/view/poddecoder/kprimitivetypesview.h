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

class PODDecoderTool;
namespace KHECore {
class KCharCodec;
}
class QComboBox;
class QLineEdit;
class QCheckBox;

class KPrimitiveTypesView : public QWidget
{
  Q_OBJECT

  public:
    KPrimitiveTypesView( PODDecoderTool *tool, QWidget *parent = 0 );
    ~KPrimitiveTypesView();

  public:
    void setUndefinedChar( const QChar &undefinedChar );
    void setCharCode( const QString &codeName );

  public Q_SLOTS:
    void onDataChange();

    void fontChanged();
    void onEndianActivation( int );
    void onUnsignedAsHex();
    void onBitWidthChange( int );

  protected:
    static const int NoOfRows = 4;

  protected:
    PODDecoderTool *mTool;

    QLineEdit *mInt8Int16Display[NoOfRows];
    QLineEdit *mInt32FloatDisplay[NoOfRows];
    QLineEdit *mByteCodeDisplay[NoOfRows];
    QComboBox *mEndianSelection;
    QCheckBox *mUnsignedAsHexCheck;
    QComboBox *mBitWidthSelection;

    KHECore::KCharCodec *mCharCodec;
    QChar mUndefinedChar;
};

#endif
