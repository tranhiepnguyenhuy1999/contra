#include "QuadTree.h"
#include "Camera.h"

Quadtree::Quadtree(float l, float b, int width, int height) {
    m_nodes = NULL;
    
    qWidth = width;
    qHeight = height;
    
    ql = l;
    qb = b;
    qr = ql + qWidth;
    qt = qb + qHeight;
};
Quadtree::Quadtree() {
    m_nodes = NULL;

    qWidth = 0;
    qHeight = 0;

    ql = 0;
    qb = 0;
    qr = ql + 0;
    qt = qb + 0;
};

void Quadtree::Clear()
{
    // Clear all nodes
    if (m_nodes)
    {
        for (int i = 0; i < 4; i++)
        {
            m_nodes[i]->Clear();
            delete m_nodes[i];
        }
        delete[] m_nodes;
    }
    // Clear current Quadtree
     DebugOut(L">>> node list %d >>> \n", m_objects_list.size());

    //for (UINT i = 0; i < m_objects_list.size(); i++) delete m_objects_list[i];
}

bool Quadtree::IsContain(LPGAMEOBJECT objSrc)
{
    float el, et, er, eb;
    objSrc->GetBoundingBox(el, et, er, eb);

    return !(er < ql ||
        eb > qt ||
        el > qr ||
        et < qb);
}
void Quadtree::Split()
{
    m_nodes = new Quadtree * [4];

    m_nodes[0] = new Quadtree(
        ql, qb, qWidth / 2, qHeight / 2);
    m_nodes[1] = new Quadtree(
        ql + qWidth / 2, qb, qWidth/2 ,qHeight / 2);
    m_nodes[2] = new Quadtree(
        ql, qb + qHeight / 2, qWidth / 2, qHeight / 2);
    m_nodes[3] = new Quadtree(
        ql + qWidth / 2, qb + qHeight / 2, qWidth / 2, qHeight / 2);
}
void Quadtree::Insert(LPGAMEOBJECT entity)
{

    // Insert entity into corresponding nodes
    if (m_nodes)
    {
        if (m_nodes[0]->IsContain(entity))
            m_nodes[0]->Insert(entity);
        if (m_nodes[1]->IsContain(entity))
            m_nodes[1]->Insert(entity);
        if (m_nodes[2]->IsContain(entity))
            m_nodes[2]->Insert(entity);
        if (m_nodes[3]->IsContain(entity))
            m_nodes[3]->Insert(entity);

        return; // Return here to ignore rest.
    }

    Camera* cam = Camera::GetInstance();
    float cWidth, cHeight;
    cam->getCamWidth(cWidth);
    cam->getCamHeight(cHeight);

    // Insert entity into current quadtree
    if (this->IsContain(entity))
        m_objects_list.push_back(entity);
    //// Split and move all objects in list into it’s corresponding nodes
    //if (m_objects_list.size() !=0 && qWidth > cWidth/2 && qHeight > cHeight/2)
    //{
    //    Split();

    //    // start moving all object form father node
    //    while (!m_objects_list.empty())
    //    {
    //        if (m_nodes[0]->IsContain(m_objects_list.back()))
    //            m_nodes[0]->Insert(m_objects_list.back());
    //        if (m_nodes[1]->IsContain(m_objects_list.back()))
    //            m_nodes[1]->Insert(m_objects_list.back());
    //        if (m_nodes[2]->IsContain(m_objects_list.back()))
    //            m_nodes[2]->Insert(m_objects_list.back());
    //        if (m_nodes[3]->IsContain(m_objects_list.back()))
    //            m_nodes[3]->Insert(m_objects_list.back());

    //        m_objects_list.pop_back();
    //    }
    //}
}
void Quadtree::Retrieve(vector<LPGAMEOBJECT> &coObjects, LPGAMEOBJECT entity)
{
    Camera* cam = Camera::GetInstance();

    if (!cam->isCamContain(ql, qt, qr, qb)) return;
    
    if (m_nodes)
    {
        if (m_nodes[0]->IsContain(entity))
            m_nodes[0]->Retrieve(coObjects, entity);
        if (m_nodes[1]->IsContain(entity))
            m_nodes[1]->Retrieve(coObjects, entity);
        if (m_nodes[2]->IsContain(entity))
            m_nodes[2]->Retrieve(coObjects, entity);
        if (m_nodes[3]->IsContain(entity))
            m_nodes[3]->Retrieve(coObjects, entity);

        return; // Return here to ignore rest.
    }

    // Add all entities in current region into return_objects_list
    if (this->IsContain(entity))
    {
        for (size_t i = 0; i < m_objects_list.size(); i++)
        {
            DebugOut(L">>> here >>> \n");
            if (entity != m_objects_list[i])
            {
                bool isContained = false;
                for (size_t j = 0; j < coObjects.size(); j++)
                {
                    if (m_objects_list[i] == coObjects[j]) isContained = true;
                }

                if(!isContained) coObjects.push_back(m_objects_list[i]);

            }
        }
    }
}