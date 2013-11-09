#include "player.h"

Player::Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;
	state = ALIVE;
	playerPosVel.x = 256;
	playerPosVel.y = 256;
	playerPosVel.xv = 0;
	playerPosVel.yv = 0;

	renderState = playerPosVel;

	animDest.x = 256;
	animDest.y = 16;
	leftKeyDest.x = 256;
	leftKeyDest.y = 128;
	rightKeyDest.x = 256;
	rightKeyDest.y = 136;
	pposDest.x = 400;
	pposDest.y = 464;
	derivDest.x = 16;
	derivDest.y = 384;

	e_inputCfg = p_inputCfg;
	e_animobj = p_animobj;
	e_animobj->updateLoc(renderState.x, renderState.y);

	e_animobj->startAnim(2);
	animInfo.assign("Standing facing left playing");
	
	keyMap.ClearMap();
}

void Player::configInput(SDL_Scancode lastKey, bool *waitingForInput, bool *menuactive)
{
	e_inputCfg->assignPlayerMap(&keyMap);
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
			e_animobj->startAnim(3);
			playerPosVel.xv = STOPPED_SPD;
			animInfo.assign("Standing facing right playing");
		}
		else
		{
			e_animobj->startAnim(0);
			playerPosVel.xv = LEFT_RUN_SPD;
			animInfo.assign("Running Left playing");
		}
		leftKeyInfo.assign("left Key pressed");
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			e_animobj->startAnim(2);
			playerPosVel.xv = STOPPED_SPD;
			animInfo.assign("Standing facing left playing");
		}
		else
		{
			e_animobj->startAnim(1);
			playerPosVel.xv = RIGHT_RUN_SPD;
			animInfo.assign("Running Right playing");
		}
		rightKeyInfo.assign("right Key pressed");
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
			e_animobj->startAnim(1);
			playerPosVel.xv = RIGHT_RUN_SPD;
			animInfo.assign("Running Right playing");
		}
		else
		{
			e_animobj->startAnim(2);
			playerPosVel.xv = STOPPED_SPD;
			animInfo.assign("Standing facing left playing");
		}
		leftKeyInfo.assign("left Key not pressed");
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			e_animobj->startAnim(0);
			playerPosVel.xv = LEFT_RUN_SPD;
			animInfo.assign("Running Left playing");
		}
		else
		{
			e_animobj->startAnim(3);
			playerPosVel.xv = STOPPED_SPD;
			animInfo.assign("Standing facing right playing");
		}
		rightKeyInfo.assign("right Key not pressed");
		break;
	default:
		break;
	}
}

void Player::emitInfo(TxtLayer *txtOut)
{
	txtOut->ReceiveString(animInfo, animDest);
	txtOut->ReceiveString(leftKeyInfo, leftKeyDest);
	txtOut->ReceiveString(rightKeyInfo, rightKeyDest);
	txtOut->ReceiveString(pposInfo, pposDest);
	txtOut->ReceiveString(derivInfo, derivDest);
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

	pposInfo.assign("xv: ");
	pposInfo.append(std::to_string(playerPosVel.xv));
	derivInfo.assign("a.dx: ");
	derivInfo.append(std::to_string(a.dx));
}

void Player::Interpolate(const double alpha)
{
	renderState.x = playerPosVel.x*alpha + prevState.x * (1.0 - alpha);
	renderState.y = playerPosVel.y*alpha + prevState.y * (1.0 - alpha);

	renderState.xv = playerPosVel.xv*alpha + prevState.xv * (1.0 - alpha);
	renderState.yv = playerPosVel.yv*alpha + prevState.yv * (1.0 - alpha);

	e_animobj->updateLoc(renderState.x, renderState.y);
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