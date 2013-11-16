#include "player.h"

Player::Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;
	playerState.alive = true;
	playerState.moving = false;

	pstate.x = PLAYER_INITIAL_X;
	pstate.y = PLAYER_INITIAL_Y;
	pstate.xv = pstate.yv = pstate.xa = pstate.ya = 0;
	pstate.mass = 0.1;
	xf = yf = 0;

	//renderState = playerPosVel;	//only renderstate talks to the renderer

	e_inputCfg = p_inputCfg;
	e_animobj = p_animobj;
	//e_animobj->updateLoc(renderState.x, renderState.y);

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
		//Thinking about implementing a Spelunky style approach to simultaneous
		//keydowns for left and right move inputs. That is, if one is pressed
		//while the other was already being held down, then the response is for
		//the player to stop in his tracks, without turning around.
		//If a key is released, the player begins moving in the direction of the
		//key that is still held down.
	case MOVE_LEFT:
		if(keyPressed[keyMap.returnScancode(MOVE_RIGHT)] == true)
		{
			xf = ZERO_FORCE;
		}
		else
		{
			xf = -DEF_FORCE;
		}
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			xf = ZERO_FORCE;
		}
		else
		{
			xf = DEF_FORCE;
		}
		break;
	case MOVE_UP:
		if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		{
			yf = ZERO_FORCE;
		}
		else
		{
			yf = -DEF_FORCE;
		}
		break;
	case MOVE_DOWN:
		if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		{
			yf = ZERO_FORCE;
		}
		else
		{
			yf = DEF_FORCE;
		}
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
		if(keyPressed[keyMap.returnScancode(MOVE_RIGHT)] == true)
		{
			xf = DEF_FORCE;
		}
		else
		{
			xf = ZERO_FORCE;
		}
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			xf = -DEF_FORCE;
		}
		else
		{
			xf = ZERO_FORCE;
		}
		break;
	case MOVE_UP:
		if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		{
			yf = DEF_FORCE;
		}
		else
		{
			yf = ZERO_FORCE;
		}
		break;
	case MOVE_DOWN:
		if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		{
			yf = -DEF_FORCE;
		}
		else
		{
			yf = ZERO_FORCE;
		}
		break;
	case JUMP:
		break;
	default:
		break;
	}
}

void Player::verlet(double dt)
{
	previous = pstate;

	double lastxa = pstate.xa;

	pstate.x += pstate.xv * dt + (0.5f * lastxa * dt * dt);

	double newxa = xf / pstate.mass;
	
	double avgxa = (lastxa + newxa) / 2;

	pstate.xa = newxa;

	pstate.xv += avgxa * dt;
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
	double rendx = pstate.x*alpha + previous.x * (1.0 - alpha);
	double rendy = 32;	//for now

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

void Player::SelectAnim()
{
	//if()
	//{
	//	playerState.moving = false;
	//}
	//else
	//{
	//	playerState.moving = true;
	//}

	//if(playerState.moving == true)
	//{
	//	e_animobj->startAnim(0);	//walking anim
	//}
	//else
	//{
	//	e_animobj->startAnim(1);	//standing anim
	//}
}