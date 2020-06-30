/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearrayfilter.hpp" to the name of the header of your filter,
//// e.g. "mybytearrayfilter.hpp"
#include "template_bytearrayfilterparameterset.hpp"
//// ADAPT(end)

//// ADAPT(start)
//// define the default values for all parameters
static constexpr unsigned int DefaultLevel = 127;
//// ADAPT(end)

Template_ByteArrayFilterParameterSet::Template_ByteArrayFilterParameterSet()
//// ADAPT(start)
//// set default values to all parameters
    : mLevel(DefaultLevel)
//// ADAPT(end)
{}

Template_ByteArrayFilterParameterSet::~Template_ByteArrayFilterParameterSet() = default;

//// ADAPT(start)
//// rename "Template_ParameterSetId" to a unique identifier,
//// e.g. "MyParameterSet"
const char* Template_ByteArrayFilterParameterSet::id() const { return "Template_ParameterSetId"; }
//// ADAPT(end)

//// ADAPT(start)
//// implement the getters for all parameters
unsigned char Template_ByteArrayFilterParameterSet::level() const { return mLevel; }
//// ADAPT(end)

//// ADAPT(start)
//// implement the setters for all parameters
void Template_ByteArrayFilterParameterSet::setLevel(unsigned int level) { mLevel = level; }
//// ADAPT(end)
