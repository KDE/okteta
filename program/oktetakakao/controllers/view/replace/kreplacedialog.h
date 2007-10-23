/***************************************************************************
                          kreplacedialog.h  -  description
                             -------------------
    begin                : Fri Nov 24 2006
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


#ifndef KREPLACEDIALOG_H
#define KREPLACEDIALOG_H


// dialog
#include <kabstractfinddialog.h>


class KReplaceDialog : public KAbstractFindDialog
{
  Q_OBJECT

  public:
    explicit KReplaceDialog( QWidget *parent = 0 );
    virtual ~KReplaceDialog();

  public: // set
//     void setDirection( KReplaceDirection Direction );
    void setCharCode( const QString &CodeName );

  public: // get 
    QByteArray replaceData() const;
    bool prompt() const;

  private:
    KByteArrayLineEdit *ReplaceDataEdit;
    QCheckBox *PromptCheckBox;
};

#endif
