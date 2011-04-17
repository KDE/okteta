/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearrayfilter.h" to the name of the header of your filter,
//// e.g. "mybytearrayfilter.h"
#include "template_bytearrayfilter.h"
//// ADAPT(end)

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


Template_ByteArrayFilter::Template_ByteArrayFilter()
  : AbstractByteArrayFilter(
//// ADAPT(start)
//// change "TEMPLATE OPERATION ON" to a short and descriptive name of the operation of this filter, also in upper case
        i18nc("name of the filter; it does a TEMPLATE OPERATION ON operation",
              "TEMPLATE OPERATION ON data") )
//// ADAPT(end)
{}

AbstractByteArrayFilterParameterSet* Template_ByteArrayFilter::parameterSet() { return &mParameterSet; }

bool Template_ByteArrayFilter::filter( Okteta::Byte* result,
                                       Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    bool success = true;

//// ADAPT(start)
//// modify the following code to write the filtered bytes into the array pointed to by result
//// The array is as large as range and not initialized to a value.

    const unsigned int level = mParameterSet.level();

    int r = 0;
    Okteta::Address m = range.start();
    int nextBlockEnd = FilteredByteCountSignalLimit;
    while( m <= range.end() )
    {
        // example code turns all bytes larger than level into 255, the others to 0
        const Okteta::Byte byte = model->byte( m++ );
        const Okteta::Byte resultByte = ( byte > level ) ? 255 : 0;
        result[r++] = resultByte;

        if( r >= nextBlockEnd )
        {
            nextBlockEnd += FilteredByteCountSignalLimit;
            emit filteredBytes( r );
        }
    }
//// ADAPT(end)

    return success;
}

Template_ByteArrayFilter::~Template_ByteArrayFilter() {}
