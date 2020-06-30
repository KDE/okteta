/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "booldatainformation.hpp"

#include <KLocalizedString>

template <typename T>
QString BoolDataInformationMethods<T>::staticValueString(T value, int base)
{
    if (value == 0) {
        return i18nc("boolean value", "false");
    }
    if (value == 1) {
        return i18nc("boolean value", "true");
    }
    // we can reuse the uint valueString() here
    QString num = UIntDataInformationMethods<T>::staticValueString(value, base);
    return i18nc("boolean value with actual value", "true (%1)", num);
}

template <typename T>
inline QWidget* BoolDataInformationMethods<T>::staticCreateEditWidget(QWidget* parent)
{
    return UIntDataInformationMethods<T>::staticCreateEditWidget(parent);
}

template <typename T>
inline QVariant BoolDataInformationMethods<T>::staticDataFromWidget(const QWidget* w)
{
    return UIntDataInformationMethods<T>::staticDataFromWidget(w);
}

template <typename T>
inline void BoolDataInformationMethods<T>::staticSetWidgetData(T value, QWidget* w)
{
    UIntDataInformationMethods<T>::staticSetWidgetData(value, w);
}

// explicitly instantiate all valid classes (c++-faq-lite 35.12)
template class BoolDataInformationMethods<quint8>;
template class BoolDataInformationMethods<quint16>;
template class BoolDataInformationMethods<quint32>;
template class BoolDataInformationMethods<quint64>;
