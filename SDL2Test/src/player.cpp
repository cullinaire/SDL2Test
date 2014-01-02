#include "player.h"

Player::Player()
{
}

//This constructor only used to create "dummy players"
Player::Player(int p_id)
{
	playerID = p_id;
}

Player::Player(SpriteSheet *playerSheet, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;
	playerState.alive = true;
	playerState.moving = false;
	playerState.leftpressed = false;
	playerState.rightpressed = false;
	playerState.downpressed = false;
	playerState.uppressed = false;

	//playerBox.objID will get set when it is added later
	playerBox.type = PLAYER;
	
	pstate.pos.set(PLAYER_INITIAL_X, PLAYER_INITIAL_Y, 0);
	pstate.vel.zero();
	pstate.acc.zero();

	this->updateAABB(PLAYER_INITIAL_X, PLAYER_INITIAL_Y);

	pstate.mass = 0.1;

	elapsed = -1;
	impDuration = 0;
	impulseActive = false;

	impulse.zero();
	moveForce.zero();

	e_inputCfg = p_inputCfg;
	e_inputCfg->assignPlayerMap(&keyMap);

	playerAnim.loadSheet(playerSheet);
	playerAnim.defineAnim("../assets/hatmanani.def");
	playerAnim.startAnim();
	
	keyMap.ClearMap();
}

//Set up collision AABB: The box will be slightly smaller than the actual player sprite, excluding
//The hat and the feet. The width will also be slightly less than the sprite width.
//Right now the actual player width and height (full sprite size) is 11 and 24 (subject to change,
//check the spritesheet definition files!)
//Remember that the x,y coord of the player is the upper left corner of the sprite.
//X increases to the right and Y increases to the bottom. (THIS DISTINCTION IS CRITICAL)
//Make sure to update these whenever the player changes position!!!
void Player::updateAABB(int x, int y)
{
	playerBox.vals[0][0] = x+2;		//minX
	playerBox.vals[0][1] = y+2;		//minY
	playerBox.vals[1][0] = x+11-2;	//maxX
	playerBox.vals[1][1] = y+24-3;	//maxY
}

void Player::relocate(int x, int y)
{
	this->updateAABB(x, y);
	pstate.pos.set(x, y, 0);
}

void Player::configInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive)
{
	std::string newTitle;
	newTitle.assign("Player ");
	newTitle.append(std::to_string(playerID));
	newTitle.append(" input config");

	e_inputCfg->assignPlayerMap(&keyMap);
	e_inputCfg->changeMenuTitle(newTitle);
	e_inputCfg->processInput(lastKey, waitingForInput, menuactive);
}

void Player::assignInput(SDL_Scancode lastKey)
{
	e_inputCfg->assignPlayerMap(&keyMap);
	e_inputCfg->assignInput(lastKey);
}

void Player::assignInput(const std::string keybindFilename)
{
	e_inputCfg->assignPlayerMap(&keyMap);
	e_inputCfg->assignInput(keybindFilename);
}

std::string Player::getInputName(SDL_Scancode lastKey)
{
	return keyMap.returnInputName(keyMap.returnInput(lastKey));
}

void Player::processKeyDown(SDL_Scancode p_scancode, bool *keyPressed)
{
	switch(keyMap.returnInput(p_scancode))
	{
	case MOVE_LEFT:
		playerState.leftpressed = true;
		break;
	case MOVE_RIGHT:
		playerState.rightpressed = true;
		break;
	case MOVE_UP:
		playerState.uppressed = true;
		break;
	case MOVE_DOWN:
		playerState.downpressed = true;
		break;
	case JUMP:
		break;
	default:
		break;
	}
}

void Player::processKeyUp(SDL_Scancode p_scancode, bool *keyPressed)
{
	switch(keyMap.returnInput(p_scancode))
	{
	case MOVE_LEFT:
		playerState.leftpressed = false;
		break;
	case MOVE_RIGHT:
		playerState.rightpressed = false;
		break;
	case MOVE_UP:
		playerState.uppressed = false;
		break;
	case MOVE_DOWN:
		playerState.downpressed = false;
		break;
	case JUMP:
		break;
	default:
		break;
	}
}

