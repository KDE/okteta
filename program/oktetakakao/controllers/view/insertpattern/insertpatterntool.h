/***************************************************************************
                          insertpatterntool.h  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef INSERTPATTERNTOOL_H
#define INSERTPATTERNTOOL_H


// Qt
#include <QtCore/QObject>

class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}
class QString;
class QByteArray;


class InsertPatternTool : public QObject
{
  Q_OBJECT

  public:
    InsertPatternTool();
    virtual ~InsertPatternTool();

  public:
    void setView( KAbstractView *view );

  public:
    void insertPattern( const QByteArray &pattern, int count );

  public:
    QString charCodecName() const;

  Q_SIGNALS:
    void viewChanged( bool hasView );

  protected:
    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArrayModel;
};

#endif
