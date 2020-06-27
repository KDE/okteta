/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONVIEWTOOL_HPP
#define KASTEN_VERSIONVIEWTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>

namespace Kasten {

/**
*/
class VersionViewTool : public AbstractTool
{
    Q_OBJECT

public:
    VersionViewTool();
    ~VersionViewTool() override;

public:
    AbstractModel* model() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

Q_SIGNALS:
    void modelChanged(Kasten::AbstractModel* model);

private: // sources
    AbstractModel* mModel = nullptr;
};

inline AbstractModel* VersionViewTool::model() const { return mModel; }

}

#endif
