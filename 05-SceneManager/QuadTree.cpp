#include "QuadTree.h"
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
    m_objects_list.clear();

    //delete m_objects_list;
}
bool Quadtree::IsContain(LPGAMEOBJECT objSrc)
{
    float el, et, er, eb;
    objSrc->GetBoundingBox(el, et, er, eb);

    return !(er < ql ||
        eb > qt ||
        el > ql+ qWidth ||
        et < qt-qHeight);
}
void Quadtree::Split()
{
    m_nodes = new Quadtree * [4];

    m_nodes[0] = new Quadtree(m_level + 1,
        ql, qt, qWidth / 2, qHeight / 2);
    m_nodes[1] = new Quadtree(m_level + 1,
        ql + qWidth / 2, qt, qWidth/2 ,qHeight / 2);
    m_nodes[2] = new Quadtree(m_level + 1,
        ql, qt - qHeight / 2, qWidth / 2, qHeight / 2);
    m_nodes[3] = new Quadtree(m_level + 1,
        ql + qWidth / 2,
            qt - qHeight / 2, qWidth / 2, qHeight / 2);
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

    // Insert entity into current quadtree
    if (this->IsContain(entity))
        m_objects_list.push_back(entity);

    // Split and move all objects in list into it’s corresponding nodes
    if (m_objects_list.size() > MAX_OBJECT_IN_REGION && m_level < MAX_LEVEL)
    {
        Split();

        while (!m_objects_list.empty())
        {
            if (m_nodes[0]->IsContain(m_objects_list.back()))
                m_nodes[0]->Insert(m_objects_list.back());
            if (m_nodes[1]->IsContain(m_objects_list.back()))
                m_nodes[1]->Insert(m_objects_list.back());
            if (m_nodes[2]->IsContain(m_objects_list.back()))
                m_nodes[2]->Insert(m_objects_list.back());
            if (m_nodes[3]->IsContain(m_objects_list.back()))
                m_nodes[3]->Insert(m_objects_list.back());

            m_objects_list.pop_back();
        }
    }
}
void Quadtree::Retrieve(vector<LPGAMEOBJECT> return_objects_list, LPGAMEOBJECT entity)
{
    if (m_nodes)
    {
        if (m_nodes[0]->IsContain(entity))
            m_nodes[0]->Retrieve(return_objects_list, entity);
        if (m_nodes[1]->IsContain(entity))
            m_nodes[1]->Retrieve(return_objects_list, entity);
        if (m_nodes[2]->IsContain(entity))
            m_nodes[2]->Retrieve(return_objects_list, entity);
        if (m_nodes[3]->IsContain(entity))
            m_nodes[3]->Retrieve(return_objects_list, entity);

        return; // Return here to ignore rest.
    }

    // Add all entities in current region into return_objects_list
    if (this->IsContain(entity))
    {
        for (auto i = m_objects_list.begin(); i != m_objects_list.end(); i++)
        {
            if (entity != *i)
                return_objects_list.push_back(*i);
        }
    }
}
void DetectCollision(vector<LPGAMEOBJECT> entity_list)
{
    Quadtree* quadtree = CreateQuadTree();

    vector<LPGAMEOBJECT> return_objects_list;

    for (size_t i = 1; i < entity_list.size(); i++)
    {
        //Get all objects that can collide with current entity
        quadtree->Retrieve(return_objects_list, entity_list[i]);

        for (size_t i = 1; i < return_objects_list.size(); i++)
        {
        // Your algorithm about Collision Detection
        // Do something here
        }

        return_objects_list.clear();
    }

    //quadtree->Release();

    //delete return_objects_list;
    delete quadtree;
}
Quadtree* CreateQuadTree()
{
    // Init base game region for detecting collision
    Quadtree* quadtree = new Quadtree(1, 0, 0, 800, 600);
    // add objects to quadtree
    vector<LPGAMEOBJECT> entity_list;
    for (auto i = entity_list.begin(); i != entity_list.end(); i++)
        quadtree->Insert(*i);

    return quadtree;
}