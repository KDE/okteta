/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPTENGINEINITIALIZER_H_
#define SCRIPTENGINEINITIALIZER_H_

class QScriptEngine;
class QScriptContext;
class QScriptValue;
class QLatin1String;

/** This class adds all functions to the scriptengine that are needed for the scripts.
 *
 * For example this allows developers to write something like.
 * @code
 * var teststructure = struct({
 *      // now the members
 *      member1 : uint8(),
 *      member2 : array(uint16(),12),
 *      member3 : bitfield("bool",1),
 *      member4 : bitfield("unsigned", 12),
 * })
 * @endcode
 * @note
 */
namespace ScriptEngineInitializer
{

/** @return a new QScriptEngine with the functions added to the global object */
QScriptEngine* newEngine();
/** add the necessary functions to the QScriptEngine */
void addFuctionsToScriptEngine(QScriptEngine* engine);

namespace Private
{
/** create a new enum:
 *  first parameter is the type, second parameter is a list of name - value pairs */
QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng);
/** create a new flag value:
 *  first parameter is the type, second parameter is a list of name - value pairs */
QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewUInt8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewUInt16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewUInt32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewUInt64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewInt8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewInt16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewInt32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewInt64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewBool8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewBool16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewBool32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewBool64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewFloat(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewDouble(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
QScriptValue scriptNewChar(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructor/initializer function takes 2 arguments:
 *  <br>
 *  -the first is the type of the bitfield: "signed", "bool" or "unsigned"<br>
 *  -the second is the width (in bits) of the bitfield)
 */
QScriptValue scriptNewBitfield(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructor/initializer function takes 1 argument and returns a struct object:<br>
 * an object (hierarchy), which represents the children.<br>
 * An example would be:
 * @code
 * var obj = struct({
 *      member1 : uint8(),
 *      member2 : int32(),
 *      member3 : union({
 *          first : uint32(),
 *          second: float(),
 *          }),
 *      member4 : double(),
 *    })
 * @endcode
 */
QScriptValue scriptNewStruct(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructor/initializer function takes 1 argument and returns a union object:<br>
 * an object (hierarchy), which represents the children.<br>
 * An example would be:
 * @code
 * var obj = union({
 *      member1 : uint8(),
 *      member2 : int32(),
 *      member3 : struct({
 *          first : uint32(),
 *          second: float(),
 *          }),
 *      member4 : double(),
 *    })
 * @endcode
 */
QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor/initializer function takes 2 arguments and returns an array:<br>
 *  -the first is an object of the type of the array (can also be a struct or a union or even another array)<br>
 *  -the second is the length of the array<br>
 */
QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes one argument, the encoding of the string (as a string) */
QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes two arguments: 1. type of the pointer and 2. the target type */
QScriptValue scriptNewPointer(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes three arguments: 1. initial fields 2. the alternatives 3. (optional) default fields */
QScriptValue scriptNewTaggedUnion(QScriptContext* ctx, QScriptEngine* eng);

QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng);
QScriptValue addUpdateFunc(QScriptContext* ctx, QScriptEngine* eng);
QScriptValue addValidationFunc(QScriptContext* ctx, QScriptEngine* eng);
QScriptValue alternativeFunc(QScriptContext* ctx, QScriptEngine* eng);

/** this allows you to write e.g.
 *  return struct({foo : uint8() })
 *      .set({ updateFunc : ...,
 *             name : "something"})
 *
 */
QScriptValue addCustomPropertiesFunc(QScriptContext* ctx, QScriptEngine* eng);

}

}

#endif /* SCRIPTENGINEINITIALIZER_H_ */
