/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include <QtScript/QScriptValue>

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
class ScriptEngineInitializer
{
public:
    static void addFuctionsToScriptEngine(QScriptEngine& engine);
private:
    /** create a new enum:
     *  first parameter is the type, second parameter is a list of name - value pairs */
    static QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng);
    /** create a new flag value:
     *  first parameter is the type, second parameter is a list of name - value pairs */
    static QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue createEnumObject(QScriptContext* ctx, QScriptEngine* eng, bool flags);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewUInt8(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewUInt16(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewUInt32(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewUInt64(QScriptContext* ctx, QScriptEngine* eng);

    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewInt8(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewInt16(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewInt32(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewInt64(QScriptContext* ctx, QScriptEngine* eng);

    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewBool8(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewBool16(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewBool32(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewBool64(QScriptContext* ctx, QScriptEngine* eng);

    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewFloat(QScriptContext* ctx, QScriptEngine* eng);
    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewDouble(QScriptContext* ctx, QScriptEngine* eng);

    /** this script constructors/initializer takes no parameters */
    static QScriptValue scriptNewChar(QScriptContext* ctx, QScriptEngine* eng);

    /** this script constructor/initializer function takes 2 arguments:
     *  <br>
     *  -the first is the type of the bitfield: "signed", "bool" or "unsigned"<br>
     *  -the second is the width (in bits) of the bitfield)
     */
    static QScriptValue scriptNewBitfield(QScriptContext* ctx, QScriptEngine* eng);

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
    static QScriptValue scriptNewStruct(QScriptContext* ctx, QScriptEngine* eng);
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
    static QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng);
    /** this constructor/initializer function takes 2 arguments and returns an array:<br>
     *  -the first is an object of the type of the array (can also be a struct or a union or even another array)<br>
     *  -the second is the length of the array<br>
     */
    static QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng);
    /** this constructor takes one argument, the encoding of the string (as a string) */
    static QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng);

    /** A toString() implementation for primitive types.
     * <br>
     *  If called with 1 argument returns a string containing the way this would be defined in C/C++<br>
     *  <br>
     *  Example would be: @code "uint16 foo;" @endcode
     *  <br>
     *  Otherwise just returns the primitive type, i.e. @code "int64" @endcode
     */
private:
    //the toString functions
    static QScriptValue primitiveToString(QScriptContext* ctx, QScriptEngine* eng);
    /** A toString() implementation for bitfields
     *
     *  If called with one argument returns a string containing the way this would be defined in C/C++<br>
     *  <br>
     *  Example would be: @code "unsigned foo : 3;" @endcode <br>
     *  <br>
     *  Otherwise returns for example "unsigned : 2" or "bool : 1" or "signed : 12".
     */
    static QScriptValue bitfieldToString(QScriptContext* ctx, QScriptEngine* eng);
    /** A toString() implementation for arrays
     *  If called with one argument returns a string containing the way this would be defined in C/C++<br>
     *  <br>
     *  Example would be: @code "char foo[24];" @endcode<br>
     *  <br>
     *  Otherwise just returns type and length of the array, e.g. @c "uint8[12]"
     */
    static QScriptValue arrayToString(QScriptContext* ctx, QScriptEngine* eng);
    /** A toString implementation for unions.
     *  If called with one argument returns a string containing the way this would be defined in C/C++<br>
     *  Example would be:
     *  @code
     *  union foo {
     *      int16 member1;
     *      char member2;
     *   };
     *   @endcode
     *   <br>
     *   Otherwise just returns the type, which is @c "union"
     */
    static QScriptValue unionToString(QScriptContext* ctx, QScriptEngine* eng);
    /** A toString implementation for structs.
     *  If called with one argument returns a string containing the way this would be defined in C/C++<br>
     *  Example would be:<br>
     *  @code
     *  "struct foo {
     *      int16 member1;
     *      char member2;
     *   };"
     *   @endcode
     *   <br>
     *   Otherwise just returns the type, which is @c "struct"
     */
    static QScriptValue structToString(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue enumToString(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue unionOrStructToCPPString(QScriptContext* ctx, QScriptEngine* eng);
private:
    static QScriptValue primitiveConstructor(QScriptContext* ctx,
            QScriptEngine* eng, const QLatin1String type);

    static QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng);

    static const QString typePropertyString;
    static const QString toStringPropertyString;
};

#endif /* SCRIPTENGINEINITIALIZER_H_ */
