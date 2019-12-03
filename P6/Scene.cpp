#include "Scene.h"

using namespace physx;
using namespace std;

//------------------------------------------------------------------

//Plano donde reposaran los objetos de la pantalla
Particle*				plano = NULL;

//Muelles
Particle*			   	myParticle = NULL; //Particula
Particle*				Cube_ = NULL;

//Fuerzas
ParticleGravity*		inverse_grav = NULL;
ParticleGravity*		grav1 = NULL;	//Gravedad1
ParticleGravity*		grav2 = NULL;	//Gravedad2
Wind*					wind = NULL;	//Viento
Wind*					wind1 = NULL;	//Viento temp para muelles
Explosion*				explosion = NULL; //Explosion
ParticleAnchoredSpring*	muelleFijo = NULL;	//Punto de anclaje del muelle

// Constraints
ParticleContactManager* contactManager = NULL;
ParticleContact*		contact = NULL;
ParticleCable*			cable = NULL;
ParticleRod*			barra = NULL;
//Particulas de constraints para poder manejarlas desde el teclado
bool fall1, fall2;
Particle* p1;
Particle* p3;

// RigidBodys
RBodySystem* rb = NULL;
ParticleForceRegistry<RigidObject>* pFRegRigids = NULL;
PxRigidStatic* suelo = nullptr;

// Registro de fuerzas para los sistemas
ParticleForceRegistry<Particle>*	reg = NULL;

vector<vector<ParticleForceGenerator*>> forces_;	// Vector de fuerzas para cada sistema
list<ParticleSystem*> s_particles_;					// Lista de sistemas de particulas
list<Particle*> particles_;							// Lista de particulas sueltas


Scene::Scene(PxScene* s, PxPhysics* ph) : scene_(s), physx_(ph) {
	Init();
}

Scene::~Scene() {
	// ------Limpieza de los elementos de la escena------ -
		// Limpieza de los Sistemas de particulas
		if (s_particles_.size() > 0) {
			for (auto p : s_particles_) {
				delete p;
				p = nullptr;
			}
		}

	// Limpieza de las Fuerzas
	if (forces_.size() > 0) {
		for (auto i : forces_) {
			for (auto f : i) {
				delete f;
				f = nullptr;
			}
		}
	}

	// Limpieza de particulas sueltas
	if (particles_.size() > 0) {
		for (auto p : particles_) {
			delete p;
			p = nullptr;
		}
	}

	// Limpieza de elementos sueltos
	if (wind1 != NULL) {
		delete wind1;
		wind1 = nullptr;
	}
	
	// Muelles------
	delete muelleFijo;
	muelleFijo = nullptr;

	// Constraints-----
	delete cable;
	cable = nullptr;

	delete barra;
	barra = nullptr;

	delete contact;
	contact = nullptr;

	// --------
	reg->clear();
	delete reg;
	reg = nullptr;
}

void Scene::Init() {

	Vector3 pos;

	// Registro de fuerzas
	reg = new ParticleForceRegistry<Particle>();

	// Plano donde reposaran los demas elementos de la escena
	plano = new Particle(CreateShape(*new PxBoxGeometry(100, 1, 1000)), { 0, 0, 0, 1 }, 0);
	plano->setPos({ 0, 0, 0 });

	// Diferentes simulaciones
	//generateParticleSystems();
	//generateSprings();
	//generateConstraints();
	generateRigidBodys();

	//--------------CAMARA------------------
	//myCamera_ = GetCamera();

}

void Scene::update(double t) {

	//--------Update P4---------
	// En caso de limites en la barra o el cable
	if (contactManager != NULL) {
		if (cable->addContact(contact)) contactManager->addContact(contact);
		if (barra->addContact(contact)) contactManager->addContact(contact);

		contactManager->update(t);
	}
	//--------------------------

	// Update Fuerzas
	reg->UpdateForces(t);

	// Update Sistemas
	if (s_particles_.size() != 0) {
		for (auto s : s_particles_) s->update(t);
	}

	// Update particulas
	if (particles_.size() != 0) {
		for (auto p : particles_) p->update(t);
	}

	// Limpieza del viento1 para que no interactue con la particula.
	if (wind1 != NULL && wind1->getRender()) {
		wind1->unrender();
		reg->remove(myParticle, wind1);
		delete wind1;
		wind1 = NULL;
	}
}