void Player::modifyForces(double t)
{
	if(playerState.rightpressed)
	{
		moveForce[0] = 1;
		if(playerState.leftpressed)	//implies left is already held down when right is pressed
		{
			moveForce[0] = 0;	//Then the response is to zero the horizontal force
		}
	}
	else if(playerState.leftpressed)	//The else is critical!
	{
		moveForce[0] = -1;
		if(playerState.rightpressed)
		{
			moveForce[0] = 0;
		}
	}
	else
		moveForce[0] = 0;	//Don't forget the null condition!

	if(playerState.downpressed)
	{
		moveForce[1] = 1;
		if(playerState.uppressed)
		{
			moveForce[1] = 0;
		}
	}
	else if(playerState.uppressed)
	{
		moveForce[1] = -1;
		if(playerState.downpressed)
		{
			moveForce[1] = 0;
		}
	}
	else
		moveForce[1] = 0;	//likewise...

	//Process move force
	if(moveForce[0] != 0 || moveForce[1] != 0)	//do not normalize vector if it is zero!
	{
		moveForce.normalize();
		moveForce *= DEF_FORCE;
	}

	//Apply impulses if any
	if(impulseActive)
	{
		if(elapsed < 0)	//Countdown not started yet
		{
			elapsed = t;
		}
		if(t - elapsed > impDuration)
		{
			elapsed = -1;
			impulseActive = false;
		}
	}

	finalForce = moveForce;

	if(impulseActive)
		finalForce = moveForce + impulse;
}

void Player::verlet(double dt)
{
	this->updateAABB(pstate.pos[0], pstate.pos[1]);

	previous = pstate;

	cml::vector3d lastAcc = pstate.acc;

	pstate.pos += pstate.vel * dt + (0.5f * lastAcc * dt * dt);

	cml::vector3d newAcc = finalForce / pstate.mass;

	cml::vector3d avgAcc = (lastAcc + newAcc) / 2;

	pstate.acc = newAcc;

	pstate.vel += avgAcc * dt;

	pstate.vel *= DEF_FRIC;	//Not really a force here, but apply friction to slow to stop
}

void Player::Interpolate(const double alpha)
{
	double rendx = pstate.pos[0]*alpha + previous.pos[0]*(1.0 - alpha);
	double rendy = pstate.pos[1]*alpha + previous.pos[1]*(1.0 - alpha);
	playerAnim.updateLoc(rendx, rendy);
	playerAnim.outputCurFrame();
}

void Player::SelectAnim(double t)
{
	if(pstate.vel.length() > 1)
	{
		playerAnim.playAnim(0, t);
		if(pstate.vel.length() < 30)
			playerAnim.changeSpeed(2);
		else if(pstate.vel.length() < 100)
			playerAnim.changeSpeed(1);
		else
			playerAnim.changeSpeed(0.5);
	}
	else
		playerAnim.playAnim(1, t);
}

int Player::getPid()
{
	return playerID;
}

void Player::setBoxId(int newId)
{
	playerBox.boxId = newId;
}

int Player::getBoxId()
{
	return playerBox.boxId;
}

void Player::reportForce(std::string &forceStr)
{
	forceStr.assign("FinalForce X: ");
	forceStr.append(std::to_string(finalForce[0]));
	forceStr.append(", Y: ");
	forceStr.append(std::to_string(finalForce[1]));
}

void Player::applyImpulse(double duration, cml::vector3d direction, double magnitude)
{
	impulse = direction;
	impulse.normalize();
	impulse *= magnitude;
	impulseActive = true;
	impDuration = duration;
}

AABB Player::outputAABB()
{
	return playerBox;
}

PlayerGroup::PlayerGroup()
{
	numActive = 0;
	emptyPlayer = Player(EMPTY_PLAYER);
	for(int i=0;i < MAXPLAYERS;++i)
	{
		players[i] = &emptyPlayer;
	}
	playerSheet = SpriteSheet("../assets/men.bmp", rend, "../assets/hatman.def");
}

