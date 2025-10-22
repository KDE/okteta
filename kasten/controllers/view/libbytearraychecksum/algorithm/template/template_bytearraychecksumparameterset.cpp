/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearraychecksumparameterset.hpp" to the name of the header of your filter,
//// e.g. "mybytearraychecksumparameterset.hpp"
#include "template_bytearraychecksumparameterset.hpp"
//// ADAPT(end)

//// ADAPT(start)
//// define the default value static members
// C++11 needs a definition for static constexpr members
constexpr int Template_ByteArrayChecksumParameterSet::DefaultBitNumber;
//// ADAPT(end)

Template_ByteArrayChecksumParameterSet::Template_ByteArrayChecksumParameterSet() = default;

Template_ByteArrayChecksumParameterSet::~Template_ByteArrayChecksumParameterSet() = default;

//// ADAPT(start)
//// rename "Template_ParameterSetId" to a unique identifier,
//// e.g. "MyParameterSet"
const char* Template_ByteArrayChecksumParameterSet::id() const { return "Template_ParameterSetId"; }
//// ADAPT(end)

//// ADAPT(start)
//// implement the getters for all parameters
int Template_ByteArrayChecksumParameterSet::bitNumber() const { return mBitNumber; }
//// ADAPT(end)

//// ADAPT(start)
//// implement the setters for all parameters
void Template_ByteArrayChecksumParameterSet::setBitNumber(int bitNumber) { mBitNumber = bitNumber; }
//// ADAPT(end)
