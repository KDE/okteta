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

// Kakao core
#include <abstracttool.h>
// Okteta core
#include <section.h>

class StatisticTableModel;

class KByteArrayDisplay;
namespace KHECore {
class AbstractByteArrayModel;
}

/**
*/
class InfoTool : public AbstractTool
{
  Q_OBJECT

  public:
    InfoTool();
    virtual ~InfoTool();

  public:
    StatisticTableModel *statisticTableModel() const;
    int size() const;
    bool isApplyable() const;
    bool isStatisticUptodate() const;

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public Q_SLOTS:
    void updateStatistic();

  Q_SIGNALS:
    void isApplyableChanged( bool isApplyable );
    void statisticDirty( bool dirty );

  protected Q_SLOTS:
    void onSelectionChanged( bool hasSelection );
    void onSourceChanged();
    void onSourceDestroyed();

  protected:
    int mByteCount[256]; // TODO: here or in statistic model?

    StatisticTableModel *mStatisticTableModel;

    KByteArrayDisplay* mByteArrayDisplay;
    KHECore::AbstractByteArrayModel *mByteArrayModel;

    //
    bool mSourceByteArrayModelUptodate;
    // selection source
    KHE::Section mSourceSelection;
    // source of strings
    KHECore::AbstractByteArrayModel *mSourceByteArrayModel;
};

#endif
