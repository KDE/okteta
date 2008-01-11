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
class KComboBox;
class KLineEdit;
class QCheckBox;

class KPrimitiveTypesView : public QWidget
{
  Q_OBJECT

  public:
    explicit KPrimitiveTypesView( PODDecoderTool *tool, QWidget *parent = 0 );
    virtual ~KPrimitiveTypesView();

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

    KLineEdit *mInt8Int16Display[NoOfRows];
    KLineEdit *mInt32FloatDisplay[NoOfRows];
    KLineEdit *mByteCodeDisplay[NoOfRows];
    KComboBox *mEndianSelection;
    QCheckBox *mUnsignedAsHexCheck;
    KComboBox *mBitWidthSelection;

    KHECore::KCharCodec *mCharCodec;
    QChar mUndefinedChar;
};

#endif
