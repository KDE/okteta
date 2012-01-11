/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef KABSTRACTFINDDIALOG_H
#define KABSTRACTFINDDIALOG_H

// lib
#include "kfinddirection.h"
// KDE
#include <KDialog>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QString>

namespace Okteta {
class ByteArrayComboBox;
}

class QGroupBox;
class QCheckBox;


namespace Kasten2
{

class KAbstractFindDialog : public KDialog
{
  Q_OBJECT

  public:
    explicit KAbstractFindDialog( QWidget* parent = 0 );
    virtual ~KAbstractFindDialog();

  public: // set
    void setDirection( KFindDirection Direction );
    void setInSelection( bool InSelection );

  public: // get 
    QByteArray data() const;
    bool fromCursor() const;
    bool inSelection() const;
    Qt::CaseSensitivity caseSensitivity() const;
    KFindDirection direction() const;

  public Q_SLOTS:
    void setCharCodec( const QString& codecName );

  protected: // QWidget API
    virtual void showEvent( QShowEvent *e );

  protected:
    void setupFindBox();
    void setupOperationBox( QGroupBox *operationBox = 0 );
    void setupCheckBoxes( QCheckBox *optionCheckBox = 0 );

  protected:
    virtual void rememberCurrentSettings();

  private Q_SLOTS:
    void onSearchDataChanged( const QByteArray &ata );
    void onSearchDataFormatChanged( int Format );

  private:
    Okteta::ByteArrayComboBox* SearchDataEdit;
    QCheckBox *BackwardsCheckBox;
    QCheckBox *AtCursorCheckBox;
    QCheckBox *SelectedCheckBox;
    QCheckBox *WholeWordsCheckBox;
    QCheckBox *CaseSensitiveCheckBox;
};

}

#endif
