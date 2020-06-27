/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// program
#include "program.hpp"

int main(int argc, char* argv[])
{
    Kasten::OktetaProgram program(argc, argv);

    const int result = program.execute();

    return result;
}
