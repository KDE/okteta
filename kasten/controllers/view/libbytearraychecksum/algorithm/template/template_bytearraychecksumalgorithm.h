/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYCHECKSUMALGORITHM_H"
#ifndef TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_H
#define TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_H
//// ADAPT(end)

// lib
//// ADAPT(start)
//// change "template_bytearraychecksumparameterset.h" to the name of the header of the used parameterset
//// in case of no parameters use "nobytearraychecksumparameterset.h"
#include "template_bytearraychecksumparameterset.h"
//// ADAPT(end)
#include <abstractbytearraychecksumalgorithm.h>


//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilter to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilter
class Template_ByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
//// ADAPT(end)
{
  public:
    Template_ByteArrayChecksumAlgorithm();

    virtual ~Template_ByteArrayChecksumAlgorithm();

  public: // AbstractByteArrayChecksumAlgorithm API
    virtual bool calculateChecksum( QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const;
    virtual AbstractByteArrayChecksumParameterSet* parameterSet();

  protected:
//// ADAPT(start)
//// change "Template_ByteArrayChecksumParameterSet" to the name of the class of the used parameterset
//// in case of no parameters use "NoByteArrayChecksumParameterSet"
    Template_ByteArrayChecksumParameterSet mParameterSet;
//// ADAPT(end)
};

#endif
