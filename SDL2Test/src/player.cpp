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

	playerBox.objID = playerID;	//IDs for all other types of collidables will begin with numbers > MAX_PLAYERS
	playerBox.type = PLAYER;
	
	pstate.pos.set(PLAYER_INITIAL_X, PLAYER_INITIAL_Y, 0);
	pstate.vel.zero();
	pstate.acc.zero();

	this->updateAABB(PLAYER_INITIAL_X, PLAYER_INITIAL_Y);

	pstate.mass = 0.1;

	moveForce.zero();

	e_inputCfg = p_inputCfg;

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
//X increases to the right and Y increases to the bottom.
//Make sure to update these whenever the player changes position!!!
void Player::updateAABB(int x, int y)
{
	playerBox.vals[0][0] = x+2;		//minX
	playerBox.vals[0][1] = y+24-3;	//minY
	playerBox.vals[1][0] = x+11-2;	//maxX
	playerBox.vals[1][1] = y+2;		//maxY
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
	this->updateAABB(pstate.pos[0], pstate.pos[1]);

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
	playerAnim.updateLoc(rendx, rendy);
	playerAnim.outputCurFrame();
}

void Player::SelectAnim()
{
	if(pstate.vel.length() > 1)
	{
		playerAnim.playAnim(0);
		if(pstate.vel.length() < 30)
			playerAnim.changeSpeed(2);
		else if(pstate.vel.length() < 100)
			playerAnim.changeSpeed(1);
		else
			playerAnim.changeSpeed(0.5);
	}
	else
		playerAnim.playAnim(1);
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

void Player::drawAABB(SDL_Renderer *rend)
{
	SDL_Point corners[5];
	corners[0].x = playerBox.vals[0][0];
	corners[0].y = playerBox.vals[0][1];
	corners[1].x = playerBox.vals[0][0];
	corners[1].y = playerBox.vals[1][1];
	corners[2].x = playerBox.vals[1][0];
	corners[2].y = playerBox.vals[1][1];
	corners[3].x = playerBox.vals[1][0];
	corners[3].y = playerBox.vals[0][1];
	corners[4].x = playerBox.vals[0][0];
	corners[4].y = playerBox.vals[0][1];

	SDL_RenderDrawLines(rend, corners, 5);
}