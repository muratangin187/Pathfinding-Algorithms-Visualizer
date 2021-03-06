﻿#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
struct Cell
{
	int i;
	int j;
	int type;
	unsigned int d_cost = INFINITE;
	Cell* d_prev = nullptr;
	//A*
	unsigned int f = INFINITE;
	unsigned int g = INFINITE;
	unsigned int h;
};

class PF : public::olc::PixelGameEngine {
private:
	Cell** cells;
	Cell* start;
	Cell* end;
	Cell* current;
	int cellSize;
	int offSet;
	int cols;
	int rows;
	int selectedMenuId;
	float time = 0;
	std::string menuItems[3] = {"Dijkstra's algorithm", "A* algorithm", "Deneme"};
	bool isRunning = false;
	bool isFinished = false;
	float k = 0;
	std::vector<Cell*> openSet;
public:

	bool OnUserCreate() {
		cellSize = 40;
		offSet = 30;
		selectedMenuId = 0;
		sAppName = "Pathfinding Algorithms";

		cols = ScreenWidth() / cellSize;
		rows = (ScreenHeight() - offSet) / cellSize;

		cells = new Cell * [cols];

		for (size_t i = 0; i < cols; i++)
		{
			cells[i] = new Cell[rows];
			for (size_t j = 0; j < rows; j++)
			{
				cells[i][j] = Cell{(int)i,(int)j,0};
			}
		}

		return true;
	}
	
	bool OnUserUpdate(float fElapsedTime) {
		Clear(olc::BLACK);
		Render(fElapsedTime);
		TakeInput(fElapsedTime);
		Calculate(fElapsedTime);
		return true;
	}

	void Render(float fElapsedTime) {
		RenderMenu();
		RenderMaze();
	}
	
	// Menu will take top 30px
	// Menu will show a option for selecting algorithm
	// and show time elapsed for algorithm
	void RenderMenu() {
		DrawRect(0,0,ScreenWidth()-1, offSet);
		FillTriangle(5, 13, 8, 5, 11, 13);
		FillTriangle(5, 17, 8, 25, 11, 17);
		DrawString(15,7, menuItems[selectedMenuId],isRunning ? olc::RED : olc::WHITE, 2);
		DrawString(ScreenWidth() - 400,7, "Time elapsed: " + std::to_string(time), olc::WHITE, 2);
	}

	void RenderMaze() {
		for (size_t i = 0; i < cols; i++)
		{
			for (size_t j = 0; j < rows; j++)
			{
				DrawCell(cells[i][j]);
				DrawRect(i * cellSize, offSet + j * cellSize, cellSize, cellSize, olc::BLACK);
			}
		}

		if (isRunning) {
			if (start != nullptr)
				FillRect(start->i * cellSize, offSet + start->j * cellSize, cellSize, cellSize, olc::DARK_BLUE);
			if (end != nullptr)
				FillRect(end->i * cellSize, offSet + end->j * cellSize, cellSize, cellSize, olc::RED);
		}
	}

	void DrawCell(Cell currentCell) {
		int i = currentCell.i;
		int j = currentCell.j;
		olc::Pixel color;
		if (currentCell.type == 0)		// default
			color = olc::DARK_GREY;
		else if (currentCell.type == 1)		// start
			color = olc::DARK_BLUE;
		else if (currentCell.type == 2)	//	end
			color = olc::RED;
		else if (currentCell.type == 3)	// already looked
			color = olc::GREY;
		else if (currentCell.type == 4)	// currently looking
			color = olc::YELLOW;
		else if (currentCell.type == 5)	// wall
			color = olc::VERY_DARK_GREY;
		else if (currentCell.type == 6)	// path
			color = olc::BLUE;

		FillRect(i * cellSize, offSet + j * cellSize, cellSize, cellSize, color);
	}
	