void Scene::keyPress(unsigned char key, const PxTransform &camera) {
	switch (toupper(key)) {
	//Activar la explosion
	case 'X': {
		explosion->activate(true);
		break;
	}
	// Activar/desactivar la fuerza afectando al sistema de muelles
	case char(32) : {
		if (wind1 == nullptr) wind1 = new Wind({ 300, 0, -100 }, Cube_->getPos(), 50);
		else wind1->unrender();

		reg->add(myParticle, wind1);
		break;
	}
					//-----p4-----
	// Damos fuerza a p1(cable) de gravedad y quitamos la inversa
	case 'I': {
		reg->add(p1, grav1);
		reg->remove(p1, inverse_grav);
		break;
	}
	// Damos fuerza a p1(cable) hacia arribay quitamos la gravedad
	case 'O': {
		reg->add(p1, inverse_grav);
		reg->remove(p1, grav1);
		break;
	}
	// Detenemos las particulas del cable
	case 'P': {
		p1->setVel(Vector3(0.0, 0.0, 0.0));
		cable->particle[1]->setVel({ 0.0, 0.0, 0.0 });
		reg->remove(p1, inverse_grav);
		reg->remove(p1, grav1);
		break;
	}
	// Damos fuerza a p3(barra) de gravedad y quitamos la inversa
	case 'J': {
		reg->add(p3, grav1);
		reg->remove(p3, inverse_grav);
		break;
	}
	// Damos fuerza a p3(barra) hacia arriba y quitamos la gravedad.
	case 'K': {
		reg->add(p3, inverse_grav);
		reg->remove(p3, grav1);
		break;
	}
	// Detenemos las particulas de la barra
	case 'L': {
		reg->remove(p3, grav1);
		reg->remove(p3, inverse_grav);
		p3->setVel({ 0.0, 0.0, 0.0 });
		barra->particle[1]->setVel({ 0.0, 0.0, 0.0 });
		break;
	}
			  //-------------------------------------
	// Aumentar la k
	case '+':
		muelleFijo->addK();
		break;
	// Disminuir la k
	case '-':
		muelleFijo->subK();
		break;
	default:
		break;
	}
}

void Scene::generateParticleSystems() {
	Vector3 pos;

	//-----------------Objects----------------
	// Sistema Fuente
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, 0 };
	ParticleSystem* FountainSystem = new ParticleSystem(pos);
	s_particles_.push_back(FountainSystem);

	// Sistema Fireworks
	pos = GetCamera()->getDir() * 100 + Vector3{ -100, 400, 0 };
	FireworkSystem* fireWorkSystem = new FireworkSystem(pos);
	s_particles_.push_back(fireWorkSystem);

	//---------------------------------------Fuerzas---------------------------------
	//--------Creacion de las fuerzas--------
	//------Sistema1 -> Viento -------
	grav1 = new ParticleGravity({ 0, -10, 0 });		// Gravedad
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 200, 0 };
	wind = new Wind({ 500, 0,0 }, pos, 50);			// Viento
	//------Sistema2 -> Explosion ------
	grav2 = new ParticleGravity({ 0, -35, 0 });		// Gravedad2
	pos = GetCamera()->getDir() * 100 + Vector3{ -100, 150, 0 };
	explosion = new Explosion(100000, pos, 100);	// Explosion

	// Vector de Generadores de Fuerzas, cada "x" son las fuerzas que se aplican a 1 unico sistema
	forces_.resize(s_particles_.size());
	//Fuerzas del sistema fuente
	forces_[0].push_back(grav1);
	forces_[0].push_back(wind);
	//Fuerzas del sistema Fireworks
	forces_[1].push_back(grav2);
	forces_[1].push_back(explosion);
	//Fuerzas muelles
	//forces_[2].push_back(grav1);
	//forces_[2].push_back(muelleFijo);

	// Dar fuerzas a los sistemas
	int i = 0;
	for (auto s : s_particles_) {
		s->setReg(reg);
		for (auto f : forces_[i]) s->registerForce(f);
		i++;
	}
}

