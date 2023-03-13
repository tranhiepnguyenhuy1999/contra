#include "QuestionBrick.h"

void CQuestionBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	float aniID = ID_ANI_TOUCHED_QUESTION_BRICK;
	if (this->state == QUESTIONBRICK_STATE_UNTOUCHED) aniID = ID_ANI_UNTOUCHED_QUESTION_BRICK;
		animations->Get(aniID)->Render(x, y);

	//RenderBoundingBox();
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state== QUESTIONBRICK_STATE_TOUCHED_1 && (GetTickCount64() - count_start > 150))
	{
		SetState(QUESTIONBRICK_STATE_STATIC);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case  QUESTIONBRICK_STATE_TOUCHED_1:
		count_start = GetTickCount64();
		y = yLimit;
		break;
	case  QUESTIONBRICK_STATE_UNTOUCHED:
	case  QUESTIONBRICK_STATE_STATIC:
		y = yLimit + 2;
		break;
	}
}