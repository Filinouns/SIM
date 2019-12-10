#include "FireworkManager.h"
#include <algorithm>

FireworkManager::FireworkManager() {
	rules = std::vector<FireWorkRule>(4);
	initFireWorkRules();
}

void FireworkManager::initFireWorkRules() {
	rules[0].setParameters(azul, 2, 4, Vector3(-100, 200, -100), Vector3(100, 250, 100), -15, -10, 0.1);
	rules[0].payloads.push_back(Payload(verde, randomInt(2,8)));
	rules[0].payloads.push_back(Payload(rojo, randomInt(2, 8)));
	rules[0].payloads.push_back(Payload(morado, randomInt(2, 8)));

	rules[1].setParameters(verde, 1, 2, Vector3(-150, 250, -150), Vector3(150, 250, 150), -15, -10, 0.1);
	rules[1].payloads.push_back(Payload(rojo, randomInt(2, 4)));

	rules[2].setParameters(rojo, 1, 2, Vector3(-150, 250, -150), Vector3(150, 250, 150), -15, -10, 0.1);
	rules[2].payloads.push_back(Payload(morado, randomInt(1, 2)));

	rules[3].setParameters(morado, 1, 2, Vector3(-150, 250, -150), Vector3(150, 250, 150), -15, -10, 0.1);
}

FireworkManager::FireWorkRule* FireworkManager::GetRuleFromType(Bala type) {
	int i = 0;
	while (i <= rules.size()) {
		if (rules[i].type == type) {
			return &rules[i];
		}
		i++;
	}
}

Firework* FireworkManager::AllocNewFireWork() {
	RenderItem* render = new RenderItem();
	Firework* newFireWork = new Firework(render, CreateShape(*new PxSphereGeometry(1.5)));
	fireworks.push_back(newFireWork);
	return newFireWork;
}

void FireworkManager::fireWorksCreate(Bala type, Firework* parent) {
	FireWorkRule* rule = GetRuleFromType(type);
	Firework* newFire = AllocNewFireWork();
	rule->create(newFire, parent);
	//addGenToReg(newFire);
}

void FireworkManager::create(Bala type, unsigned count, Firework* parent) {
	for (int i = 0; i < count; i++) {
		fireWorksCreate(type, parent);
	}
}

void FireworkManager::FireworksUpdate(float t) {
	for (int i = 0; i < fireworks.size(); i++) {
		if (fireworks[i]->getState() == ON) {
			//fireworks[i]->setOn();
			if (fireworks[i]->update(t)) {
				fireworks[i]->setOff();
				//DeregisterRenderItem(fireworks[i]->getRender());
				FireWorkRule* rule = GetRuleFromType(fireworks[i]->getType());
				//fireworks[i]->setOff();
				for (auto itPayload = rule->payloads.begin(); itPayload != rule->payloads.end(); itPayload++) {
					Payload payload = (*itPayload);
					create(payload.type, payload.count, fireworks[i]);
				}
			}
		}
	}
}