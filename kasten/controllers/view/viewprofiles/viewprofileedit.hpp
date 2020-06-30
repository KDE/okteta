/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILEEDIT_HPP
#define KASTEN_VIEWPROFILEEDIT_HPP

// Qt
#include <QWidget>

// KF
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
