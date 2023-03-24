#include "RenderScene.hxx"

#define VectorFind(vector, item)         (std::find((vector).begin(), (vector).end(), (item)))
#define VectorFindBinary(vector, item)   ((VectorFind((vector), (item)) != (vector).end()))

namespace GL {

    RenderGroupID RenderScene::getDistinctRenderGroupID(void) const noexcept {
        RenderGroupID renderGroupIdIterator{0};

        // Iterate the indexes until we hit non-used number
        while(!VectorFindBinary(m_vRenderGroupIDtoInternalID, renderGroupIdIterator))
            ++renderGroupIdIterator;

        return renderGroupIdIterator;
    }

}