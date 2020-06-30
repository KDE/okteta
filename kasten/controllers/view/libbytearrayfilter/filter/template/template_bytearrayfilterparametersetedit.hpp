/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "TEMPLATE_BYTEARRAYFILTERWITHPARAMETERSETEDIT_HPP" to an include guard matching your class/file name
//// e.g. "MYBYTEARRAYFILTERWITHPARAMETERSETEDIT_HPP"
#ifndef KASTEN_TEMPLATE_BYTEARRAYFILTERPARAMETERSETEDIT_HPP
#define KASTEN_TEMPLATE_BYTEARRAYFILTERPARAMETERSETEDIT_HPP
//// ADAPT(end)

// lib
#include <abstractbytearrayfilterparametersetedit.hpp>

//// ADAPT(start)
//// add here all needed headers and forward declarations for the members of the widget
class QSpinBox;
//// ADAPT(end)

//// ADAPT(start)
//// rename the class name from Template_ByteArrayFilterParameterSetEdit to a proper name, both here and in the .cpp file
//// e.g. MyByteArrayFilterParameterSetEdit
class Template_ByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
//// ADAPT(end)
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit Template_ByteArrayFilterParameterSetEdit(QWidget* parent = nullptr);

    ~Template_ByteArrayFilterParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setValues(const AbstractByteArrayFilterParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const override;
//// ADAPT(start)
//// if the values in the widget can reach invalid states, which should not be used for the parameters,
//// keep the declaration of "isValid()", otherwise remove the next line
    bool isValid() const override;
//// ADAPT(end)

//// ADAPT(start)
//// define slots to catch changes in the values to check if the current state is valid or not
//// not needed if there cannot be invalid states

private Q_SLOTS:
    void onLevelChanged(int value);
//// ADAPT(end)

//// ADAPT(start)
//// here list the UI elements as usual

private:
    QSpinBox* mLevelEdit;

    bool mIsValid : 1;
//// ADAPT(end)
};

#endif
