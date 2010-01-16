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
#include "chardatainformation.h"
#include <KLineEdit>

QString CharDataInformation::getValueString() const
{
    if (!mIsValid)
        return i18nc("invalid value (out of range)", "<invalid>");
    //TODO char codec
    QChar qchar(mValue.ubyteValue, 0);
    qchar = qchar.isPrint() ? qchar : QChar(QChar::ReplacementCharacter);
    QString charStr = '\'' + qchar + '\'';
    if (Kasten::StructViewPreferences::showCharNumericalValue())
    {
        int base = displayBase();
        QString num = QString::number(mValue.ubyteValue, base);
        if (base == 16)
            num = "0x" + num;
        if (Kasten::StructViewPreferences::localeAwareDecimalFormatting() && base
                == 10)
            num = KGlobal::locale()->formatNumber(num, false, 0);
        charStr += " (" + num + ')';
    }
    return charStr;
}

QWidget* CharDataInformation::createEditWidget(QWidget* parent) const
{
    return new KLineEdit(parent);
}

QVariant CharDataInformation::dataFromWidget(const QWidget* w) const
{
    const KLineEdit* edit = dynamic_cast<const KLineEdit*> (w);
    if (edit)
    {
        QString text = edit->text();
        if (text.length() == 0)
        {
            return QVariant();
        }
        if (text.length() == 1)
        {
            //TODO char codec
            return (unsigned char) text.at(0).toLatin1();
        }
        if (text.at(0) == '\\')
        {
            //escape sequence
            if (text.at(1) == 'x')
            {
                //hex escape:
                bool okay;
                QString valStr = text.mid(2, 2); //only 2 chars
                quint8 val = valStr.toInt(&okay, 16);
                if (okay)
                    return val;
                else
                    return QVariant();
            }
            else if (text.at(1) == 'n')
            {
                return (quint8) '\n'; //newline
            }
            else if (text.at(1) == 't')
            {
                return (quint8) '\t'; //tab
            }
            else if (text.at(1) == 'r')
            {
                return (quint8) '\r'; //cr
            }
            else
            {
                //octal escape:
                bool okay;
                QString valStr = text.mid(1, 3); //only 2 chars
                quint8 val = valStr.toInt(&okay, 8);
                if (okay)
                    return val;
                else
                    return QVariant();
            }
        }
    }
    return QVariant();
}

void CharDataInformation::setWidgetData(QWidget* w) const
{
    KLineEdit* edit = dynamic_cast<KLineEdit*> (w);
    if (edit)
    {
        QChar qchar = mValue.ubyteValue;
        if (! qchar.isPrint())
            qchar = QChar(QChar::ReplacementCharacter);
        edit->setText( qchar );
    }
}
