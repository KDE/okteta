/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTVALUECONVERTER_HPP
#define KASTEN_SCRIPTVALUECONVERTER_HPP

class QString;
class QScriptValue;
class ScriptLogger;
template <typename T> class QVector;
class DataInformation;

namespace ScriptValueConverter {

// FIXME remove the = 0
/** If the value is one element */
DataInformation* convert(const QScriptValue& value, const QString& name, ScriptLogger* logger,
                         DataInformation* parent = nullptr);
/** If the value is a list of elements or an object with many elements */
QVector<DataInformation*> convertValues(const QScriptValue& value, ScriptLogger* logger,
                                        DataInformation* parent = nullptr);

}

#endif /* KASTEN_SCRIPTVALUECONVERTER_HPP */
