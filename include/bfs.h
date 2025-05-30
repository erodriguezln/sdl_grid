#ifndef BFS_H
#define BFS_H
#include <algorithm>
#include <vector>
#include <queue>
#include <SDL3/SDL_rect.h>

bool operator==(const SDL_Point& a, const SDL_Point& b);
bool operator!=(const SDL_Point& a, const SDL_Point& b);

std::vector<SDL_Point> getAdjacent(
  SDL_Point root,
  const std::vector<std::vector<int>>& matrix
);
std::vector<SDL_Point> recreatePath(
  const std::vector<std::vector<SDL_Point>>& parents,
  SDL_Point start,
  SDL_Point end
);
std::vector<SDL_Point> bfs(
  const std::vector<std::vector<int>>& matrix,
  const SDL_Point start = { 0, 0 },
  const SDL_Point end = { 19, 19 }
);

#endif //BFS_H