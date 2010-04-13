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
#ifndef TOPLEVELDATAINFORMATION_H_
#define TOPLEVELDATAINFORMATION_H_

#include "datainformation.h"

#include <QtCore/QFileInfo>
#include <QtCore/QExplicitlySharedDataPointer>

class QScriptEngine;
class ScriptHandler;
class TopLevelDataInformation: public QObject
{
Q_OBJECT
public:
    /** creates a new TopLevelDataInformation object wrapping the existing DataInformation @p data.
     *  @param data the object to wrap (ownership is taken)
     *  @param scriptFile the file which contains the file this object was initialized from
     *  @param dynamic whether the wrapped object is a dynamic structure definition (i.e. one using QtScript)
     */
    TopLevelDataInformation(DataInformation* data, QFileInfo structureFile,
            bool dynamic = false, QString name = QString());
    TopLevelDataInformation(const TopLevelDataInformation& d);
    virtual ~TopLevelDataInformation();
    TopLevelDataInformation* clone() const;
public:
    void validate();
    QScriptEngine* scriptEngine() const;

    DataInformation* actualDataInformation() const;
    bool wasAbleToParse() const;


public Q_SLOTS:
    void resetValidationState();
    //the new methods of topLevelDataInformation

private:
    DataInformation* mData;
    QExplicitlySharedDataPointer<ScriptHandler> mScriptHandler;
    QFileInfo mStructureFile;
    bool mWasAbleToParse :1;
};

inline DataInformation* TopLevelDataInformation::actualDataInformation() const
{
    return mData;
}

inline bool TopLevelDataInformation::wasAbleToParse() const
{
    return mWasAbleToParse;
}

inline TopLevelDataInformation* TopLevelDataInformation::clone() const
{
    return new TopLevelDataInformation(*this);
}
#endif /* TOPLEVELDATAINFORMATION_H_ */
