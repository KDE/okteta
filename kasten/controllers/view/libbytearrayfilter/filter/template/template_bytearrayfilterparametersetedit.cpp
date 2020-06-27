/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    Public domain.
*/

//// ADAPT(start)
//// rename "template_bytearrayfilterparametersetedit.hpp" to the name of the header of your filter,
//// e.g. "mybytearrayfilterparametersetedit.hpp"
#include "template_bytearrayfilterparametersetedit.hpp"
//// ADAPT(end)

// parameterset
//// ADAPT(start)
//// rename "template_bytearrayfilterparameterset.hpp" to the name of the header of your filter,
//// e.g. "mybytearrayfilterparameterset.hpp"
#include "template_bytearrayfilterparameterset.hpp"
//// ADAPT(end)
// KF
#include <KLocalizedString>
//// ADAPT(start)
//// add includes for all elements used in the widget
#include <QSpinBox>
// Qt
#include <QFormLayout>
//// ADAPT(end)

//// ADAPT(start)
//// rename "Template_ParameterSetId" to the id of your parameterset,
//// e.g. "MyParameterSet"
const char Template_ByteArrayFilterParameterSetEdit::Id[] = "Template_ParameterSetId";
//// ADAPT(end)

Template_ByteArrayFilterParameterSetEdit::Template_ByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{
//// ADAPT(start)
//// setup the widget with all edit fields needed for the parameter set
//// if there can be invalid states connect the change signals of the edit fields to some slots
//// where you check if the validity changed
    auto* baseLayout = new QFormLayout(this);
    // margin is provided by the container for this widget
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mLevelEdit = new QSpinBox(this);
    // For demonstration purpose we start at -1, not 0, to show handling of an invalid state
    // Otherwise the range should start at 0 and there is no need to connect to the valueChanged signal
    mLevelEdit->setRange(-1, 256);
    // start with the invalid number
    mLevelEdit->setValue(-1);
    connect(mLevelEdit, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Template_ByteArrayFilterParameterSetEdit::onLevelChanged);

    const QString levelLabelText =
        i18nc("@label:spinbox decimal value up to which bytes are set to 0",
              "Level:");
    const QString levelToolTip =
        i18nc("@info:tooltip",
              "The decimal value up to which the bytes are set to x00. Bytes above this value are set to x01.");
    mLevelEdit->setToolTip(levelToolTip);
    const QString levelWhatsThis =
        i18nc("@info:whatsthis",
              "Control the value which decides how the bytes are ending up. And more explanation.");
    mLevelEdit->setWhatsThis(levelWhatsThis);

    baseLayout->addRow(levelLabelText, mLevelEdit);

    // note start state
    mIsValid = isValid();
//// ADAPT(end)
}

Template_ByteArrayFilterParameterSetEdit::~Template_ByteArrayFilterParameterSetEdit() = default;

//// ADAPT(start)
//// if invalid states are possible implement here the check
bool Template_ByteArrayFilterParameterSetEdit::isValid() const { return mLevelEdit->value() != -1; }
//// ADAPT(end)

//// ADAPT(start)
//// change "Template_ByteArrayFilterParameterSet" to the class of the parameter set which this widget should edit
//// e.g. "MyByteArrayFilterParameterSet"
//// also adapt the passing of the values between the parameter set and the edit fields
void Template_ByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    const auto* template_ParameterSet = static_cast<const Template_ByteArrayFilterParameterSet*>(parameterSet);

    mLevelEdit->setValue(template_ParameterSet->level());
}

void Template_ByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    auto* template_ParameterSet = static_cast<Template_ByteArrayFilterParameterSet*>(parameterSet);

    template_ParameterSet->setLevel(mLevelEdit->value());
}
//// ADAPT(end)

//// ADAPT(start)
//// define the slots to catch changes in the values to check if the current state is valid or not
//// not needed if there cannot be invalid states
void Template_ByteArrayFilterParameterSetEdit::onLevelChanged(int value)
{
    const bool isValid = (value != -1);

    if (mIsValid == isValid) {
        return;
    }

    mIsValid = isValid;
    emit validityChanged(isValid);
}
//// ADAPT(end)
