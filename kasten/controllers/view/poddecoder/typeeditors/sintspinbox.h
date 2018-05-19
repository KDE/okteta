/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SINTSPINBOX_H
#define SINTSPINBOX_H

// Qt
#include <QAbstractSpinBox>
// C++
#include <limits>

class SIntSpinBox : public QAbstractSpinBox
{
    Q_OBJECT
    Q_PROPERTY(qint64 value READ value WRITE setValue USER true)

public:
    explicit SIntSpinBox(QWidget* parent = nullptr, int base = 10);

    ~SIntSpinBox() override;

public:
    qint64 value() const;

public:
    void setValue(qint64 value);

    // TODO getters + setters
    void setRange(qint64 minimum, qint64 maximum);
    void setBase(int base);

    static SIntSpinBox* createSInt64Spinbox(QWidget* parent = nullptr);
    static SIntSpinBox* createSInt32Spinbox(QWidget* parent = nullptr);
    static SIntSpinBox* createSInt16Spinbox(QWidget* parent = nullptr);
    static SIntSpinBox* createSInt8Spinbox(QWidget* parent = nullptr);

protected: // QAbstractSpinBox API
    QValidator::State validate(QString& input, int& pos) const override;
    void stepBy(int steps) override;
    void fixup(QString& input) const override;
    StepEnabled stepEnabled() const override;

protected:
    void updateEditLine() const;

protected:
    mutable qint64 mValue;

    qint64 mMinimum;
    qint64 mMaximum;
    int mBase;

    QString mPrefix;
};

inline SIntSpinBox::SIntSpinBox(QWidget* parent, int base)
    : QAbstractSpinBox(parent)
    , mValue(0)
    , mMinimum(std::numeric_limits<qint64>::min())
    , mMaximum(std::numeric_limits<qint64>::max())
    , mBase(0)
{
    setBase(base);
}

inline qint64 SIntSpinBox::value() const { return mValue; }

inline void SIntSpinBox::setValue(qint64 value)
{
    if (value > mMaximum) {
        value = mMaximum;
    } else if (value < mMinimum) {
        value = mMinimum;
    }

    if (mValue == value) {
        return;
    }

    mValue = value;

    updateEditLine();
}

inline void SIntSpinBox::setRange(qint64 minimum, qint64 maximum)
{
    mMinimum = minimum;
    mMaximum = (maximum > minimum) ? maximum : minimum;

    if (mValue > mMaximum) {
        mValue = mMaximum;

        updateEditLine();
    } else if (mValue < mMinimum) {
        mValue = mMinimum;

        updateEditLine();
    }
}

inline void SIntSpinBox::setBase(int base)
{
    base = qBound(2, base, 36);

    if (mBase == base) {
        return;
    }

    mBase = base;
    mPrefix = QString::fromLatin1(
        (base == 16) ? "0x" :
        (base ==  8) ? "0o" :
        (base ==  2) ? "0b" :
        /* else */     nullptr);
}

inline SIntSpinBox::~SIntSpinBox() {}

inline SIntSpinBox* SIntSpinBox::createSInt64Spinbox(QWidget* parent)
{
    return new SIntSpinBox(parent);
}

inline SIntSpinBox* SIntSpinBox::createSInt32Spinbox(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt16Spinbox(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint16>::min(), std::numeric_limits<qint16>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt8Spinbox(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint8>::min(), std::numeric_limits<qint8>::max());
    return ret;
}

#endif
