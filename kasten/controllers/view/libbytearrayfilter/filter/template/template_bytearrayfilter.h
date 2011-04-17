/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTER_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTER_H"
#ifndef TEMPLATE_BYTEARRAYFILTER_H
#define TEMPLATE_BYTEARRAYFILTER_H
// ADAPT(end)

// lib
//// ADAPT(start)
//// change "template_bytearrayfilterparameterset.h" to the name of the header of the used parameterset
//// in case of no parameters use "nobytearrayfilterparameterset.h"
#include "template_bytearrayfilterparameterset.h"
//// ADAPT(end)
#include <abstractbytearrayfilter.h>


//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilter to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilter
class Template_ByteArrayFilter : public AbstractByteArrayFilter
//// ADAPT(end)
{
  public:
    Template_ByteArrayFilter();

    virtual ~Template_ByteArrayFilter();

  public: // AbstractByteArrayFilter API
    virtual bool filter( Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const;
    virtual AbstractByteArrayFilterParameterSet* parameterSet();

  protected:
//// ADAPT(start)
//// change "Template_ByteArrayFilterParameterSet" to the name of the class of the used parameterset
//// in case of no parameters use "NoByteArrayFilterParameterSet"
    Template_ByteArrayFilterParameterSet mParameterSet;
//// ADAPT(end)
};

#endif
