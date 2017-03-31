void StateSprite::load(int state, const std::string& name) {
  load(state, Sprite(name));
}
void StateSprite::load(int state, Sprite sprite) {
  sprites[state] = sprite;
}
void StateSprite::update() {
  update(0.033f);
}
void StateSprite::update(float delta) {
  for(Sprites::iterator i = sprites.begin(); i != sprites.end(); ++i) {
    (*i).second.update(delta);
  }
}
Sprite& StateSprite::operator[](int state) {
  Sprites::iterator i = sprites.find(state);
  if (i != sprites.end()) {
    return i->second;
  }
}
