/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINTSPINBOX_HPP
#define KASTEN_SINTSPINBOX_HPP

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

private:
    void updateEditLine() const;

private:
    mutable qint64 mValue = 0;

    qint64 mMinimum = std::numeric_limits<qint64>::min();
    qint64 mMaximum = std::numeric_limits<qint64>::max();
    int mBase = 0;

    QString mPrefix;
};

inline SIntSpinBox::SIntSpinBox(QWidget* parent, int base)
    : QAbstractSpinBox(parent)
{
    setBase(base);
}

inline SIntSpinBox::~SIntSpinBox() = default;

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

inline SIntSpinBox* SIntSpinBox::createSInt64Spinbox(QWidget* parent)
{
    return new SIntSpinBox(parent);
}

inline SIntSpinBox* SIntSpinBox::createSInt32Spinbox(QWidget* parent)
{
    auto* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt16Spinbox(QWidget* parent)
{
    auto* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint16>::min(), std::numeric_limits<qint16>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt8Spinbox(QWidget* parent)
{
    auto* ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint8>::min(), std::numeric_limits<qint8>::max());
    return ret;
}

#endif
