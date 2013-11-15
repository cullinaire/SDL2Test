#include "player.h"

Player::Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;
	playerState.alive = true;
	playerState.moving = false;

	playerPosVel.x = PLAYER_INITIAL_X;
	playerPosVel.y = PLAYER_INITIAL_Y;
	playerPosVel.xv = 0;
	playerPosVel.yv = 0;

	renderState = playerPosVel;	//only renderstate talks to the renderer

	//debug msg settings
	pposDest.x = 16;
	pposDest.y = 464;
	derivXDest.x = 16;
	derivXDest.y = 384;
	derivYDest.x = 16;
	derivYDest.y = 400;

	e_inputCfg = p_inputCfg;
	e_animobj = p_animobj;
	e_animobj->updateLoc(renderState.x, renderState.y);

	e_animobj->startAnim(0);
	
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
			playerPosVel.xv = STOPPED_SPD;
		}
		else
		{
			playerPosVel.xv = -DEF_SPD;
		}
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			playerPosVel.xv = STOPPED_SPD;
		}
		else
		{
			playerPosVel.xv = DEF_SPD;
		}
		break;
	case MOVE_UP:
		if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		{
			playerPosVel.yv = STOPPED_SPD;
		}
		else
		{
			playerPosVel.yv = -DEF_SPD;
		}
		break;
	case MOVE_DOWN:
		if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		{
			playerPosVel.yv = STOPPED_SPD;
		}
		else
		{
			playerPosVel.yv = DEF_SPD;
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
			playerPosVel.xv = DEF_SPD;
		}
		else
		{
			playerPosVel.xv = STOPPED_SPD;
		}
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			playerPosVel.xv = -DEF_SPD;
		}
		else
		{
			playerPosVel.xv = STOPPED_SPD;
		}
		break;
	case MOVE_UP:
		if(keyPressed[keyMap.returnScancode(MOVE_DOWN)] == true)
		{
			playerPosVel.yv = DEF_SPD;
		}
		else
		{
			playerPosVel.yv = STOPPED_SPD;
		}
		break;
	case MOVE_DOWN:
		if(keyPressed[keyMap.returnScancode(MOVE_UP)] == true)
		{
			playerPosVel.yv = -DEF_SPD;
		}
		else
		{
			playerPosVel.yv = STOPPED_SPD;
		}
		break;
	case JUMP:
		break;
	default:
		break;
	}
}

void Player::emitInfo(TxtLayer *txtOut)
{
	txtOut->ReceiveString(pposInfo, pposDest);
	txtOut->ReceiveString(derivXInfo, derivXDest);
	txtOut->ReceiveString(derivYInfo, derivYDest);
}

void Player::Integrate(double t, double dt)
{
	prevState = playerPosVel;

	Derivative a = this->eval(playerPosVel, t);
	Derivative b = this->eval(playerPosVel, t, dt*0.5f, a);
	Derivative c = this->eval(playerPosVel, t, dt*0.5f, b);
	Derivative d = this->eval(playerPosVel, t, dt, c);

	const double dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const double dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);

	const double dxvdt = 1.0f/6.0f * (a.dxv + 2.0f*(b.dxv + c.dxv) + d.dxv);
	const double dyvdt = 1.0f/6.0f * (a.dyv + 2.0f*(b.dyv + c.dyv) + d.dyv);

	playerPosVel.x = playerPosVel.x + dxdt * dt;
	playerPosVel.y = playerPosVel.y + dydt * dt;

	playerPosVel.xv = playerPosVel.xv + dxvdt * dt;
	playerPosVel.yv = playerPosVel.yv + dyvdt * dt;

	pposInfo.assign("a.dxv: ");
	pposInfo.append(std::to_string(a.dxv));
	pposInfo.append(" a.dyv: ");
	pposInfo.append(std::to_string(a.dyv));
	pposInfo.append(" x: ");
	pposInfo.append(std::to_string(playerPosVel.x));
	pposInfo.append(" y: ");
	pposInfo.append(std::to_string(playerPosVel.y));
	derivXInfo.assign("a.dx: ");
	derivXInfo.append(std::to_string(a.dx));
	derivYInfo.assign("a.dy: ");
	derivYInfo.append(std::to_string(a.dy));
}

void Player::Interpolate(const double alpha)
{
	renderState.x = playerPosVel.x*alpha + prevState.x * (1.0 - alpha);
	renderState.y = playerPosVel.y*alpha + prevState.y * (1.0 - alpha);

	//Renderstate has no need for velocity information - it's only drawing a snapshot
	//renderState.xv = playerPosVel.xv*alpha + prevState.xv * (1.0 - alpha);
	//renderState.yv = playerPosVel.yv*alpha + prevState.yv * (1.0 - alpha);

	e_animobj->updateLoc(renderState.x, renderState.y);
	e_animobj->playAnim();
}

void Player::accel(const State &state, double t, Derivative &p_output)
{
	//Derivative acc;
	//const double k = 10;
	//const double b = 1;
	//acc.dxv = -k * state.x - b*state.xv;
	//acc.dyv = -k * state.y - b*state.yv;
	//return acc;
	p_output.dxv = 0;
	p_output.dyv = 0;
}

Derivative Player::eval(const State &initial, double t, double dt, const Derivative &d)
{
	State state;

	state.x = initial.x + d.dx*dt;
	state.y = initial.y + d.dy*dt;

	state.xv = initial.xv + d.dxv*dt;
	state.yv = initial.yv + d.dyv*dt;

	Derivative output;

	output.dx = state.xv;
	output.dy = state.yv;

	this->accel(state, t+dt, output);	//dxv, dxy assigned here

	return output;
}

Derivative Player::eval(const State &initial, double t)
{
	Derivative output;

	output.dx = initial.xv;
	output.dy = initial.yv;

	this->accel(initial, t, output);
	
	return output;
}

void Player::Collide()
{
}