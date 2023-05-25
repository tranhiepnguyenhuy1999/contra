#include "GameObject.h"
#include "PlayScene.h"
#include <vector>

using namespace std;

typedef CGameObject* LPGAMEOBJECT;

class Quadtree
{
private:
    float ql, qt, qr, qb;
    int qWidth, qHeight;
    vector<LPGAMEOBJECT> m_objects_list;
    Quadtree** m_nodes;

    bool           IsContain(LPGAMEOBJECT obj);
    void           Split();
    //Quadtree* CreateQuadTree();
public:
    Quadtree( float l, float b, int width, int height);
    ~Quadtree() {};

    void    Clear();
    void    Insert(LPGAMEOBJECT entity);
    void Retrieve(vector<LPGAMEOBJECT> return_objects_list, LPGAMEOBJECT entity);
};
