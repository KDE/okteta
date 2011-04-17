/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTERPARAMETERSET_H" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTERPARAMETERSET_H"
#ifndef TEMPLATE_BYTEARRAYFILTERPARAMETERSET_H
#define TEMPLATE_BYTEARRAYFILTERPARAMETERSET_H
//// ADAPT(end)

// lib
#include <abstractbytearrayfilterparameterset.h>


//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilterParameterSet to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilterParameterSet
class Template_ByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
//// ADAPT(end)
{
  public:
    Template_ByteArrayFilterParameterSet();

    virtual ~Template_ByteArrayFilterParameterSet();

  public: // AbstractByteArrayFilterParameterSet API
    virtual const char* id() const;

//// ADAPT(start)
//// declare the parameters and the corresponding getters and setters
  public: // getters
    unsigned char level() const;

  public: // setters
    void setLevel( unsigned int level );

  protected: // parameters
    unsigned char mLevel;
//// ADAPT(end)
};

#endif
