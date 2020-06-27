/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_HPP" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYCHECKSUMALGORITHM_HPP"
#ifndef KASTEN_TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_TEMPLATE_BYTEARRAYCHECKSUMALGORITHM_HPP
//// ADAPT(end)

// lib
//// ADAPT(start)
//// change "template_bytearraychecksumparameterset.hpp" to the name of the header of the used parameterset
//// in case of no parameters use "nobytearraychecksumparameterset.hpp"
#include "template_bytearraychecksumparameterset.hpp"
//// ADAPT(end)
#include <abstractbytearraychecksumalgorithm.hpp>

//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilter to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilter
class Template_ByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
//// ADAPT(end)
{
    Q_OBJECT

public:
    Template_ByteArrayChecksumAlgorithm();

    ~Template_ByteArrayChecksumAlgorithm() override;

public: // AbstractByteArrayChecksumAlgorithm API
    bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayChecksumParameterSet* parameterSet() override;

private:
//// ADAPT(start)
//// change "Template_ByteArrayChecksumParameterSet" to the name of the class of the used parameterset
//// in case of no parameters use "NoByteArrayChecksumParameterSet"
    Template_ByteArrayChecksumParameterSet mParameterSet;
//// ADAPT(end)
};

#endif
