#include "player.h"

Player::Player(AnimObj *p_animobj, InputCfg *p_inputCfg, int p_id)
{
	playerID = p_id;

	animDest.x = 256;
	animDest.y = 16;
	leftKeyDest.x = 256;
	leftKeyDest.y = 128;
	rightKeyDest.x = 256;
	rightKeyDest.y = 136;

	e_inputCfg = p_inputCfg;
	e_animobj = p_animobj;

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
			animInfo.assign("Standing facing right playing");
		}
		else
		{
			e_animobj->startAnim(0);
			animInfo.assign("Running Left playing");
		}
		leftKeyInfo.assign("left Key pressed");
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			e_animobj->startAnim(2);
			animInfo.assign("Standing facing left playing");
		}
		else
		{
			e_animobj->startAnim(1);
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
			animInfo.assign("Running Right playing");
		}
		else
		{
			e_animobj->startAnim(2);
			animInfo.assign("Standing facing left playing");
		}
		leftKeyInfo.assign("left Key not pressed");
		break;
	case MOVE_RIGHT:
		if(keyPressed[keyMap.returnScancode(MOVE_LEFT)] == true)
		{
			e_animobj->startAnim(0);
			animInfo.assign("Running Left playing");
		}
		else
		{
			e_animobj->startAnim(3);
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
}