	void TakeInput(float fElapsedTime) {
		if (!isRunning) {
			int mouseX = GetMouseX();
			int mouseY = GetMouseY() - offSet;
			int proceededX = mouseX / cellSize;
			int proceededY = mouseY / cellSize;
			//std::cout << "X:" << proceededX << " Y:" << proceededY << std::endl;
			Cell* selected = &cells[(int)ceil(proceededX)][(int)ceil(proceededY)];
			if (GetMouse(0).bHeld) {
				if (GetKey(olc::Key::S).bHeld) {
					// set type to start(1)
					selected->type = 1;
					start = selected;
				}else if (GetKey(olc::Key::E).bHeld) {
					// set type to end(2)
					selected->type = 2;
					end = selected;
					current = end;
				}
				else {
					// set type to wall(5)
					selected->type = 5;
				}
			}
			else if (GetMouse(1).bHeld) {
				// set type to normal(0)
				if(selected->type == 1)
					start->d_cost = INFINITE;
				selected->type = 0;
			}

			if (GetKey(olc::Key::UP).bPressed) {
				selectedMenuId++;
				selectedMenuId = selectedMenuId % 3;
			}else if (GetKey(olc::Key::DOWN).bPressed) {
				selectedMenuId--;
				if(selectedMenuId < 0)
				selectedMenuId += 3;
			}
			if (GetKey(olc::Key::ENTER).bPressed) {
				isRunning = true;
				isFinished = false;
				start->d_cost = 0;
				start->g = 0;
				start->f = calculateH(start);
				openSet.push_back(start);
			}
			if (GetKey(olc::Key::ESCAPE).bPressed) {
				resetCells();
			}
		}
		else {
			if (GetKey(olc::Key::ENTER).bPressed) {
				isRunning = false;
			}
		}
		
	}

	void Calculate(float fElapsedTime) {
		if (isRunning) {
			switch (selectedMenuId)
			{
			case 0:	// Dijkstra
				DijkstraAlgorithm();
				break;
			case 1:
				AStarAlgorithm();
				break;
			case 2:
				break;
			default:
				break;
			}
			//isFinished = false;
			k += fElapsedTime;
			time = k;
		}
		else {
			if (isFinished) {
				for (size_t i = 0; i < cols; i++)
				{
					for (size_t j = 0; j < rows; j++)
					{
						if (cells[i][j].type == 4)	cells[i][j].type = 3;
					}
				}
				k = 0;
			}
		}
	}

	void DijkstraAlgorithm() {

		// type=0 not visited, type=3 already visited, type=4 currently visiting
		if (!isEveryCellVisited()) {
			Cell* selected = minDistCell();
			if (selected == nullptr) {
				std::cout << "No Solution" << std::endl;
				isRunning = false;
			}
			else {
				updateAdjCosts(selected);
				selected->type = 3;
			}
		}
		else 
		{
			if (current->d_prev != start) {
				current->type = 6;
				current = current->d_prev;
			}
			else {
				current->type = 6;
				isRunning = false;
				isFinished = true;
			}
			start->type = 1;
			end->type = 2;
		}


	}

	void resetCells() {
		for (size_t i = 0; i < cols; i++)
		{
			delete[] cells[i];
		}
		delete[] cells;
		cells = new Cell * [cols];
		openSet.clear();
		for (size_t i = 0; i < cols; i++)
		{
			cells[i] = new Cell[rows];
			for (size_t j = 0; j < rows; j++)
			{
				cells[i][j] = Cell{ (int)i,(int)j,0 };
			}
		}
		isRunning = false;
		isFinished = true;
	}

	void updateAdjCosts(Cell* selected) {
		Cell* leftC = nullptr;
		Cell* rightC = nullptr;
		Cell* upC = nullptr;
		Cell* downC = nullptr;
		if (selected->i > 0)
			leftC = &cells[selected->i - 1][selected->j];
		if (selected->i < cols-1)
			rightC = &cells[selected->i + 1][selected->j];
		if (selected->j > 0)
			upC = &cells[selected->i][selected->j - 1];
		if (selected->j < rows-1)
			downC = &cells[selected->i][selected->j + 1];

		unsigned int tempCost = selected->d_cost + 1;

		if (leftC != nullptr)
			if (leftC->d_cost > tempCost) {
				if (leftC->type != 5) {
					leftC->d_cost = tempCost;
					leftC->type = 4;
					leftC->d_prev = selected;
				}
			}
		if (rightC != nullptr)
			if (rightC->d_cost > tempCost) {
				if (rightC->type != 5) {
					rightC->d_cost = tempCost;
					rightC->type = 4;
					rightC->d_prev = selected;
				}
			}
		if (upC != nullptr)
			if (upC->d_cost > tempCost) {
				if (upC->type != 5) {
					upC->d_cost = tempCost;
					upC->type = 4;
					upC->d_prev = selected;
				}
			}
		if (downC != nullptr)
			if (downC->d_cost > tempCost) {
				if (downC->type != 5) {
					downC->d_cost = tempCost;
					downC->type = 4;
					downC->d_prev = selected;
				}
			}
	}

