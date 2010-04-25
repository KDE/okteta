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
#include "datainformationwithchildren.h"
#include "../script/scripthandler.h"
#include "primitivefactory.h"
#include "../allprimitivetypes.h"
//QtScript
#include <QtScript/QScriptEngine>

TopLevelDataInformation::TopLevelDataInformation(DataInformation* data,
        QFileInfo structureFile, bool dynamic, QString name) :
    QObject(), mData(data), mScriptHandler(NULL), mStructureFile(structureFile),
            mWasAbleToParse(true)
{

    if (dynamic)
    {
        //dynamic object -> data is a dummy and must be calculated by the script engine.
        mScriptHandler = new ScriptHandler(structureFile.absoluteFilePath(), name);
        mData = mScriptHandler->initialDataInformationFromScript();
        if (!mData)
        {
            //just a dummy, this object should be deleted anyway
            mData = PrimitiveFactory::newInstance(
                    "failed_to_load__this_is_a_dummy", Type_Int32, 0);
            mWasAbleToParse = false;
        }
    }
    if (!mData)
        kError() << "mData == NULL!";

    setObjectName(mData->name());
    mData->setParent(this);
}
TopLevelDataInformation::TopLevelDataInformation(const TopLevelDataInformation& d) :
    QObject(), mData(d.mData->clone()), mScriptHandler(d.mScriptHandler),
            mStructureFile(d.mStructureFile), mWasAbleToParse(d.mWasAbleToParse)
{
    setObjectName(mData->name());
    mData->setParent(this);
}

TopLevelDataInformation::~TopLevelDataInformation()
{
}

void TopLevelDataInformation::validate()
{
    kDebug()
        << "validation of structure " << mData->name() << "requested";
    if (mScriptHandler)
    {
        mScriptHandler->validateData(mData);
        DataInformationWithChildren* data =
                dynamic_cast<DataInformationWithChildren*> (mData);
        if (data)
            data->calculateValidationState(); //set validation state based on children
    }
    else
    {
        kDebug()
            << "no handler available -> cannot validate structure " << mData->name();
    }

}

QScriptEngine* TopLevelDataInformation::scriptEngine() const
{
    if (!mScriptHandler)
        return NULL;
    return mScriptHandler->engine();
}

void TopLevelDataInformation::resetValidationState()
{
    mData->resetValidationState();
}

void TopLevelDataInformation::updateElement(DataInformation* elem) {
	if (!elem || !mScriptHandler)
		return;
	mScriptHandler->updateDataInformation(elem);

}

