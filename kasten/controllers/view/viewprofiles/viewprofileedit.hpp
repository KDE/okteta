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

#ifndef KASTEN_VIEWPROFILEEDIT_HPP
#define KASTEN_VIEWPROFILEEDIT_HPP

// Qt
#include <QWidget>

// KF5
class KComboBox;
// Qt
class QCheckBox;
class QSpinBox;
class QLineEdit;

namespace Kasten {
class ByteArrayViewProfile;

class ViewProfileEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ViewProfileEdit(QWidget* parent = nullptr);

    ~ViewProfileEdit() override;

public:
    ByteArrayViewProfile viewProfile() const;

public:
    void setViewProfile(const ByteArrayViewProfile& viewProfile);

Q_SIGNALS:
    void profileTitleChanged(const QString& title);

private Q_SLOTS:
    void onLineBreakIndexChanged(int lineBreakIndex);

private: // title
    QLineEdit* mTitleEdit;

private: // display
    QCheckBox* mLineOffsetShownCheckBox;
    KComboBox* mOffsetCodingComboBox;
    KComboBox* mValuesCharsShownComboBox;
    KComboBox* mDisplayModeComboBox;

private: // layout
    KComboBox* mLineBreakComboBox;
    QSpinBox* mGroupedBytesCountEdit;
    QSpinBox* mBytesPerLineEdit;

private: // value
    KComboBox* mValueCodingComboBox;

private: // char
    KComboBox* mCharCodingComboBox;
    QCheckBox* mNonPrintableShownCheckBox;
    QLineEdit* mUndefinedCharEdit;
    QLineEdit* mNonPrintableCharEdit;
};

}

#endif
