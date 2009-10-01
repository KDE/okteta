/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearrayfilter.h" to the name of the header of your filter,
//// e.g. "mybytearrayfilter.h"
#include "template_bytearraychecksumalgorithm.h"
//// ADAPT(end)

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


Template_ByteArrayChecksumAlgorithm::Template_ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
//// ADAPT(start)
//// change "TEMPLATE OPERATION ON" to a short and descriptive name of the operation of this filter, also in upper case
     i18nc("name of the checksum algorithm", "Adler-32") )
//// ADAPT(end)
{}

AbstractByteArrayChecksumParameterSet* Template_ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Template_ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                           const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    bool success = true;

//// ADAPT(start)
//// modify the following code to calculate the checksum/hashsum.
//// The final checksum is passed as a QString to result.
    const int mask = ( 1 << mParameterSet.bitNumber() );

    int sum = 0;

    // TODO: this is the "inefficient but straightforward implementation" from the Wikipedia entry, search for improved
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        sum ^= ( model->byte(i) & mask );

        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }

    *result = QString::fromLatin1("%1").arg( sum );
//// ADAPT(end)

    return success;
}

Template_ByteArrayChecksumAlgorithm::~Template_ByteArrayChecksumAlgorithm() {}
