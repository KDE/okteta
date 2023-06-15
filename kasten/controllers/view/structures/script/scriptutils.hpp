/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTUTILS_HPP
#define KASTEN_SCRIPTUTILS_HPP

class QScriptValue;
class QString;

namespace ScriptUtils {

QString qScriptValueToString(const QScriptValue& val);

}

#endif /* KASTEN_SCRIPTUTILS_HPP */
