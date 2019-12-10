#pragma once
#include "Firework.h"
#include <vector>
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "Explosion.h";
#include <ctime>


using namespace physx;

//template <typename myType>
class FireworkManager {

private:

	ParticleForceRegistry<Particula>* reg;

	std::vector<ParticleForceGenerator*> forces;

	static int randomInt(int min, int max) { //range : [min, max)
		static bool first = true;
		if (first) {
			srand(time(NULL)); //seeding for the first time only!
			first = false;
		}
		return min + rand() % ((max + 1) - min);
	}

	static float randomfl(float min, float max)	{
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	};

	struct Payload {
		Bala type;
		unsigned count;

		Payload(Bala _type, unsigned _count) {
			type = _type;
			count = _count;
		}
	};

	struct FireWorkRule	{
		FireWorkRule() {};
		Bala type;

		float minAge_, maxAge_;
		Vector3 minVel_, maxVel_;
		int minGrav_ = 0;
		int maxGrav_ = 0;
		PxShape* mySh;

		float damping_;

		std::vector<Payload> payloads;

		void setParameters(Bala ty, float minAge, float maxAge, Vector3& minVel, Vector3& maxVel, signed int minGrav, signed int maxGrav, float damp, PxShape* shape = CreateShape(*new PxSphereGeometry(1.5))) {
			mySh = shape;
			type = ty;
			minAge_ = minAge;
			maxAge_ = maxAge;
			minVel_ = minVel;
			maxVel_ = maxVel;
			minGrav_ = minGrav;
			maxGrav_ = maxGrav;
			damping_ = damp;
		}

		void create(Firework* firework, const Firework* parent = NULL) const {
			firework->setType(type);
			firework->setAge(randomInt(minAge_, maxAge_));

			Vector3 vel(0, 0, 0);

			if (parent) {
				firework->setPos(parent->getPos());
				vel += parent->getVel();
			}
			else {
				Vector3 start(0, 0, 0);
				int x = rand() % 3 - 1;
				start.x = 5.0f * x;
				firework->setPos(start);
			}
			firework->setVel({ vel.x += (randomInt(minVel_.x, maxVel_.x) - 50),
				vel.y += (randomInt(minVel_.y, maxVel_.y)),
				vel.z += (randomInt(minVel_.z, maxVel_.z) - 50) });
			firework->setMass(1);
			firework->setDamp(damping_);
			firework->setAcc({ 0, randomfl(minGrav_, maxGrav_), 0 });
		}
	};
	std::vector<FireWorkRule> rules;
	std::vector<Firework*> fireworks;

	void initFireWorkRules();

	FireWorkRule* GetRuleFromType(Bala type);
	Firework* AllocNewFireWork();

	void create(Bala type, unsigned count, Firework* parent);
public:
	FireworkManager();
	~FireworkManager() {
		for (auto f : fireworks) {
			delete f;
			f = nullptr;
		}
	};
	void fireWorksCreate(Bala type, Firework* parent = NULL);
	void FireworksUpdate(float t);

	void addForceToGens(ParticleForceGenerator* newForce) {	forces.push_back(newForce); }

	void addGenToReg(Particula* p) {
		for (int i = 0; i < forces.size(); i++)	reg->add(p, forces[i]);
	}

	ParticleForceRegistry<Particula>* getReg(ParticleForceRegistry<Particula>* reg_) { return reg = reg_; };

	RenderItem* fire;
};