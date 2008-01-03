/***************************************************************************
                          infotool.h  -  description
                             -------------------
    begin                : Wed Jan 2 2008
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


#ifndef INFOTOOL_H
#define INFOTOOL_H


// Qt
#include <QtCore/QObject>

class StatisticTableModel;

class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}

/**
*/
class InfoTool : public QObject
{
  Q_OBJECT

  public:
    InfoTool();
    virtual ~InfoTool();

  public:
    void setView( KAbstractView *view );

  public:
    StatisticTableModel *statisticTableModel() const;
    int size() const;
    bool hasByteArrayView() const;

  public Q_SLOTS:
    void updateStatistic();

  Q_SIGNALS:
    void byteArrayViewChanged( bool hasByteArrayView );
    void statisticDirty( bool dirty );

  protected:
    int mByteCount[256]; // TODO: here or in statistic model?

    StatisticTableModel *mStatisticTableModel;

    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArrayModel;
};

#endif
