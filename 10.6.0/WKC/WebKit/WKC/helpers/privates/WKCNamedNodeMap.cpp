/*
 *  Copyright (c) 2011-2013 ACCESS CO., LTD. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifdef WKC_ADD_WKC_HELPERS

#include "config.h"

#include "helpers/WKCNamedNodeMap.h"
#include "helpers/privates/WKCNamedNodeMapPrivate.h"

#include "helpers/WKCQualifiedName.h"
#include "helpers/privates/WKCQualifiedNamePrivate.h"
#include "helpers/privates/WKCAttributePrivate.h"

#include "NamedNodeMap.h"
#include "Node.h"

namespace WKC {

NamedNodeMapPrivate::NamedNodeMapPrivate(WebCore::NamedNodeMap* webcore)
    : m_webcore(webcore)
    , m_wkc(*this)
    , m_attr(0)
{
}

NamedNodeMapPrivate::~NamedNodeMapPrivate()
{
    delete m_attr;
}

Attribute*
NamedNodeMapPrivate::getAttributeItem(const QualifiedName* name)
{
    WebCore::Attribute *attr = m_webcore->getAttributeItem(*(name->priv()->webcore()));
    if (!attr) {
        return 0;
    }
    if (!m_attr || m_attr->webcore() != attr) {
        delete m_attr;
        m_attr = new AttributePrivate(attr);
    }
    return &m_attr->wkc();
}

NamedNodeMap::NamedNodeMap(NamedNodeMapPrivate& parent)
    : m_private(parent)
{
}

NamedNodeMap::~NamedNodeMap()
{
}

Attribute*
NamedNodeMap::getAttributeItem(const QualifiedName& name) const
{
    return m_private.getAttributeItem(&name);
}

} // namespace

#endif // WKC_ADD_WKC_HELPERS
