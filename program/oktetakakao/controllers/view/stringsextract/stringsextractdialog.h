/***************************************************************************
                          stringsextractdialog.h  -  description
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


#ifndef STRINGSEXTRACTDIALOG_H
#define STRINGSEXTRACTDIALOG_H

// KDE
#include <KDialog>

class StringsExtractTool;
class KByteArrayDocument;
class QListWidget;

class StringsExtractDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit StringsExtractDialog( StringsExtractTool *tool, QWidget *parent = 0 );
    virtual ~StringsExtractDialog();

  public:

  public Q_SLOTS:
    void onUpdateClicked();
    void onStringsChanged();

  protected:
    StringsExtractTool *mTool;

    QListWidget *mListView;
};

#endif