	bool isEveryCellVisited() {
		for (size_t i = 0; i < cols; i++)
		{
			for (size_t j = 0; j < rows; j++)
			{
				if (cells[i][j].type == 0) 
					return false;
			}
		}
		return true;
	}

	Cell* minDistCell() {
		unsigned int min = INFINITE;
		Cell* result = nullptr;
		for (size_t i = 0; i < cols; i++)
		{
			for (size_t j = 0; j < rows; j++)
			{
				if (cells[i][j].d_cost < min && (cells[i][j].type == 0 || cells[i][j].type == 1 || cells[i][j].type == 2 || cells[i][j].type == 4)) {
					if (cells[i][j].type == 4)
						cells[i][j].type == 3;
					min = cells[i][j].d_cost;
					result = &(cells[i][j]);
				}
			}
		}
		return result;
	}

	void AStarAlgorithm() {
		if (isFinished) {
			if (current->d_prev != start) {
				if(current != end)
					current->type = 6;
				current = current->d_prev;
			}
			else {
				current->type = 6;
				isFinished = true;
				isRunning = false;
			}
			end->type = 2;
		}
		else {
			if (openSet.size() != 0) {
				int currentIndex = lowestF();
				current = openSet.at(currentIndex);
				if (current == end) {
					isFinished = true;
				}
				else {
					openSet.erase(openSet.begin() + currentIndex);
					updateAdjAStar(current);
				}
			}
		}
		
	}
	void updateAdjAStar(Cell* selected) {
		Cell* leftC = nullptr;
		Cell* rightC = nullptr;
		Cell* upC = nullptr;
		Cell* downC = nullptr;
		if (selected->i > 0)
			leftC = &cells[selected->i - 1][selected->j];
		if (selected->i < cols - 1)
			rightC = &cells[selected->i + 1][selected->j];
		if (selected->j > 0)
			upC = &cells[selected->i][selected->j - 1];
		if (selected->j < rows - 1)
			downC = &cells[selected->i][selected->j + 1];

		unsigned int gCost = selected->g + 1;

		if (leftC != nullptr)
			if (leftC->g > gCost) {
				if (leftC->type != 5) {
					leftC->g = gCost;
					leftC->type = 4;
					leftC->h = calculateH(leftC);
					leftC->f = leftC->g + leftC->h;
					leftC->d_prev = selected;
					if (std::find(openSet.begin(), openSet.end(), leftC) != openSet.end()) {
						//found

					}
					else {
						//not found
						openSet.push_back(leftC);
					}

				}
			}
		if (rightC != nullptr)
			if (rightC->g > gCost) {
				if (rightC->type != 5) {
					rightC->g = gCost;
					rightC->type = 4;
					rightC->h = calculateH(rightC);
					rightC->f = rightC->g + rightC->h;
					rightC->d_prev = selected;
					if (std::find(openSet.begin(), openSet.end(), rightC) != openSet.end()) {
						//found

					}
					else {
						//not found
						openSet.push_back(rightC);
					}
				}
			}
		if (upC != nullptr)
			if (upC->g > gCost) {
				if (upC->type != 5) {
					upC->g = gCost;
					upC->type = 4;
					upC->h = calculateH(upC);
					upC->f = upC->g + upC->h;
					upC->d_prev = selected;
					if (std::find(openSet.begin(), openSet.end(), upC) != openSet.end()) {
						//found

					}
					else {
						//not found
						openSet.push_back(upC);
					}
				}
			}
		if (downC != nullptr)
			if (downC->g > gCost) {
				if (downC->type != 5) {
					downC->g = gCost;
					downC->type = 4;
					downC->h = calculateH(downC);
					downC->f = downC->g + downC->h;
					downC->d_prev = selected;
					if (std::find(openSet.begin(), openSet.end(), downC) != openSet.end()) {
						//found

					}
					else {
						//not found
						openSet.push_back(downC);
					}
				}
			}
	}


	int lowestF() {
		int min = 0;
		for (size_t i = 0; i < openSet.size(); i++)
		{
			if (openSet.at(min)->f > openSet.at(i)->f) {
				min = i;
			}
		}
		return min;
	}

	unsigned int calculateH(Cell* cell) {
		return abs(cell->i - end->i) + abs(cell->j - end->j);
	}



};


int main() {
	PF pf;
	if (pf.Construct(1200, 900, 1, 1)) {
		pf.Start();
	}
	return 0;
}