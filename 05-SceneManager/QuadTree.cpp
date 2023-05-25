#include "QuadTree.h"
#include "Camera.h"

Quadtree::Quadtree(float l, float b, int width, int height) {
    qWidth = width;
    qHeight = height;
    ql = l;
    qb = b;
    qr = ql + qWidth;
    qt = qb + qHeight;
    m_nodes = NULL;
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
    for (UINT i = 0; i < m_objects_list.size(); i++) delete m_objects_list[i];
    m_objects_list.clear();

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
        ql, qt, qWidth / 2, qHeight / 2);
    m_nodes[1] = new Quadtree(
        ql + qWidth / 2, qt, qWidth/2 ,qHeight / 2);
    m_nodes[2] = new Quadtree(
        ql, qt - qHeight / 2, qWidth / 2, qHeight / 2);
    m_nodes[3] = new Quadtree(
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

    Camera* cam = Camera::GetInstance();
    float cWidth, cHeight;
    cam->getCamWidth(cWidth);
    cam->getCamHeight(cHeight);

    // Insert entity into current quadtree
    if (this->IsContain(entity))
        m_objects_list.push_back(entity);
    // Split and move all objects in list into it’s corresponding nodes
    if (m_objects_list.size() !=0 && qWidth > cWidth/2 && qHeight > cHeight/2)
    {
        Split();

        // start moving all object form father node
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

Quadtree* CreateQuadTree()
{
    // Init base game region for detecting collision
    Quadtree* quadtree = new Quadtree(0, 0, 800, 600);
    // add objects to quadtree
    vector<LPGAMEOBJECT> entity_list;
    for (auto i = entity_list.begin(); i != entity_list.end(); i++)
        quadtree->Insert(*i);

    return quadtree;
}

void DetectCollision(vector<LPGAMEOBJECT> objList, vector<LPGAMEOBJECT> coObjects)
{
    Quadtree* quadtree = CreateQuadTree();

    for (size_t i = 1; i < objList.size(); i++)
    {
        //Get all objects that can collide with current entity
        quadtree->Retrieve(coObjects, objList[i]);

        for (size_t i = 1; i < coObjects.size(); i++)
        {
        // Your algorithm about Collision Detection
        // Do something here
        }

        coObjects.clear();
    }

    //quadtree->Release();

    //delete return_objects_list;
    delete quadtree;
}