/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modelencoderfilesystemexporter.h"

// lib
#include "abstractmodelstreamencoder.h"
// Qt
#include <QtCore/QDataStream>
#include <QtCore/QFile>

ModelEncoderFileSystemExporter::ModelEncoderFileSystemExporter( AbstractModelStreamEncoder *encoder )
: AbstractModelFileSystemExporter(encoder->remoteTypeName(),encoder->remoteMimeType()), mEncoder( encoder )
{}

QString ModelEncoderFileSystemExporter::modelTypeName( AbstractModel *model, const AbstractModelSelection *selection ) const
{
    return mEncoder->modelTypeName( model, selection );
}

bool ModelEncoderFileSystemExporter::writeToFile( AbstractModel *model, const AbstractModelSelection *selection,
                                                  const QString &workFilePath )
{
    QFile file( workFilePath );
    file.open( QIODevice::WriteOnly );

    const bool success =
//     if( selection )
//         mEncoder->encodeToStream( &file, selection );
//     else
        mEncoder->encodeToStream( &file, model, selection );

    return success;
}

ModelEncoderFileSystemExporter::~ModelEncoderFileSystemExporter() {}
