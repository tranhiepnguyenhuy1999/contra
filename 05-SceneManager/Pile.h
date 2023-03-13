
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_PILE_HEAD_FIRST 8
#define ID_ANI_PILE_HEAD_LAST 9
#define ID_ANI_PILE_BODY_FIRST 11
#define ID_ANI_PILE_BODY_LAST 12
#define PILE_WIDTH 32
#define PILE_BBOX_WIDTH 16
#define PILE_BBOX_HEIGHT 16

class CPile : public CGameObject {
	float height;
public:
	CPile(float x, float y, float height) : CGameObject(x, y) {
		this->height = height;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};