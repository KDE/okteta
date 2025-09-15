/*
    SPDX-FileCopyrightText: 2008, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_COPYASDIALOG_HPP
#define KASTEN_COPYASDIALOG_HPP

// Qt
#include <QDialog>
// Std
#include <memory>

namespace Kasten {

class AbstractModelStreamEncoderConfigEditor;
class AbstractSelectionView;
class AbstractModelStreamEncoder;
class AbstractModelSelection;
class AbstractModel;

class CopyAsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @param configEditor the editor to embed, dialog takes ownership
     */
    CopyAsDialog(const QString& remoteTypeName, std::unique_ptr<AbstractModelStreamEncoderConfigEditor>&& configEditor,
                 AbstractModelStreamEncoder* encoder,
                 QWidget* parent = nullptr);

    ~CopyAsDialog() override;

public:
    void setData(AbstractModel* model, const AbstractModelSelection* selection);

Q_SIGNALS:
    void copyAccepted(Kasten::AbstractModelStreamEncoder* encoder,
                      const Kasten::AbstractModelSelection* selection);

private Q_SLOTS:
//     void onSelectorChanged( int index );
//     void onOffsetChanged( const QString &text );
    void onFinished(int result);

private:
    AbstractModelStreamEncoderConfigEditor* const mConfigEditor;
    AbstractModelStreamEncoder* const m_encoder;

    std::unique_ptr<AbstractSelectionView> mPreviewView;

    const AbstractModelSelection* m_selection = nullptr;
};

}

#endif
