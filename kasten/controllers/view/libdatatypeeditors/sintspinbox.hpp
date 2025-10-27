/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_SINTSPINBOX_HPP
#define OKTETA_SINTSPINBOX_HPP

// Qt
#include <QAbstractSpinBox>
// Std
#include <algorithm>
#include <limits>

namespace Okteta {

class SIntSpinBox : public QAbstractSpinBox
{
    Q_OBJECT
    Q_PROPERTY(qint64 value READ value WRITE setValue USER true)

public:
    explicit SIntSpinBox(QWidget* parent = nullptr, int base = 10);

    ~SIntSpinBox() override;

public:
    [[nodiscard]]
    qint64 value() const;

public:
    void setValue(qint64 value);

    // TODO getters + setters
    void setRange(qint64 minimum, qint64 maximum);
    void setBase(int base);

    [[nodiscard]]
    static SIntSpinBox* createSInt64Spinbox(QWidget* parent = nullptr);
    [[nodiscard]]
    static SIntSpinBox* createSInt32Spinbox(QWidget* parent = nullptr);
    [[nodiscard]]
    static SIntSpinBox* createSInt16Spinbox(QWidget* parent = nullptr);
    [[nodiscard]]
    static SIntSpinBox* createSInt8Spinbox(QWidget* parent = nullptr);

protected: // QAbstractSpinBox API
    [[nodiscard]]
    QValidator::State validate(QString& input, int& pos) const override;
    void stepBy(int steps) override;
    void fixup(QString& input) const override;
    [[nodiscard]]
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
    base = std::clamp(base, 2, 36);

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
    auto* const ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt16Spinbox(QWidget* parent)
{
    auto* const ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint16>::min(), std::numeric_limits<qint16>::max());
    return ret;
}

inline SIntSpinBox* SIntSpinBox::createSInt8Spinbox(QWidget* parent)
{
    auto* const ret = new SIntSpinBox(parent);
    ret->setRange(std::numeric_limits<qint8>::min(), std::numeric_limits<qint8>::max());
    return ret;
}

}

#endif
