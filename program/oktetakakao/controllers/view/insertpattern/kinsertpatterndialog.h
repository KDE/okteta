/***************************************************************************
                          kinsertpatterndialog.h  -  description
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


#ifndef KINSERTPATTERNDIALOG_H
#define KINSERTPATTERNDIALOG_H


// KDE
#include <KDialog>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>

class QSpinBox;
class KByteArrayLineEdit;


class KInsertPatternDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit KInsertPatternDialog( QWidget *parent = 0 );
    virtual ~KInsertPatternDialog();

  public: // set
    void setCharCode( const QString &CodeName );

  public: // get 
    QByteArray pattern() const;
    int number() const;

  protected:
    virtual void showEvent( QShowEvent *e );

  private Q_SLOTS:
    void onInputChanged( const QByteArray &Data );

  private:
    QSpinBox *NumberSpinBox;
    KByteArrayLineEdit *PatternEdit;
};

#endif
