#include "SFAsset.h"

int SFAsset::SFASSETID=0;

SFAsset::SFAsset(SFASSETTYPE type) {
  this->type = type;
  this->id   = ++SFASSETID;
  SDL_Surface * tmp_surf;

  switch (type) {
  case SFASSET_PLAYER:
    tmp_surf = IMG_Load("assets/player.png");
    break;
  case SFASSET_TAIL:
    tmp_surf = IMG_Load("assets/player.png");
    break;
  case SFASSET_PROJECTILE:
    tmp_surf = IMG_Load("assets/projectile.png");
    break;
  case SFASSET_ALIEN:
    tmp_surf = IMG_Load("assets/alien.png");
    break;
  case SFASSET_COIN:
    tmp_surf = IMG_Load("assets/coin.png");
    break;
  }

  if(!tmp_surf) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  sprite = SDL_DisplayFormatAlpha(tmp_surf);
  SDL_FreeSurface(tmp_surf);

  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), sprite->w, sprite->h));
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  bbox   = a.bbox;
  type   = a.type;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_FreeSurface(sprite);
    sprite = NULL;
  }
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */
Vector2 GameSpaceToScreenSpace(Vector2 &r) {
  return Vector2 (
		  r.getX(),
		  (SDL_GetVideoSurface()->h - r.getY())
		  );
}

void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

SFAssetId SFAsset::GetId() {
  return id;
}

void SFAsset::OnRender(SDL_Surface * level) {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * -1);
  Vector2 ss = GameSpaceToScreenSpace(gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_BlitSurface(sprite, NULL, level, &rect);
}

// 0,0 is at bottom left. Took too long to figure out -.-
void SFAsset::GoWest() {
	Vector2 c = *(bbox->centre) + Vector2(-32.0f, 0.0f);
	bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
  //cout << "Something's going West" << endl;
  if (c.getX() < 0) {
  	cout << "Hopping to East side of screen" << endl;
  	// Bring back onto the other side of screen if fallen off the edge
  	Vector2 c = *(bbox->centre) + Vector2((SDL_GetVideoSurface()->w),0.0f);
  	bbox->centre.reset();
  	bbox->centre = make_shared<Vector2>(c);
	}
}

void SFAsset::GoEast() {
  Vector2 c = *(bbox->centre) + Vector2(32.0f, 0.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
  if(c.getX() > (SDL_GetVideoSurface()->w)-16.0f) {
  	cout << "Hopping to West side of screen" << endl;
  	Vector2 c = *(bbox->centre) + Vector2(-(SDL_GetVideoSurface()->w), 0.0f);
    bbox->centre.reset();
  	bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoNorth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, 32.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
  if(c.getY() > ((SDL_GetVideoSurface()->h)+16.0f)) {
  	cout << "Hopping to South side of screen" << endl;
  	Vector2 c = *(bbox->centre) + Vector2(0.0f,(-(SDL_GetVideoSurface()->h)));
  	bbox->centre.reset();
  	bbox->centre = make_shared<Vector2>(c);
	}
}

void SFAsset::GoSouth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -32.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
  if (c.getY() < 16) {
  	cout << "Hopping to North side of screen" << endl;
  	Vector2 c = *(bbox->centre) + Vector2(0.0f,(SDL_GetVideoSurface()->h));
  	bbox->centre.reset();
  	bbox->centre = make_shared<Vector2>(c);
	}
}

// Change direction if the intended direction isn't straight forwards or backwards (i.e
// only let the player change to left or right)
void SFAsset::FaceNorth() {
	if ((facingNorth == false) && (facingSouth == false) && (canChangeDirection == true)) {
		facingNorth = true;
		facingSouth = false;
		facingEast = false;
		facingWest = false;
		canChangeDirection = false;
	}
}

void SFAsset::FaceSouth() {
	if ((facingNorth == false) && (facingSouth == false) && (canChangeDirection == true)) {
		facingNorth = false;
		facingSouth = true;
		facingEast = false;
		facingWest = false;
		canChangeDirection = false;
	}
}

void SFAsset::FaceEast() {
	if ((facingEast == false) && (facingWest == false) && (canChangeDirection == true)) {
		facingNorth = false;
		facingSouth = false;
		facingEast = true;
		facingWest = false;
		canChangeDirection = false;
	}
}

void SFAsset::FaceWest() {
	if ((facingEast == false) && (facingWest == false) && (canChangeDirection == true)) {
		facingNorth = false;
		facingSouth = false;
		facingEast = false;
		facingWest = true;
		canChangeDirection = false;
	}
}

bool SFAsset::FacingNorth() {
  return (facingNorth == true);
}

bool SFAsset::FacingSouth() {
  return (facingSouth == true);
}

bool SFAsset::FacingEast() {
  return (facingEast == true);
}

bool SFAsset::FacingWest() {
  return (facingWest == true);
}

void SFAsset::CanChangeDirectionAgain() {
  canChangeDirection = true;
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

void SFAsset::SetNotAlive() {
  type = SFASSET_DEAD;
}

bool SFAsset::IsAlive() {
  return (SFASSET_DEAD != type);
}

void SFAsset::HandleCollision() {
  if(SFASSET_PROJECTILE == type || SFASSET_ALIEN == type || SFASSET_COIN == type) {
    SetNotAlive();
  }
}