void Scene::generateSprings() {

	Vector3 pos;
	Vector4 color = { 1, 0, 1, 1 };

	//----------------------------Muelles------------------------------

	//-----------P1
	// Particula ancladas a un punto mediante un muelle

	//Punto anclaje
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, 200 };
	Cube_ = new Particle(CreateShape(*new PxBoxGeometry(5, 5, 5)), color, 1);
	Cube_->init(pos, { 0, 0, 0 }, 0.99);
	Cube_->setState(INF);
	particles_.push_back(Cube_);
	//Particula anclada
	myParticle = new Particle(CreateShape(*new PxSphereGeometry(3)), { 0, 0, 1, 1 }, 2.0f);
	myParticle->init(
		Cube_->getPos() - Vector3{ 0, 20, 0 },
		{ 0, 0, 0 },
		1);
	myParticle->setState(INF);
	particles_.push_back(myParticle);

	//-----------P2
	// --Particulas unidas entre si por un  muelle--

	pos = GetCamera()->getDir() * 100 + Vector3{ -200, 100, 200 };
	//Particula1
	Particle* PartP2_1 = new Particle(CreateShape(*new PxSphereGeometry(5)), { 1, 0.92, 0.016, 1 }, 2.0f);
	PartP2_1->init(pos, { 3, 3, 0 }, 0.99);
	PartP2_1->setState(INF);
	particles_.push_back(PartP2_1);
	//Particula2
	Particle* PartP2_2 = new Particle(CreateShape(*new PxSphereGeometry(5)), { 1, 0.92, 0.016, 1 }, 2.0f);
	PartP2_2->init(pos + Vector3{ 0, 50, 0 }, { 0, 0, 0 }, 0.99);
	PartP2_2->setState(INF);
	particles_.push_back(PartP2_2);

	//-----------P3
	// --Particula simulando el movimiento en el agua mediante un muelle

	//Particula
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, 300 };
	Particle* PartP3_1 = new Particle(CreateShape(*new PxBoxGeometry(5, 5, 5)), color, 20);
	PartP3_1->init(pos, { 0, 0, 0 }, 0.99);
	PartP3_1->setState(INF);
	particles_.push_back(PartP3_1);
	//Plano
	Particle* Agua = new Particle(CreateShape(*new PxBoxGeometry(30, 1, 30)), { 0, 0, 1, 1 }, 1);
	Agua->init(pos - Vector3{ 0, 50, 0 }, { 0, 0, 0 }, 1);
	Agua->setState(INF);
	particles_.push_back(Agua);


	//-Fuerzas-
	//P1
	muelleFijo = new ParticleAnchoredSpring(Cube_->getPosPointer(), 1, 20);	// Muelle anclado
	//P2
	ParticleSpring* muelle = new ParticleSpring(PartP2_1, 1, 30);
	ParticleSpring* muelle1 = new ParticleSpring(PartP2_2, 1, 30);
	//P3
	ParticleBuoyancy* water = new ParticleBuoyancy(5, 1.1f, Agua->getPos().y, 1000.0f);

	// Fuerzas de los muelles
	// P1 muelles
	//muelleFijo->setAnchor(&Cube_->getPos());
	reg->add(myParticle, muelleFijo);
	reg->add(myParticle, grav1);
	// P2 muelles
	reg->add(PartP2_2, muelle);
	reg->add(PartP2_1, muelle1);
	// P3 muelles
	reg->add(PartP3_1, water);
	reg->add(PartP3_1, grav2);

}

void Scene::generateConstraints() {
	contact = new ParticleContact();
	contactManager = new ParticleContactManager();
	
	// Simulaciones representadas 
	cable = new ParticleCable();
	barra = new ParticleRod();

	// Particulas necesarias
	Vector3 pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, -100 };
	
	//Cable
	p1 = new Particle(CreateShape(*new PxBoxGeometry(3, 3, 3)), { 1, 0, 0, 1 }, 1);
	p1->init(pos, { 0, 0, 0 }, 0.99);
	p1->setState(INF);
	particles_.push_back(p1);

	Particle *p2 = new Particle(CreateShape(*new PxBoxGeometry(3, 3, 3)), { 1, 0, 1, 1 }, 1);
	p2->init(pos + Vector3{-5, 0, 0}, { 0, 0, 0 }, 0.99);
	p2->setState(INF);
	particles_.push_back(p2);

	//Barra
	pos = GetCamera()->getDir() * 100 + Vector3{ 150, 100, -100 };
	p3 = new Particle(CreateShape(*new PxBoxGeometry(2, 2, 2)), { 1, 1, 0, 1 }, 1);
	p3->init(pos, { 0, 0, 0 }, 0.99);
	p3->setState(INF);
	particles_.push_back(p3);

	Particle *p4 = new Particle(CreateShape(*new PxBoxGeometry(2, 2, 2)), { 0, 0, 1, 1 }, 1);
	p4->init(pos + Vector3{ -10, 0, 0 }, { 0, 0, 0 }, 0.99);
	p4->setState(INF);
	particles_.push_back(p4);

	// Fuerza inversa a la gravedad
	inverse_grav = new ParticleGravity({ 0, 10, 0 });

	// Juntamos las particulas con las simulaciones
	//Cable
	cable->particle[0] = p1;
	cable->particle[1] = p2;
	cable->maxLength = 10;
	cable->restitution = 1;

	//Barra
	barra->particle[0] = p3;
	barra->particle[1] = p4;
	barra->length = 10;
}

void Scene::generateRigidBodys() {
	pFRegRigids = new ParticleForceRegistry<RigidObject>();

	rb = new RBodySystem(scene_, physx_);
	rb->getReg(pFRegRigids);

	//explosion = new Explosion(100, 10, { 0, 0, 0 }, 40.0f);
	wind = new Wind({ -10, 0, 0 }, { 0, 0, 0 }, 30.0f);

	/*
	fManager = new FireworkManager();
	fManager->getReg(pFRegParticulas);
	*/
	rb->addForceToGens(explosion);
	rb->addForceToGens(wind);
}
