#include "player.h"

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

	e_inputCfg->assignPlayerMap(&keyMap);

	e_inputCfg->assignInput(SDL_SCANCODE_LEFT, MOVE_LEFT);
	e_inputCfg->assignInput(SDL_SCANCODE_RIGHT, MOVE_RIGHT);
	e_inputCfg->assignInput(SDL_SCANCODE_UP, MOVE_UP);
	e_inputCfg->assignInput(SDL_SCANCODE_DOWN, MOVE_DOWN);
}

void Player::configInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive)
{
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
		//if(keyPressed[keyMap.returnScancode(MOVE_RIGHT)] == true)
		//{
		//	moveForce[0] = ZERO_FORCE;
		//}
		//else
		//{
		//	moveForce[0] = -DEF_FORCE;
		//}
		playerState.leftpressed = true;
		break;
	case MOVE_RIGHT:
		//if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		//{
		//	moveForce[0] = ZERO_FORCE;
		//}
		//else
		//{
		//	moveForce[0] = DEF_FORCE;
		//}
		playerState.rightpressed = true;
		break;
	case MOVE_UP:
		//if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		//{
		//	moveForce[1] = ZERO_FORCE;
		//}
		//else
		//{
		//	moveForce[1] = -DEF_FORCE;
		//}
		playerState.uppressed = true;
		break;
	case MOVE_DOWN:
		//if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		//{
		//	moveForce[1] = ZERO_FORCE;
		//}
		//else
		//{
		//	moveForce[1] = DEF_FORCE;
		//}
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
		//if(keyPressed[keyMap.returnScancode(MOVE_RIGHT)] == true)
		//{
		//	moveForce[0] = DEF_FORCE;
		//}
		//else
		//{
		//	moveForce[0] = ZERO_FORCE;
		//}
		playerState.leftpressed = false;
		break;
	case MOVE_RIGHT:
		//if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		//{
		//	moveForce[0] = -DEF_FORCE;
		//}
		//else
		//{
		//	moveForce[0] = ZERO_FORCE;
		//}
		playerState.rightpressed = false;
		break;
	case MOVE_UP:
		//if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		//{
		//	moveForce[1] = DEF_FORCE;
		//}
		//else
		//{
		//	moveForce[1] = ZERO_FORCE;
		//}
		playerState.uppressed = false;
		break;
	case MOVE_DOWN:
		//if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		//{
		//	moveForce[1] = -DEF_FORCE;
		//}
		//else
		//{
		//	moveForce[1] = ZERO_FORCE;
		//}
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

//void Player::Integrate(double t, double dt)
//{
//	prevState = playerPosVel;
//
//	Derivative a = this->eval(playerPosVel, t);
//	Derivative b = this->eval(playerPosVel, t, dt*0.5f, a);
//	Derivative c = this->eval(playerPosVel, t, dt*0.5f, b);
//	Derivative d = this->eval(playerPosVel, t, dt, c);
//
//	const double dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
//	const double dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);
//
//	const double dxvdt = 1.0f/6.0f * (a.dxv + 2.0f*(b.dxv + c.dxv) + d.dxv);
//	const double dyvdt = 1.0f/6.0f * (a.dyv + 2.0f*(b.dyv + c.dyv) + d.dyv);
//
//	playerPosVel.x = playerPosVel.x + dxdt * dt;
//	playerPosVel.y = playerPosVel.y + dydt * dt;
//
//	playerPosVel.xv = playerPosVel.xv + dxvdt * dt;
//	playerPosVel.yv = playerPosVel.yv + dyvdt * dt;
//}

void Player::Interpolate(const double alpha)
{
	//double rendx = pstate.x*alpha + previous.x * (1.0 - alpha);
	double rendx = pstate.pos[0]*alpha + previous.pos[0]*(1.0 - alpha);
	double rendy = pstate.pos[1]*alpha + previous.pos[1]*(1.0 - alpha);

	e_animobj->updateLoc(rendx, rendy);
	e_animobj->playAnim();
}

//void Player::accel(const State &state, double t, Derivative &p_output)
//{
//	p_output.dxv = 0;
//	p_output.dyv = 0;
//}
//
//Derivative Player::eval(const State &initial, double t, double dt, const Derivative &d)
//{
//	State state;
//
//	state.x = initial.x + d.dx*dt;
//	state.y = initial.y + d.dy*dt;
//
//	state.xv = initial.xv + d.dxv*dt;
//	state.yv = initial.yv + d.dyv*dt;
//
//	Derivative output;
//
//	output.dx = state.xv;
//	output.dy = state.yv;
//
//	this->accel(state, t+dt, output);	//dxv, dxy assigned here
//
//	return output;
//}
//
//Derivative Player::eval(const State &initial, double t)
//{
//	Derivative output;
//
//	output.dx = initial.xv;
//	output.dy = initial.yv;
//
//	this->accel(initial, t, output);
//	
//	return output;
//}

void Player::Collide()
{
}

void Player::applyTimers()
{
	//if(xtimerStarted)
	//{
	//	xdt = (double)(SDL_GetPerformanceCounter() - xtimer)/SDL_GetPerformanceFrequency();
	//	if(xdt >= FORCEFALLOFFTIME)
	//	{
	//		xtimerStarted = false;
	//		xf = ZERO_FORCE;
	//	}
	//}
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

void Player::reportVel(std::string &velstr)
{
	velstr.assign("Velocity: ");
	velstr.append(std::to_string(pstate.vel.length()));
}