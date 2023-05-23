#include "GameObject.h"
#include "PlayScene.h"
#include <vector>

using namespace std;

typedef CGameObject* LPGAMEOBJECT;

class Quadtree
{
private:
    int            m_level;
    float ql, qt, qr, qb;
    int qWidth, qHeight;
    vector<LPGAMEOBJECT> m_objects_list;
    Quadtree** m_nodes;

    bool           IsContain(LPGAMEOBJECT obj);
    void           Split();
    //Quadtree* CreateQuadTree();
public:
    Quadtree() {
        m_level = 0;
        ql = 0;
        qt = 0;
        qWidth = 0;
        qHeight = 0;
        m_nodes = NULL;
    };
    Quadtree(int level, float l, float t, int width, int height) {
        m_level = level;
        ql = l;
        qt = t;
        qWidth = width;
        qHeight = height;
        m_nodes = NULL;
    };
    ~Quadtree() {};

    void    Clear();
    void    Insert(LPGAMEOBJECT entity);
    void Retrieve(vector<LPGAMEOBJECT> return_objects_list, LPGAMEOBJECT entity);
};
