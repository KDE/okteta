/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "bytearraystreamencoderconfigeditorfactoryfactory.h"

// lib
#include "streamencoder/sourcecode//bytearraysourcecodestreamencoderconfigeditorfactory.h"
#include "streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.h"
#include "streamencoder/base32/bytearraybase32streamencoderconfigeditorfactory.h"
#include "streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditorfactory.h"
#include "streamencoder/xxencoding/bytearrayxxencodingstreamencoderconfigeditorfactory.h"
#include "streamencoder/srec/bytearraysrecstreamencoderconfigeditorfactory.h"
#include "streamencoder/ihex/bytearrayihexstreamencoderconfigeditorfactory.h"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.h"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)
// Qt
#include <QtCore/QList>


namespace Kasten2
{

QList<AbstractModelStreamEncoderConfigEditorFactory*> ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys()
{
    QList<AbstractModelStreamEncoderConfigEditorFactory*> result;

    result
        << new ByteArraySourceCodeStreamEncoderConfigEditorFactory()
        << new ByteArrayValuesStreamEncoderConfigEditorFactory()
        << new ByteArrayBase32StreamEncoderConfigEditorFactory()
        << new ByteArraySRecStreamEncoderConfigEditorFactory()
        << new ByteArrayIHexStreamEncoderConfigEditorFactory()
        << new ByteArrayUuencodingStreamEncoderConfigEditorFactory()
        << new ByteArrayXxencodingStreamEncoderConfigEditorFactory();
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////     result
////         << new My_ByteArrayStreamEncoderConfigEditorFactory();
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)

    return result;
}

}
