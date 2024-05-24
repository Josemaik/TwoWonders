#pragma once

#include "../utils/types.hpp"

struct Cell
{
    using CellVec = std::vector<std::pair<Entity*, ColliderComponent*>>;

    void addEntity(Entity& entity, ColliderComponent& collider)
    {
        cellEntities_.push_back({ &entity, &collider });
    }

    CellVec& getEntities()
    {
        return cellEntities_;
    }

private:
    CellVec cellEntities_;
};

struct Grid
{
    Grid() {
        // Define the size of the grid and initialize the cells
        cells.resize(gridSize * gridSize);
    }

    Cell& getCell(const vec3d& position) {
        // Convert the position to a grid cell index
        int x = static_cast<int>(position.x()) / cellSize;
        int y = static_cast<int>(position.y()) / cellSize;

        // Ensure the index is within the grid bounds
        x = std::max(0, std::min(x, gridSize - 1));
        y = std::max(0, std::min(y, gridSize - 1));

        return cells[y * gridSize + x];
    }

    std::vector<Cell>& getCells() {
        return cells;
    }

    void clear() {
        for (auto& cell : cells) {
            cell.getEntities().clear();
        }
    }

private:
    static const int gridSize = 100;  // Define the size of the grid
    static const int cellSize = 10;   // Define the size of each cell
    std::vector<Cell> cells;
};