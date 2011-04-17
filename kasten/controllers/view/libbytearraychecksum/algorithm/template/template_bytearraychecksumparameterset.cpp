/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearraychecksumparameterset.h" to the name of the header of your filter,
//// e.g. "mybytearraychecksumparameterset.h"
#include "template_bytearraychecksumparameterset.h"
//// ADAPT(end)


//// ADAPT(start)
//// define the default values for all parameters
static const int DefaultBitNumber = 1;
//// ADAPT(end)


Template_ByteArrayChecksumParameterSet::Template_ByteArrayChecksumParameterSet()
//// ADAPT(start)
//// set default values to all parameters
  : mBitNumber( DefaultBitNumber )
//// ADAPT(end)
{}

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
void Template_ByteArrayChecksumParameterSet::setBitNumber( int bitNumber ) { mBitNumber = bitNumber; }
//// ADAPT(end)

Template_ByteArrayChecksumParameterSet::~Template_ByteArrayChecksumParameterSet() {}
