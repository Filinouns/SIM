#include "FireWork.h"

FireWork::FireWork(PxShape* s, Vector4 color, float m) : Particle(s, color, m){
	explote_ = 2.0f;
	//generateFirework(t);
}


FireWork::~FireWork() {}