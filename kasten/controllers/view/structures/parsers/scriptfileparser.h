/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef SCRIPTFILEPARSER_H_
#define SCRIPTFILEPARSER_H_

#include "abstractstructureparser.h"

class ScriptFileParser: public AbstractStructureParser
{
public:
    ScriptFileParser(const Kasten2::StructureDefinitionFile* const def);
    virtual ~ScriptFileParser();

    virtual QStringList parseStructureNames();
    virtual QList<const TopLevelDataInformation*> parseStructures();
    virtual bool isFullyParsed() const;
private:
    bool mParsedCompletely :1;
};

#endif /* SCRIPTFILEPARSER_H_ */
