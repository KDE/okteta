/***************************************************************************
                          insertpatterndialog.h  -  description
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


#ifndef INSERTPATTERNDIALOG_H
#define INSERTPATTERNDIALOG_H


// KDE
#include <KDialog>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>

class QSpinBox;
class KByteArrayLineEdit;


class InsertPatternDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit InsertPatternDialog( QWidget *parent = 0 );
    virtual ~InsertPatternDialog();

  public: // set
    void setCharCode( const QString &codeName );

  public: // get 
    QByteArray pattern() const;
    int number() const;

  protected:
    virtual void showEvent( QShowEvent *event );

  private Q_SLOTS:
    void onInputChanged( const QByteArray &data );

  private:
    QSpinBox *mNumberSpinBox;
    KByteArrayLineEdit *mPatternEdit;
};

#endif
