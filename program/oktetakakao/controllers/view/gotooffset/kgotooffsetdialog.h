/***************************************************************************
                          kgotooffsetdialog.h  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#ifndef KSEARCHDIALOG_H
#define KSEARCHDIALOG_H

// KDE
#include <KDialog>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>

class QSpinBox;
class QCheckBox;
class QComboBox;
class KByteArrayValidator;


class KGotoOffsetDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit KGotoOffsetDialog( QWidget *parent = 0 );
    ~KGotoOffsetDialog();

  public: // set
    void setRange( int Start, int End );

  public: // get 
    int offset() const;
    bool isRelative() const;

  protected:
    virtual void showEvent( QShowEvent *e );

  private Q_SLOTS:
    void onSelectorChanged( int index );
    void onOffsetChanged( const QString &text );

  private:
    QComboBox *mSelector;
    QComboBox *OffsetEdit;
    QCheckBox *AtCursorCheckBox;
    QCheckBox *BackwardsCheckBox;

    QString    mOffsetString[2];
    KByteArrayValidator *mOffsetValidator;
};

#endif
