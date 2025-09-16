/*
    SPDX-FileCopyrightText: 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SCRIPTENGINEINITIALIZER_HPP
#define KASTEN_SCRIPTENGINEINITIALIZER_HPP

class QScriptEngine;
class QScriptContext;
class QScriptValue;

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
namespace ScriptEngineInitializer {

/** @return a new QScriptEngine with the functions added to the global object */
[[nodiscard]]
QScriptEngine* newEngine();
/** add the necessary functions to the QScriptEngine */
void addFuctionsToScriptEngine(QScriptEngine* engine);

namespace Private {
/** create a new enum:
 *  first parameter is the type, second parameter is a list of name - value pairs */
[[nodiscard]]
QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng);
/** create a new flag value:
 *  first parameter is the type, second parameter is a list of name - value pairs */
[[nodiscard]]
QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewUInt8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewUInt16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewUInt32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewUInt64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewInt8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewInt16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewInt32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewInt64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewBool8(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewBool16(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewBool32(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewBool64(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewFloat(QScriptContext* ctx, QScriptEngine* eng);
/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewDouble(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructors/initializer takes no parameters */
[[nodiscard]]
QScriptValue scriptNewChar(QScriptContext* ctx, QScriptEngine* eng);

/** this script constructor/initializer function takes 2 arguments:
 *  <br>
 *  -the first is the type of the bitfield: "signed", "bool" or "unsigned"<br>
 *  -the second is the width (in bits) of the bitfield)
 */
[[nodiscard]]
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
[[nodiscard]]
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
[[nodiscard]]
QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor/initializer function takes 2 arguments and returns an array:<br>
 *  -the first is an object of the type of the array (can also be a struct or a union or even another array)<br>
 *  -the second is the length of the array<br>
 */
[[nodiscard]]
QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes one argument, the encoding of the string (as a string) */
[[nodiscard]]
QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes two arguments: 1. type of the pointer and 2. the target type */
[[nodiscard]]
QScriptValue scriptNewPointer(QScriptContext* ctx, QScriptEngine* eng);
/** this constructor takes three arguments: 1. initial fields 2. the alternatives 3. (optional) default fields */
[[nodiscard]]
QScriptValue scriptNewTaggedUnion(QScriptContext* ctx, QScriptEngine* eng);

[[nodiscard]]
QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng);
[[nodiscard]]
QScriptValue addUpdateFunc(QScriptContext* ctx, QScriptEngine* eng);
[[nodiscard]]
QScriptValue addValidationFunc(QScriptContext* ctx, QScriptEngine* eng);
[[nodiscard]]
QScriptValue alternativeFunc(QScriptContext* ctx, QScriptEngine* eng);
[[nodiscard]]
QScriptValue importScriptFunc(QScriptContext* ctx, QScriptEngine* eng);

/** this allows you to write e.g.
 *  return struct({foo : uint8() })
 *      .set({ updateFunc : ...,
 *             name : "something"})
 *
 */
[[nodiscard]]
QScriptValue addCustomPropertiesFunc(QScriptContext* ctx, QScriptEngine* eng);

}

}

#endif /* KASTEN_SCRIPTENGINEINITIALIZER_HPP */
