/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTERPARAMETERSET_HPP" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTERPARAMETERSET_HPP"
#ifndef KASTEN_TEMPLATE_BYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_TEMPLATE_BYTEARRAYFILTERPARAMETERSET_HPP
//// ADAPT(end)

// lib
#include <abstractbytearrayfilterparameterset.hpp>

//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilterParameterSet to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilterParameterSet
class Template_ByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
//// ADAPT(end)
{
public:
    Template_ByteArrayFilterParameterSet();

    ~Template_ByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    const char* id() const override;

//// ADAPT(start)
//// declare the parameters and the corresponding getters and setters

public: // getters
    unsigned char level() const;

public: // setters
    void setLevel(unsigned int level);

private: // parameters
    unsigned char mLevel;
//// ADAPT(end)
};

#endif
