#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
struct Cell
{
	int i;
	int j;
	int type;
};

class PF : public::olc::PixelGameEngine {
private:
	Cell** cells;
	int cellSize;
	int offSet;
	int cols;
	int rows;
	int selectedMenuId;
	std::string menuItems[3] = {"Dijkstra's algorithm", "A* algorithm", "Deneme"};
	bool isRunning = false;
public:

	bool OnUserCreate() {
		cellSize = 30;
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
		cells[0][0].type = 1;
		cells[0][1].type = 3;

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
		DrawString(ScreenWidth() - 300,7, "Time elapsed: 25s", olc::WHITE, 2);
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
	}

	void DrawCell(Cell currentCell) {
		int i = currentCell.i;
		int j = currentCell.j;
		olc::Pixel color;
		if (currentCell.type == 0)		// default
			color = olc::DARK_GREY;
		else if (currentCell.type == 1)		// start
			color = olc::BLUE;
		else if (currentCell.type == 2)	//	end
			color = olc::RED;
		else if (currentCell.type == 3)	// already looked
			color = olc::GREY;
		else if (currentCell.type == 4)	// currently looking
			color = olc::YELLOW;
		else if (currentCell.type == 5)	// wall
			color = olc::VERY_DARK_GREY;

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
				}else if (GetKey(olc::Key::E).bHeld) {
					// set type to end(2)
					selected->type = 2;
				}
				else {
					// set type to wall(5)
					selected->type = 5;
				}
			}
			else if (GetMouse(1).bHeld) {
				// set type to normal(0)
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
			}
		}
		else {
			if (GetKey(olc::Key::ENTER).bPressed) {
				isRunning = false;
			}
		}
		
	}

	void Calculate(float fElapsedTime) {
	
	}

};


int main() {
	PF pf;
	if (pf.Construct(1200, 900, 1, 1)) {
		pf.Start();
	}
	return 0;
}