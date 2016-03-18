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

#include "helpers/WKCAtomicString.h"
#include "helpers/privates/WKCAtomicStringPrivate.h"

#include "helpers/WKCString.h"

#include "AtomicString.h"

namespace WKC {

AtomicStringPrivate::AtomicStringPrivate()
    : m_webcoreowner(1)
{
    m_wkc = 0;
    m_webcore = new WebCore::AtomicString();
}

AtomicStringPrivate::AtomicStringPrivate(WebCore::AtomicString* value)
    : m_webcore(value)
    , m_webcoreowner(0)
{
    m_wkc = new AtomicString(this);
}

AtomicStringPrivate::~AtomicStringPrivate()
{
    delete m_wkc;
    if (m_webcoreowner)
        delete m_webcore;
}

String&
AtomicStringPrivate::string()
{
    m_wkc_string = m_webcore->string();
    return m_wkc_string;
}

AtomicString::AtomicString()
    : m_privateowner(1)
{
    m_private = new AtomicStringPrivate();
}

AtomicString::AtomicString(AtomicStringPrivate* priv)
    : m_privateowner(0)
{
    m_private = priv;
}

AtomicString::~AtomicString()
{
    if (m_privateowner)
        delete m_private;
}

const String&
AtomicString::string() const
{
    return m_private->string();
}

} // namespace

#endif // WKC_ADD_WKC_HELPERS
