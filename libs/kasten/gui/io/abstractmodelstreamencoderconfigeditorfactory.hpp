/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_ABSTRACTMODELSTREAMENCODERCONFIGEDITORFACTORY_HPP

// lib
#include "kastengui_export.hpp"

namespace Kasten {

class AbstractModelStreamEncoderConfigEditor;
class AbstractModelStreamEncoder;

class KASTENGUI_EXPORT AbstractModelStreamEncoderConfigEditorFactory
{
public:
    virtual ~AbstractModelStreamEncoderConfigEditorFactory();

public:
    virtual AbstractModelStreamEncoderConfigEditor* tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const = 0;
};

inline AbstractModelStreamEncoderConfigEditorFactory::~AbstractModelStreamEncoderConfigEditorFactory() = default;

}

#endif
