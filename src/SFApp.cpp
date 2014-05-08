#include "SFApp.h"

SFApp::SFApp() : points(3), is_running(true) {

  surface = SDL_GetVideoSurface();
  app_box = make_shared<SFBoundingBox>(Vector2(surface->w/2, surface->h/2), surface->w/2, surface->h/2);
  player  = make_shared<SFAsset>(SFASSET_PLAYER);
  // Player is 30x30, Point2 -'s are to get them aligned to grid properly
  auto player_pos = Point2((surface->w/2)-16, (surface->h/2)-18);
  player->SetPosition(player_pos);
  
  int numberOfTails = 1; // This is temporary until no. of tails is linked to points
  for(int i=1; i<=numberOfTails; i++) {
  	auto playerTail = make_shared<SFAsset>(SFASSET_TAIL);
  	auto pos 				= Point2((surface->w/2)-16, ((surface->h/2)-18)+(32*i));
  	playerTail->SetPosition(pos);
  	playerTails.push_back(playerTail);
  }
  
//    const int number_of_aliens = 10;
//    for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
//    auto alien = make_shared<SFAsset>(SFASSET_ALIEN);
//    auto pos   = Point2((surface->w/number_of_aliens) * i, 200.0f);
//    alien->SetPosition(pos);
//    aliens.push_back(alien);
//  }

  srand (time(NULL)); // initialize random seed

//  auto coin = make_shared<SFAsset>(SFASSET_COIN);
//  auto pos  = Point2((surface->w/4), 100);
//  coin->SetPosition(pos);
//  coins.push_back(coin);
	NewCoin();
	
}

SFApp::~SFApp() {
}

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_UP:
    player->FaceNorth();
    break;
  case SFEVENT_PLAYER_DOWN:
    player->FaceSouth();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->FaceWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->FaceEast();
    break;
/*  case SFEVENT_FIRE:
    fire ++;
    std::stringstream sstm;
    sstm << "Fire " << fire;
    SDL_WM_SetCaption(sstm.str().c_str(),  sstm.str().c_str());
    FireProjectile();
    break;
*/
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // if this is an update event, then handle it in SFApp,
    // otherwise punt it to the SFEventDispacher.
    SFEvent sfevent((const SDL_Event) event);
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {

	// Tick
	gameTick++;
	if (gameTick > gameSpeed) {
		gameTick = 0;
		player->CanChangeDirectionAgain();
	}

	// Make player move every tick
	if (gameTick == 0) {
		if (player->FacingNorth()) {
			player->GoNorth();
		}
		if (player->FacingSouth()) {
			player->GoSouth();
		}
		if (player->FacingEast()) {
			player->GoEast();
		}
		if (player->FacingWest()) {
			player->GoWest();
		}
	}

	// Update tail(s)
	for(auto t: playerTails) {
		// Go to location of tail-1 or head if tail section clostest to head
	}

  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  for(auto c: coins) {
    // coins do nothing yet
  }

  // Update enemy positions
  for(auto a : aliens) {
    // do something here
  }

  // Detect collisions
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
    }
  }
  // Detect if player collides with coin
  for(auto c : coins) {
  	if(player->CollidesWith(c)) {
			fire++;
			points++;
			cout << "Number of points : " << (points-3) << ". Game speed: " << gameSpeed << endl;
			if (gameSpeed == 1) {
				cout << "CONGRADULATIONS! You win!" << endl;
				// Replace following line with some message or fireworks or crap showing you won
				cout << "The test is now over, YOU DON'T NEED ANY MORE POINTS." << endl;
				std::stringstream sstm;
	  		sstm << "You win! Points: " << points-3; //Players start with 3 points, because head +3 body parts.
	  		SDL_WM_SetCaption(sstm.str().c_str(),  sstm.str().c_str());
			} else {
				if (gameSpeed > 1) {
					gameSpeed--;
					// The more points you have, the faster the game will get
					std::stringstream sstm;
		  		sstm << "Points: " << points-3; //Players start with 3 points, because head +3 body parts.
		  		SDL_WM_SetCaption(sstm.str().c_str(),  sstm.str().c_str());
				}
			}
  		c->HandleCollision();
  		NewCoin();
  	}
  }
  
	// Detect if player collides with a tail section
	list<shared_ptr<SFAsset>> tailsTmp;
	for(auto t : playerTails) {
		if(player->CollidesWith(t)) {
			cout << "You crashed you dead :(" << endl;
		}
	}
	
	
	// Remove collected coins
  list<shared_ptr<SFAsset>> coinsTmp;
	for(auto c : coins) {
		if(c->IsAlive()) {
			coinsTmp.push_back(c);
		}
	}
	coins.clear();
  coins = list<shared_ptr<SFAsset>>(coinsTmp);

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);
}

void SFApp::OnRender() {
  // clear the surface
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 54, 54, 54) );

  // draw the player
  player->OnRender(surface);

  for(auto t: playerTails) {
    if(t->IsAlive()) {t->OnRender(surface);}
  }

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender(surface);}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender(surface);}
  }

  for(auto c: coins) {
    if(c->IsAlive()) {c->OnRender(surface);}
  }

  // Switch the off-screen buffer to be on-screen
  SDL_Flip(surface);
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}

void SFApp::NewCoin() {
// remember 20x20 grid
  // Generate 2 random numbers between 1 and 20.
	int randomX = rand() % 20 + 1;
	int randomY = rand() % 20 + 1;
  auto coin = make_shared<SFAsset>(SFASSET_COIN);
  auto pos  = Point2((randomX*32)-16, (randomY*32)+14); // coin.png is 30x30, grid sections 32x32
  coin->SetPosition(pos);
  coins.push_back(coin);
  cout << "Coin placed at (" << randomX << "," << randomY << ")" << endl;
}
