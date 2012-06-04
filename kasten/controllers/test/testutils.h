/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

namespace Utils
{

/** Converts a string to a binary number (spaces are ignored)
 * @param val the string representing a binary number
 * @return
 */
template<typename T>
T binary(const char* val)
{
    QString value = QLatin1String(val);
    value = value.remove(QLatin1Char(' '));
    Q_ASSERT(unsigned(value.length()) <= sizeof(T) * 8); //otherwise we overflow
    bool ok = false;
    quint64 result = value.toULongLong(&ok, 2);
    Q_ASSERT(ok);
    return static_cast<T>(result);
}

}

#endif /* TESTUTILS_H_ */
