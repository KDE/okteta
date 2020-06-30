/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007, 2009, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACEPROMPT_HPP
#define KASTEN_REPLACEPROMPT_HPP

// controller
#include "replaceuserqueryable.hpp"
// Qt
#include <QDialog>

namespace Kasten {

class ReplacePrompt : public QDialog
{
    Q_OBJECT

public:
    explicit ReplacePrompt(QWidget* parent);

Q_SIGNALS:
    void finished(Kasten::ReplaceBehaviour behaviour);

private Q_SLOTS:
    void onReplaceAllButton();
    void onSkipButton();
    void onReplaceButton();
    void onCloseButton();
};

}

#endif
