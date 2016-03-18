/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * All rights reserved.
 * Copyright (c) 2010 ACCESS CO., LTD. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PlatformWheelEvent.h"
#include "Scrollbar.h"

#include "NotImplemented.h"

#include "WKCPlatformEvents.h"

namespace WebCore {

// Keep this in sync with the other platform event constructors
PlatformWheelEvent::PlatformWheelEvent(void* event)
{
    WKC::WKCWheelEvent* ev = (WKC::WKCWheelEvent *)event;
    m_deltaX = ev->m_dx;
    m_deltaY = ev->m_dy;

    m_wheelTicksX = m_deltaX;
    m_wheelTicksY = m_deltaY;

    m_position = IntPoint(ev->m_x, ev->m_y);
    m_globalPosition = IntPoint(ev->m_x, ev->m_y);
    m_granularity = ScrollByPixelWheelEvent;
    m_isAccepted = false;
    if (ev->m_modifiers & WKC::EModifierShift) {
        m_shiftKey = true;
    } else {
        m_shiftKey = false;
    }
    if (ev->m_modifiers & WKC::EModifierCtrl) {
        m_ctrlKey = true;
    } else {
        m_ctrlKey = false;
    }
    if (ev->m_modifiers & WKC::EModifierAlt) {
        m_altKey = true;
    } else {
        m_altKey = false;
    }
    if (ev->m_modifiers & WKC::EModifierMod1) {
        m_metaKey = true;
    } else {
        m_metaKey = false;
    }
}

}
