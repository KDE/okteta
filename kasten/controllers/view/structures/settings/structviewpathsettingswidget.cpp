/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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
#include "structviewpathsettingswidget.h"
#include <KFileDialog>
#include <KStandardDirs>
StructViewPathSettingsWidget::StructViewPathSettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.addFilesButton, SIGNAL(pressed()), this, SLOT(onAddFilesClicked()));
    connect(ui.removeAllButton, SIGNAL(pressed()), this, SLOT(onRemoveFilesClicked()));
}

StructViewPathSettingsWidget::~StructViewPathSettingsWidget()
{

}

void StructViewPathSettingsWidget::onAddFilesClicked()
{
    QString baseDir = KGlobal::dirs()->locateLocal("data", "okteta/structures/", true);
    QStringList files = KFileDialog::getOpenFileNames(baseDir, i18n(
            "*.osd|Okteta structure definition XML files (*.osd)"), this);
    for (int i = 0; i < files.size(); ++i)
    {
        QString path = files.at(i);
        if (path.startsWith(baseDir))
        {
            path = path.right(path.length() - baseDir.length());
        }
        if (!ui.kcfg_StructureDefFiles->items().contains(path, Qt::CaseSensitive))
            ui.kcfg_StructureDefFiles->insertItem(path,
                    ui.kcfg_StructureDefFiles->count());
    }
}
void StructViewPathSettingsWidget::onRemoveFilesClicked()
{
    ui.kcfg_StructureDefFiles->clear();
}
