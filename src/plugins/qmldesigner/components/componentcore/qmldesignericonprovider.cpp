/****************************************************************************
**
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

#include "qmldesignericonprovider.h"

#include <utils/icon.h>

#include <coreplugin/coreicons.h>
#include <coreplugin/icore.h>

#include <QDebug>

namespace QmlDesigner {

QmlDesignerIconProvider::QmlDesignerIconProvider()
    : QQuickImageProvider(Pixmap)
{

}

static QString iconPath()
{
    return Core::ICore::resourcePath() + QLatin1String("/qmldesigner/propertyEditorQmlSources/HelperWidgets/images/");
}

QPixmap QmlDesignerIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize)

    using namespace Utils;

    QPixmap result;

    if (id == "close")
        result = Core::Icons::CLOSE_TOOLBAR.pixmap();
    else if (id == "plus")
        result = Core::Icons::PLUS.pixmap();
    else if (id == "expression")
        result = Icon({
                { iconPath() + QLatin1String("expression.png"), Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "placeholder")
        result = Icon({
                { iconPath() + QLatin1String("placeholder.png"), Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "submenu")
        result = Icon({
                { iconPath() + QLatin1String("submenu.png"), Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "up-arrow")
        result = Icon({
                { iconPath() + QLatin1String("up-arrow.png"), Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "down-arrow")
        result = Icon({
                { iconPath() + QLatin1String("down-arrow.png"), Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "checkbox-indicator")
        result = Icon({
                { ":/qmldesigner/images/checkbox_indicator.png", Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else if (id == "tr")
        result = Icon({
                { ":/qmldesigner/images/tr.png", Theme::IconsBaseColor}}, Icon::Tint).pixmap();
    else
        qWarning() << Q_FUNC_INFO << "Image not found:" << id;

    if (size)
        *size = result.size();
    return result;
}

} // namespace QmlDesigner
