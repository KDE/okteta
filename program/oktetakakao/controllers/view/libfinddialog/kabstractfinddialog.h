/***************************************************************************
                          kabstractfinddialog.h  -  description
                             -------------------
    begin                : Tue Nov 14 2006
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


#ifndef KABSTRACTFINDDIALOG_H
#define KABSTRACTFINDDIALOG_H


// lib
#include "kfinddirection.h"
// KDE
#include <KDialog>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>

class QGroupBox;
class QCheckBox;
class KByteArrayLineEdit;


class KAbstractFindDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit KAbstractFindDialog( QWidget *parent = 0 );
    virtual ~KAbstractFindDialog();

  public: // set
    void setDirection( KFindDirection Direction );
    void setInSelection( bool InSelection );
    void setCharCode( const QString &CodeName );

  public: // get 
    QByteArray data() const;
    bool fromCursor() const;
    bool inSelection() const;
    bool ignoreCase() const;
    KFindDirection direction() const;

  protected: // QWidget API
    virtual void showEvent( QShowEvent *e );

  protected:
    void setOperationBox( QGroupBox *OperationBox );
    void setExtraOption( QCheckBox* OptionCheckBox );

  private Q_SLOTS:
    void onSearchDataChanged( const QByteArray &Data );
    void onSearchDataFormatChanged( int Format );

  private:
    KByteArrayLineEdit *SearchDataEdit;
    QCheckBox *BackwardsCheckBox;
    QCheckBox *AtCursorCheckBox;
    QCheckBox *SelectedCheckBox;
    QCheckBox *WholeWordsCheckBox;
    QCheckBox *CaseSensitiveCheckBox;
};

#endif
