/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef CHARSETCONVERSIONVIEW_H
#define CHARSETCONVERSIONVIEW_H

// Okteta core
#include <byte.h>
// Qt
#include <QtGui/QWidget>

namespace Okteta { class ByteArrayComboBox; }
class KComboBox;
class KPushButton;
class QCheckBox;
template <class Key, class T> class QMap;

namespace Kasten2
{

class CharsetConversionTool;


class CharsetConversionView : public QWidget
{
  Q_OBJECT

  public:
    explicit CharsetConversionView( CharsetConversionTool* tool, QWidget* parent = 0 );
    virtual ~CharsetConversionView();

  public:
    CharsetConversionTool* tool() const;

  private Q_SLOTS: // gui
    void onDefaultByteEditChanged( const QByteArray& byteArray );
    void onConvertButtonClicked();

  private Q_SLOTS: // tool
    void onApplyableChanged( bool isApplyable );
    void onConversionDone( bool success, int convertedBytesCount,
                           const QMap<Okteta::Byte, int>& failedPerByteCount );

  private:
    CharsetConversionTool* mTool;

    KComboBox* mDirectionComboBox;
    KComboBox* mOtherCharSetComboBox;
    QCheckBox* mSubstituteMissingCharCheckBox;
    Okteta::ByteArrayComboBox* mSubstituteByteEdit;
    KPushButton* mConvertButton;
};


inline CharsetConversionTool *CharsetConversionView::tool() const { return mTool; }

}

#endif
