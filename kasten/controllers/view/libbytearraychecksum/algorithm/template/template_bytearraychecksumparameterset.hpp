/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_HPP" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYCHECKSUMPARAMETERSET_HPP"
#ifndef KASTEN_TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_HPP
//// ADAPT(end)

// lib
#include "abstractbytearraychecksumparameterset.hpp"

//// ADAPT(start)
//// rename the class name from Template_ByteArrayChecksumParameterSet to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayChecksumParameterSet
class Template_ByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
//// ADAPT(end)
{
public:
    Template_ByteArrayChecksumParameterSet();

    ~Template_ByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    const char* id() const override;

//// ADAPT(start)
//// declare the parameters and the corresponding getters and setters

public: // getters
    int bitNumber() const;

public: // setters
    void setBitNumber(int bitNumber);

private: // parameters
    int mBitNumber;
//// ADAPT(end)
};

#endif
