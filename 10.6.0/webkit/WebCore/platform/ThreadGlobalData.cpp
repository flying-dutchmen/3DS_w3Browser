/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 *
 */

#include "config.h"
#include "ThreadGlobalData.h"

#include "EventNames.h"
#include "StringImpl.h"
#include "ThreadTimers.h"
#include <wtf/UnusedParam.h>

#if USE(ICU_UNICODE)
#include "TextCodecICU.h"
#endif

#if PLATFORM(MAC)
#include "TextCodecMac.h"
#endif

#if ENABLE(WORKERS)
#include <wtf/Threading.h>
#include <wtf/ThreadSpecific.h>
using namespace WTF;
#endif

namespace WebCore {

#if PLATFORM(WKC)
#if ENABLE(WORKERS)
static ThreadSpecific<ThreadGlobalData>* gThreadGlobalData = 0;
#else
static ThreadGlobalData* gStaticData = 0;
#endif

void ThreadGlobalData_deleteSharedInstance()
{
#if ENABLE(WORKERS)
    delete gThreadGlobalData;
#else
    gStaticData->~ThreadGlobalData();
    fastFree(gStaticData);
#endif
}

void ThreadGlobalData::resetVariables()
{
#if ENABLE(WORKERS)
    gThreadGlobalData = 0;
#else
    gStaticData = 0;
#endif
}
ThreadGlobalData& threadGlobalData()
{
    // FIXME: Workers are not necessarily the only feature that make per-thread global data necessary.
    // We need to check for e.g. database objects manipulating strings on secondary threads.
#if ENABLE(WORKERS)
    // ThreadGlobalData is used on main thread before it could possibly be used on secondary ones, so there is no need for synchronization here.
    if (!gThreadGlobalData) {
        gThreadGlobalData = new ThreadSpecific<ThreadGlobalData>;
    }
    return **gThreadGlobalData;
#else
    if (!gStaticData) {
        gStaticData = static_cast<ThreadGlobalData*>(fastMalloc(sizeof(ThreadGlobalData)));
        // ThreadGlobalData constructor indirectly uses staticData, so we need to set up the memory before invoking it.
        new (gStaticData) ThreadGlobalData;
    }
    return *gStaticData;
#endif
}
#else
ThreadGlobalData& threadGlobalData()
{
    // FIXME: Workers are not necessarily the only feature that make per-thread global data necessary.
    // We need to check for e.g. database objects manipulating strings on secondary threads.
#if ENABLE(WORKERS)
    // ThreadGlobalData is used on main thread before it could possibly be used on secondary ones, so there is no need for synchronization here.
    static ThreadSpecific<ThreadGlobalData>* threadGlobalData = new ThreadSpecific<ThreadGlobalData>;
    return **threadGlobalData;
#else
    static ThreadGlobalData* staticData;
    if (!staticData) {
        staticData = static_cast<ThreadGlobalData*>(fastMalloc(sizeof(ThreadGlobalData)));
        // ThreadGlobalData constructor indirectly uses staticData, so we need to set up the memory before invoking it.
        new (staticData) ThreadGlobalData;
    }
    return *staticData;
#endif
}
#endif

ThreadGlobalData::ThreadGlobalData()
    : m_emptyString(new StringImpl)
    , m_atomicStringTable(new HashSet<StringImpl*>)
    , m_eventNames(new EventNames)
    , m_threadTimers(new ThreadTimers)
#ifndef NDEBUG
    , m_isMainThread(isMainThread())
#endif
#if USE(ICU_UNICODE)
    , m_cachedConverterICU(new ICUConverterWrapper)
#endif
#if PLATFORM(MAC)
    , m_cachedConverterTEC(new TECConverterWrapper)
#endif
{
}

ThreadGlobalData::~ThreadGlobalData()
{
#if PLATFORM(MAC)
    delete m_cachedConverterTEC;
#endif
#if USE(ICU_UNICODE)
    delete m_cachedConverterICU;
#endif

    delete m_eventNames;

#if PLATFORM(WKC)
    if (m_atomicStringTable) {
        HashSet<StringImpl*>::iterator end = m_atomicStringTable->end();

        for (HashSet<StringImpl*>::iterator it = m_atomicStringTable->begin(); it != end; ++it) {
            StringImpl *impl = (StringImpl*)*it;
            if (impl) {
                if (impl->inTable())
                    impl->clearInTable();
                impl->~StringImpl();
                fastFree(impl);
            }
        }
    }
#endif

    delete m_atomicStringTable;
    delete m_threadTimers;

    // Using member variable m_isMainThread instead of calling WTF::isMainThread() directly
    // to avoid issues described in https://bugs.webkit.org/show_bug.cgi?id=25973.
    // In short, some pthread-based platforms and ports can not use WTF::CurrentThread() and WTF::isMainThread()
    // in destructors of thread-specific data.
    ASSERT(m_isMainThread || m_emptyString->hasOneRef()); // We intentionally don't clean up static data on application quit, so there will be many strings remaining on the main thread.

    delete m_emptyString;
}

} // namespace WebCore
