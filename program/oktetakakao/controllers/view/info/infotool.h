/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

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
