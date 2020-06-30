/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraystreamencoderconfigeditorfactoryfactory.hpp"

// lib
#include "streamencoder/sourcecode//bytearraysourcecodestreamencoderconfigeditorfactory.hpp"
#include "streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/base32/bytearraybase32streamencoderconfigeditorfactory.hpp"
#include "streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/xxencoding/bytearrayxxencodingstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/srec/bytearraysrecstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/ihex/bytearrayihexstreamencoderconfigeditorfactory.hpp"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.hpp"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)
// Qt
#include <QVector>

namespace Kasten {

QVector<AbstractModelStreamEncoderConfigEditorFactory*> ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys()
{
    const QVector<AbstractModelStreamEncoderConfigEditorFactory*> result {
        new ByteArraySourceCodeStreamEncoderConfigEditorFactory(),
        new ByteArrayValuesStreamEncoderConfigEditorFactory(),
        new ByteArrayBase32StreamEncoderConfigEditorFactory(),
        new ByteArraySRecStreamEncoderConfigEditorFactory(),
        new ByteArrayIHexStreamEncoderConfigEditorFactory(),
        new ByteArrayUuencodingStreamEncoderConfigEditorFactory(),
        new ByteArrayXxencodingStreamEncoderConfigEditorFactory(),
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////         new My_ByteArrayStreamEncoderConfigEditorFactory(),
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)
    };
    return result;
}

}
