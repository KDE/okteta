/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KREPLACEDIALOG_H
#define KREPLACEDIALOG_H

// lib
#include <kabstractfinddialog.h>

class ReplaceTool;


class KReplaceDialog : public KAbstractFindDialog
{
  Q_OBJECT

  public:
    explicit KReplaceDialog( ReplaceTool* tool, QWidget* parent = 0 );
    virtual ~KReplaceDialog();

  public: // set
//     void setDirection( KReplaceDirection Direction );
    void setCharCodec( const QString &codecName );

  public: // get 
    QByteArray replaceData() const;
    bool prompt() const;

  protected: // KDialog API
    virtual void slotButtonClicked( int button );

  protected: // QWidget API
    virtual void showEvent( QShowEvent* showEvent );

  private:
    KByteArrayLineEdit *ReplaceDataEdit;
    QCheckBox *PromptCheckBox;

    ReplaceTool* mTool;
};

#endif
