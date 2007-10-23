/***************************************************************************
                          kbytearraylineedit.h  -  description
                             -------------------
    begin                : Sun Nov 26 2006
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


#ifndef KBYTEARRAYLINEEDIT_H
#define KBYTEARRAYLINEEDIT_H


// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtGui/QWidget>

class KByteArrayLineEditPrivate;


class KByteArrayLineEdit : public QWidget
{
  friend class KByteArrayLineEditPrivate;

  Q_OBJECT

  public:
    explicit KByteArrayLineEdit( QWidget *parent = 0 );
    virtual ~KByteArrayLineEdit();

  public: // set
    void setCharCode( const QString &CodeName );

  public: // get 
    QByteArray data() const;
    int format() const;

  Q_SIGNALS:
    void dataChanged( const QByteArray& );
    void formatChanged( int );

  private Q_SLOTS:
    void onFormatChanged( int Index );
    void onDataChanged( const QString &Data );

  private:
    KByteArrayLineEditPrivate * const d;
};

#endif