PlayerGroup::~PlayerGroup()
{
	for(int i=0;i<MAXPLAYERS;++i)
	{
		if(this->PlayerExists(i))
		{
			delete players[i];
		}
	}
}

bool PlayerGroup::Add(int id, SweepAndPrune &collider, InputCfg &playerCfg)
{
	if(!this->PlayerExists(id))
	{
		Player *newPlayer = new Player(&playerSheet, &playerCfg, id);
		newPlayer->relocate(rand() % 600, rand() % 400);
		players[id] = newPlayer;
		players[id]->setBoxId(collider.Add(newPlayer->outputAABB()));
		++numActive;
		std::cout << "Numactive: " << numActive << std::endl;
		return true;
	}
	return false;
}

void PlayerGroup::Remove(int id, SweepAndPrune &collider)
{
	if(this->PlayerExists(id))
	{
		collider.Remove(players[id]->getBoxId());
		delete players[id];
		players[id] = &emptyPlayer;
		--numActive;
		std::cout << "Numactive: " << numActive << std::endl;
	}
}

void PlayerGroup::Render(double alpha, double t)
{
	for(int i=0;i < MAXPLAYERS;++i)
	{
		if(this->PlayerExists(i))
		{
			players[i]->Interpolate(alpha);
			players[i]->SelectAnim(t);
		}
	}
}

void PlayerGroup::Update(double t, double dt, SweepAndPrune &collider, std::string &forceStr)
{
	//Do collision update first since it may affect the forces on the bodies
	for(int i=0;i < MAXPLAYERS;++i)
	{
		if(this->PlayerExists(i))
		{
			collider.Update(players[i]->outputAABB());
		}
	}

	for(int i=0;i < MAXPLAYERS;++i)
	{
		//collider.ResolveEncounters(players[i]);
	}

	for(int i=0;i < MAXPLAYERS;++i)
	{
		if(this->PlayerExists(i))
		{
			players[i]->modifyForces(t+dt);
			players[i]->verlet(dt);

			if(i == 1)
				players[i]->reportForce(forceStr);
		}
	}	
}

void PlayerGroup::AssignInput(int id, const std::string defPath)
{
	if(players[id]->getPid() != emptyPlayer.getPid())
	{
		players[id]->assignInput(defPath);
	}
}

bool PlayerGroup::PlayerExists(int id)
{
	if(players[id]->getPid() != emptyPlayer.getPid())
		return true;
	else
		return false;
}

bool PlayerGroup::is_Full()
{
	if(numActive == MAXPLAYERS)
		return true;
	else
		return false;
}

bool PlayerGroup::is_Empty()
{
	if(numActive == 0)
		return true;
	else
		return false;
}

void PlayerGroup::ProcessInput(bool DownElseUp, SDL_Scancode scancode, bool *keyPressed)
{
	for(int i=0;i<MAXPLAYERS;++i)
	{
		if(this->PlayerExists(i))
		{
			if(DownElseUp)	//Keydown event
			{
				players[i]->processKeyDown(scancode, keyPressed);
			}
			else
			{
				players[i]->processKeyUp(scancode, keyPressed);
			}
		}
	}
}

void PlayerGroup::RandomImpulse()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> duration(1.0, 2.0);
	std::uniform_real_distribution<double> direction(-50.0, 50.0);
	std::uniform_real_distribution<double> magnitude(10.0, 30.0);

	for(int i=0;i<MAXPLAYERS;++i)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		generator.seed(seed);
		cml::vector3d randDir(direction(generator), direction(generator), direction(generator));
		double randMag = magnitude(generator);
		double randDuration = duration(generator);

		std::cout << "RandMag: " << randMag << " RandDur: " << randDuration << std::endl;

		if(this->PlayerExists(i))
		{
			players[i]->applyImpulse(randDuration, randDir, randMag);
		}
	}
}