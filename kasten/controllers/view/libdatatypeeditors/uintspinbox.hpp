/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINTSPINBOX_HPP
#define KASTEN_UINTSPINBOX_HPP

// Qt
#include <QAbstractSpinBox>
// C++
#include <limits>

class UIntSpinBox : public QAbstractSpinBox
{
    Q_OBJECT
    Q_PROPERTY(quint64 value READ value WRITE setValue USER true)

public:
    explicit UIntSpinBox(QWidget* parent = nullptr, int base = 10);

    ~UIntSpinBox() override;

public:
    quint64 value() const;

    quint64 maximum() const;

public:
    void setValue(quint64 value);

    void setMaximum(quint64 max);
    void setBase(int base);

    static UIntSpinBox* createUInt64Spinbox(QWidget* parent = nullptr);
    static UIntSpinBox* createUInt32Spinbox(QWidget* parent = nullptr);
    static UIntSpinBox* createUInt16Spinbox(QWidget* parent = nullptr);
    static UIntSpinBox* createUInt8Spinbox(QWidget* parent = nullptr);

protected: // QAbstractSpinBox API
    QValidator::State validate(QString& input, int& pos) const override;
    void stepBy(int steps) override;
    void fixup(QString& input) const override;
    StepEnabled stepEnabled() const override;

private:
    void updateEditLine() const;

private:
    mutable quint64 mValue = 0;

    quint64 mMaximum = std::numeric_limits<quint64>::max();
    // TODO minimum
    int mBase = 0;

    QString mPrefix;
};

inline UIntSpinBox::UIntSpinBox(QWidget* parent, int base)
    : QAbstractSpinBox(parent)
{
    setBase(base);
}

inline UIntSpinBox::~UIntSpinBox() = default;

inline quint64 UIntSpinBox::value()   const { return mValue; }
inline quint64 UIntSpinBox::maximum() const { return mMaximum; }

inline void UIntSpinBox::setMaximum(quint64 maximum)
{
    if (mMaximum == maximum) {
        return;
    }

    mMaximum = maximum;

    if (mValue > mMaximum) {
        mValue = mMaximum;

        updateEditLine();
    }
}

inline void UIntSpinBox::setValue(quint64 value)
{
    if (value > mMaximum) {
        value = mMaximum;
    }

    if (mValue == value) {
        return;
    }

    mValue = value;

    updateEditLine();
}

inline void UIntSpinBox::setBase(int base)
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

inline UIntSpinBox* UIntSpinBox::createUInt64Spinbox(QWidget* parent)
{
    return new UIntSpinBox(parent);
}

inline UIntSpinBox* UIntSpinBox::createUInt32Spinbox(QWidget* parent)
{
    auto* ret = new UIntSpinBox(parent);
    ret->setMaximum(std::numeric_limits<quint32>::max());
    return ret;
}

inline UIntSpinBox* UIntSpinBox::createUInt16Spinbox(QWidget* parent)
{
    auto* ret = new UIntSpinBox(parent);
    ret->setMaximum(std::numeric_limits<quint16>::max());
    return ret;
}

inline UIntSpinBox* UIntSpinBox::createUInt8Spinbox(QWidget* parent)
{
    auto* ret = new UIntSpinBox(parent);
    ret->setMaximum(std::numeric_limits<quint8>::max());
    return ret;
}

#endif
