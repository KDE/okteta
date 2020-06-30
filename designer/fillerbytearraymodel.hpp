/*
    This file is part of the Okteta Designer plugin, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FILLERBYTEARRAYMODEL_HPP
#define FILLERBYTEARRAYMODEL_HPP

namespace Okteta {
class AbstractByteArrayModel;
}
class QObject;

extern Okteta::AbstractByteArrayModel* createFillerByteArrayModel(QObject* parent);

#endif
