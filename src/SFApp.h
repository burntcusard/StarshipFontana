#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"

/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {
public:
  SFApp();
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  int     OnExecute();
  void    OnUpdateWorld();
  void    OnRender();
	
	void		NewCoin();
	void		UpdateTail();
	
  void    FireProjectile();
private:
  SDL_Surface           * surface;
  bool                    is_running;
  bool										paused;

  shared_ptr<SFAsset>        player;
  shared_ptr<SFAsset>        deadPlayer;
  shared_ptr<SFBoundingBox>  app_box;
  list<shared_ptr<SFAsset> > playerTails;
  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > aliens;
  list<shared_ptr<SFAsset> > coins;

  int fire;
  
  int points = 0; // Player starts with 0 points!
  int gameSpeed = 15; // 60 = 1 tick a second
	int gameTick = 0;
	int movementAmount = 32; // Got to put this in GoNorth like Vector2(0.0f, movementAmount)

  SFError OnInit();
};
#endif
