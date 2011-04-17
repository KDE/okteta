/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYCHECKSUMPARAMETERSET_H"
#ifndef TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_H
#define TEMPLATE_BYTEARRAYCHECKSUMPARAMETERSET_H
//// ADAPT(end)

// lib
#include "abstractbytearraychecksumparameterset.h"


//// ADAPT(start)
//// rename the class name from Template_ByteArrayChecksumParameterSet to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayChecksumParameterSet
class Template_ByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
//// ADAPT(end)
{
  public:
    Template_ByteArrayChecksumParameterSet();

    virtual ~Template_ByteArrayChecksumParameterSet();

  public: // AbstractByteArrayChecksumParameterSet API
    virtual const char* id() const;

//// ADAPT(start)
//// declare the parameters and the corresponding getters and setters
  public: // getters
    int bitNumber() const;

  public: // setters
    void setBitNumber( int bitNumber );

  protected: // parameters
    int mBitNumber;
//// ADAPT(end)
};

#endif
