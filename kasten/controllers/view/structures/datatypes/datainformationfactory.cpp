/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "datainformationfactory.h"

#include "primitive/bitfield/boolbitfielddatainformation.h"
#include "primitive/bitfield/unsignedbitfielddatainformation.h"
#include "primitive/bitfield/signedbitfielddatainformation.h"

#include "../script/scriptlogger.h"

AbstractBitfieldDataInformation* DataInformationFactory::newBitfield(const BitfieldParsedData& pd)
{
    if (pd.widthStr.isEmpty())
    {
        pd.error() << "There seems to be no width specified for this bitfield.";
        return 0;
    }
    if (!pd.widthConversionOkay)
    {
       pd.error() << "It seems that the width is incorrect since '"
                << pd.widthStr << "' could not be converted to an integer.";
        return 0;
    }
    if (pd.width <= 0 || pd.width > 64)
    {
        pd.error() << "Width of bitfield is not a value from 1-64:" << pd.width;
        return 0;
    }
    AbstractBitfieldDataInformation* bitf = 0;
    const QString type = pd.type.toLower();
    if (type.isEmpty())
    {
        pd.info() << "no bitfield type specified, defaulting to unsigned.";
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width, pd.parent);
    }
    else if (type == QLatin1String("bool"))
        bitf = new BoolBitfieldDataInformation(pd.name, pd.width, pd.parent);
    else if (type == QLatin1String("unsigned"))
        bitf = new UnsignedBitfieldDataInformation(pd.name, pd.width, pd.parent);
    else if (type == QLatin1String("signed"))
        bitf = new SignedBitfieldDataInformation(pd.name, pd.width, pd.parent);
    else
    {
        pd.error() << "invalid bitfield type attribute given:" << type;
        return 0;
    }
    return bitf;
}
