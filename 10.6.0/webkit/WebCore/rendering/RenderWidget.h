/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2009 Apple Inc. All rights reserved.
 * Copyright (c) 2010 ACCESS CO., LTD. All rights reserved.
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
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef RenderWidget_h
#define RenderWidget_h

#include "OverlapTestRequestClient.h"
#include "RenderReplaced.h"

namespace WebCore {

class Widget;

class RenderWidget : public RenderReplaced, private OverlapTestRequestClient {
public:
    virtual ~RenderWidget();

    Widget* widget() const { return m_widget.get(); }
    virtual void setWidget(PassRefPtr<Widget>);

    static RenderWidget* find(const Widget*);

    void updateWidgetPosition();

    void showSubstituteImage(PassRefPtr<Image>);

    static void suspendWidgetHierarchyUpdates();
    static void resumeWidgetHierarchyUpdates();

#if 1 // webkit.org trunk r56646
    RenderArena* ref() { ++m_refCount; return renderArena(); }
    void deref(RenderArena*);
#endif

#if PLATFORM(WKC)
    static void deleteSharedInstance();
    static void resetVariables();
#endif

protected:
    RenderWidget(Node*);

    FrameView* frameView() const { return m_frameView; }

    void clearWidget();

    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle);
    virtual void layout();

private:
    virtual bool isWidget() const { return true; }

    virtual void paint(PaintInfo&, int x, int y);
    virtual void destroy();
    virtual void setSelectionState(SelectionState);
    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty, HitTestAction);
    virtual void setOverlapTestResult(bool);

    bool setWidgetGeometry(const IntRect&);

#if 0 // webkit.org trunk r56646
    friend class RenderWidgetProtector;
    RenderArena* ref() { ++m_refCount; return renderArena(); }
    void deref(RenderArena*);
#endif
#if !ENABLE(WKC_FRAME_FLATTENING)
    RefPtr<Widget> m_widget;
#endif
    RefPtr<Image> m_substituteImage;
#if !ENABLE(WKC_FRAME_FLATTENING)
    FrameView* m_frameView;
#endif
    int m_refCount;

#if ENABLE(WKC_FRAME_FLATTENING)
protected:
    RefPtr<Widget> m_widget;
    FrameView* m_frameView;
#endif
};

inline RenderWidget* toRenderWidget(RenderObject* object)
{
    ASSERT(!object || object->isWidget());
    return static_cast<RenderWidget*>(object);
}

inline const RenderWidget* toRenderWidget(const RenderObject* object)
{
    ASSERT(!object || object->isWidget());
    return static_cast<const RenderWidget*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderWidget(const RenderWidget*);

} // namespace WebCore

#endif // RenderWidget_h
