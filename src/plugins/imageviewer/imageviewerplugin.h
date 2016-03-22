/****************************************************************************
**
** Copyright (C) 2016 Denis Mingulov.
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#ifndef IMAGEVIEWERPLUGIN_H
#define IMAGEVIEWERPLUGIN_H

#include <extensionsystem/iplugin.h>

#include <QPointer>
#include <QtPlugin>

QT_BEGIN_NAMESPACE
class QAction;
class QKeySequence;
QT_END_NAMESPACE

namespace Core { class Id; }

namespace ImageViewer {
namespace Internal {

class ImageViewerFactory;

class ImageViewerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "ImageViewer.json")

public:
    ImageViewerPlugin() {}

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();

private:
    QAction *registerNewAction(Core::Id id, const QString &title, const QKeySequence &key);

    QPointer<ImageViewerFactory> m_factory;
};

} // namespace Internal
} // namespace ImageViewer

#endif // IMAGEVIEWERPLUGIN_H
