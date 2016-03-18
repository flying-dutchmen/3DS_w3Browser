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

#include "helpers/WKCQualifiedName.h"
#include "helpers/privates/WKCQualifiedNamePrivate.h"

#include "helpers/WKCString.h"
#include "helpers/WKCAtomicString.h"
#include "helpers/privates/WKCAtomicStringPrivate.h"

#include "QualifiedName.h"

namespace WebCore {
class AtomicString;
} // namespece

namespace WKC {

QualifiedNamePrivate::QualifiedNamePrivate(const AtomicString& p, const String& l, const AtomicString& n)
{
    m_webcore_local_name = l;
    m_webcore_prefix = p.priv()->webcore();
    m_webcore_namespace = n.priv()->webcore();
    m_webcore = new WebCore::QualifiedName(*m_webcore_prefix, m_webcore_local_name, *m_webcore_namespace);
}

QualifiedNamePrivate::~QualifiedNamePrivate()
{
    delete m_webcore;
}

QualifiedNamePrivate*
QualifiedName::priv() const
{
    return m_private;
}

QualifiedName::QualifiedName(const AtomicString& p, const String& l, const AtomicString& n)
{
    m_private = new QualifiedNamePrivate(p, l, n);
}

QualifiedName::~QualifiedName()
{
    delete m_private;
}

} // namespece

#endif // WKC_ADD_WKC_HELPERS
