#include <algorithm>
#include <vector>
#include <queue>
#include <SDL3/SDL_rect.h>

bool operator==(const SDL_Point& a, const SDL_Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const SDL_Point& a, const SDL_Point& b)
{
    return a.x != b.x || a.y != b.y;
}

std::vector<SDL_Point> getAdjacents(SDL_Point root, const std::vector<std::vector<int>>& matrix)
{
    std::vector<SDL_Point> adjacents;
    if (root.y - 1 >= 0 && matrix[root.x][root.y - 1])
    {
        adjacents.push_back({root.x, root.y - 1});
    }
    if (root.y + 1 < matrix[root.x].size() && matrix[root.x][root.y + 1])
    {
        adjacents.push_back({root.x, root.y + 1});
    }
    if (root.x - 1 >= 0 && matrix[root.x - 1][root.y])
    {
        adjacents.push_back({root.x - 1, root.y});
    }
    if (root.x + 1 < matrix.size() && matrix[root.x + 1][root.y])
    {
        adjacents.push_back({root.x + 1, root.y});
    }
    return adjacents;
}

std::vector<SDL_Point> recreatePath(const std::vector<std::vector<SDL_Point>>& parents, SDL_Point start, SDL_Point end)
{
    if (parents[end.x][end.y] == SDL_Point{-1, -1} && start != end)
    {
        return {};
    }

    std::vector<SDL_Point> path;
    SDL_Point current = end;

    while (current != start)
    {
        path.push_back(current);
        current = parents[current.x][current.y];
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<SDL_Point> bfs(
    const std::vector<std::vector<int>>& matrix,
    const SDL_Point start = {0, 0},
    const SDL_Point end = {19, 19})
{
    std::vector<std::vector<bool>> visited(matrix.size(), std::vector<bool>(matrix[0].size(), false));

    std::queue<SDL_Point> Q;

    std::vector<std::vector<SDL_Point>> parents(matrix.size(), std::vector<SDL_Point>(matrix[0].size(), {-1, -1}));
    visited[start.x][start.y] = true;
    Q.push(start);

    while (!Q.empty())
    {
        SDL_Point v = Q.front();
        Q.pop();

        if (v == end)
        {
            return recreatePath(parents, start, end);
        }

        std::vector<SDL_Point> adjacents = getAdjacents(v, matrix);
        for (const SDL_Point& coord : adjacents)
        {
            if (!visited[coord.x][coord.y])
            {
                visited[coord.x][coord.y] = true;
                Q.push(coord);
                // the parent of the node at position (coord.x, coord.y) is the node at position (v.x, v.y)
                parents[coord.x][coord.y] = v;
            }
        }
    }
}