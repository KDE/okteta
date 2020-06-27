/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTER_HPP" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTER_HPP"
#ifndef KASTEN_TEMPLATE_BYTEARRAYFILTER_HPP
#define KASTEN_TEMPLATE_BYTEARRAYFILTER_HPP
// ADAPT(end)

// lib
//// ADAPT(start)
//// change "template_bytearrayfilterparameterset.hpp" to the name of the header of the used parameterset
//// in case of no parameters use "nobytearrayfilterparameterset.hpp"
#include "template_bytearrayfilterparameterset.hpp"
//// ADAPT(end)
#include <abstractbytearrayfilter.hpp>

//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilter to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilter
class Template_ByteArrayFilter : public AbstractByteArrayFilter
//// ADAPT(end)
{
    Q_OBJECT

public:
    Template_ByteArrayFilter();

    ~Template_ByteArrayFilter() override;

public: // AbstractByteArrayFilter API
    bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayFilterParameterSet* parameterSet() override;

private:
//// ADAPT(start)
//// change "Template_ByteArrayFilterParameterSet" to the name of the class of the used parameterset
//// in case of no parameters use "NoByteArrayFilterParameterSet"
    Template_ByteArrayFilterParameterSet mParameterSet;
//// ADAPT(end)
};

#endif
