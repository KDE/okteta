/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "topleveldatainformation.h"
#include "../script/scripthandler.h"
#include "primitivedatainformation.h"
#include "../allprimitivetypes.h"
//QtScript
#include <QtScript/QScriptEngine>

TopLevelDataInformation::TopLevelDataInformation(const DataInformation* data,
        QFileInfo structureFile, bool dynamic, QString name) :
    DataInformation(name, 0), mScriptHandler(NULL), mStructureFile(structureFile),            mWasAbleToParse (true)
{

    if (dynamic)
    {
        //dynamic object -> data is a dummy and must be calculated by the script engine.
        mScriptHandler = new ScriptHandler(structureFile.absoluteFilePath(), name);
        mData = mScriptHandler->initialDataInformationFromScript();
        if (!mData)
        {
            //just a dummy, this object should be deleted anyway
            mWasAbleToParse = false;
            mData = PrimitiveDataInformation::newInstance(
                    "failed_to_load__this_is_a_dummy", Type_Int32, 0, this);
            mWasAbleToParse = false;
        }
    }
    else
    {
        mData = data->clone();
    }
    setObjectName(mData->name());
    mData->setParent(this);
}

TopLevelDataInformation::~TopLevelDataInformation()
{
}

bool TopLevelDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    if (this != inf)
        return false;
    //correct object -> use mValue so PrimitiveDataInformation::setData() returns true
    bool ret = mData->setData(value, mData, out, byteOrder, address, bitsRemaining,
            bitOffset);
    return ret;
}

qint64 TopLevelDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, quint64 bitsRemaining,
        quint8* bitOffset)
{
    return mData->readData(input, byteOrder, address, bitsRemaining, bitOffset);
}

TopLevelDataInformation* TopLevelDataInformation::topLevelDataInformation()
{
    return this;
}

