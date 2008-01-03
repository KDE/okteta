/***************************************************************************
                          infoview.h  -  description
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


#ifndef INFOVIEW_H
#define INFOVIEW_H


// Qt
#include <QtGui/QWidget>

class InfoTool;
class KPushButton;
class QLabel;
class QTreeView;


class InfoView : public QWidget
{
  Q_OBJECT

  public:
    explicit InfoView( InfoTool *tool, QWidget *parent = 0 );
    virtual ~InfoView();

  public Q_SLOTS:
    void updateHeader();
    void setDirty( bool dirty );
    void setByteArraySize( int size );

  private:
    InfoTool *mTool;

    QLabel *mDirtyLabel;
    QLabel *mSizeLabel;
    QTreeView *mStatisticTableView;
    KPushButton *mUpdateButton;
};

#endif
