#include "player.h"

Player::Player()
{
}

Player::Player(int p_id)
{
	playerID = p_id;
}

Player::Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;
	playerState.alive = true;
	playerState.moving = false;
	playerState.leftpressed = false;
	playerState.rightpressed = false;
	playerState.downpressed = false;
	playerState.uppressed = false;

	pstate.pos.set(PLAYER_INITIAL_X, PLAYER_INITIAL_Y, 0);
	pstate.vel.zero();
	pstate.acc.zero();

	pstate.mass = 0.1;

	moveForce.zero();

	e_inputCfg = p_inputCfg;
	e_animobj = p_animobj;

	e_animobj->startAnim(1);
	
	keyMap.ClearMap();

	//e_inputCfg->assignInput(SDL_SCANCODE_LEFT, MOVE_LEFT);
	//e_inputCfg->assignInput(SDL_SCANCODE_RIGHT, MOVE_RIGHT);
	//e_inputCfg->assignInput(SDL_SCANCODE_UP, MOVE_UP);
	//e_inputCfg->assignInput(SDL_SCANCODE_DOWN, MOVE_DOWN);
}

void Player::relocate(int x, int y)
{
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
	e_inputCfg->assignInput(lastKey);
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

	if(moveForce.length() > 0)	//Convert to unit vector then scale by desired magnitude
	{
		moveForce.normalize();
		if(!playerState.downpressed && !playerState.uppressed && !playerState.leftpressed && !playerState.rightpressed)
		{
			moveForce.zero();	//If no movement keys are pressed, stop applying force
		}
		else
		{
			moveForce *= DEF_FORCE;
		}
	}

	pstate.vel *= DEF_FRIC;	//Not really a force here, but apply friction to slow to stop
}

void Player::verlet(double dt)
{
	previous = pstate;

	cml::vector3d lastAcc = pstate.acc;

	pstate.pos += pstate.vel * dt + (0.5f * lastAcc * dt * dt);

	cml::vector3d newAcc = moveForce / pstate.mass;

	cml::vector3d avgAcc = (lastAcc + newAcc) / 2;

	pstate.acc = newAcc;

	pstate.vel += avgAcc * dt;
}

void Player::Interpolate(const double alpha)
{
	double rendx = pstate.pos[0]*alpha + previous.pos[0]*(1.0 - alpha);
	double rendy = pstate.pos[1]*alpha + previous.pos[1]*(1.0 - alpha);

	e_animobj->updateLoc(rendx, rendy);
	e_animobj->playAnim();
}

void Player::SelectAnim()
{
	if(pstate.vel.length() > 1)
	{
		e_animobj->startAnim(0);
		if(pstate.vel.length() < 30)
			e_animobj->changeSpeed(2);
		else if(pstate.vel.length() < 100)
			e_animobj->changeSpeed(1);
		else
			e_animobj->changeSpeed(0.5);
	}
	else
		e_animobj->startAnim(1);
}

int Player::getPid()
{
	return playerID;
}

void Player::reportVel(std::string &velstr)
{
	velstr.assign("Velocity: ");
	velstr.append(std::to_string(pstate.vel.length()));
}