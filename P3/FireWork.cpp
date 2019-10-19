#include "FireWork.h"

FireWork::FireWork(PxShape* s, Vector4 color, float m) : Particle(s, color, m) {
	generations_ = (rand() % 3) + 2;
}