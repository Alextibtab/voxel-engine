#include "game.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main() {
  Game game;

  game.init(SCR_WIDTH, SCR_HEIGHT);
  game.run();

  return 1;
}
