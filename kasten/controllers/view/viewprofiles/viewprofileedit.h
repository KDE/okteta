/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VIEWPROFILEEDIT_H
#define VIEWPROFILEEDIT_H

// Qt
#include <QtGui/QWidget>

// KDE
class KLineEdit;
class KComboBox;
class KIntNumInput;
// Qt
class QCheckBox;


namespace Kasten2
{
class ByteArrayViewProfile;


class ViewProfileEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ViewProfileEdit( QWidget* parent = 0 );

    virtual ~ViewProfileEdit();

public:
    ByteArrayViewProfile viewProfile() const;

public:
    void setViewProfile( const ByteArrayViewProfile& viewProfile );

protected Q_SLOTS:
    void onLineBreakIndexChanged( int lineBreakIndex );

protected: // title
    KLineEdit* mTitleEdit;
protected: // display
    QCheckBox* mLineOffsetShownCheckBox;
    KComboBox* mValuesCharsShownComboBox;
    KComboBox* mDisplayModeComboBox;
protected: // layout
    KComboBox* mLineBreakComboBox;
    KIntNumInput* mGroupedBytesCountEdit;
    KIntNumInput* mBytesPerLineEdit;
protected: // value
    KComboBox* mValueCodingComboBox;
protected: // char
    KComboBox* mCharCodingComboBox;
    QCheckBox* mNonPrintableShownCheckBox;
    KLineEdit* mUndefinedCharEdit;
    KLineEdit* mNonPrintableCharEdit;
};

}

#endif
