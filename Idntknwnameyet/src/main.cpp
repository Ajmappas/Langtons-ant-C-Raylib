#include <iostream>
#include <vector>

#include <raylib.h>

#include "imgui.h"
#include "rImGui/rlImGui.h"

bool Button(const char* text, float width, float height, float x, float y) {
	DrawRectangle(x, y, width, height, BLUE);
	DrawText(text, x + width/2 , y + height/2, 20, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), { x, y, width, height })) {
		DrawRectangle(x, y, width, height, DARKBLUE);
		if (IsMouseButtonPressed(0)) {
			return true;
		}
	}

	return false;
}

bool cmpColors(Color c1, Color c2) {
	if (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b) return true;

	return false;
}

struct Tile {
	Vector2 position;
	int size = 16;
	Color color = WHITE;
};

enum Direction { UP, RIGHT, DOWN, LEFT };

void UpdateLangtonsAnt(Vector2& pos, Direction& dir, std::vector<std::vector<Tile>>& tiles) {
	int rows = tiles.size();
	int cols = tiles[0].size();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Tile& t = tiles[i][j];
			Rectangle antRect = { pos.x, pos.y, 16, 16 };
			Rectangle tileRect = { t.position.x, t.position.y, t.size, t.size }; // assuming square tiles

			if (CheckCollisionRecs(antRect, tileRect)) {
				if (cmpColors(t.color, WHITE)) {
					t.color = BLACK;
					// Turn right
					dir = static_cast<Direction>((dir + 1) % 4);
				}
				else if (cmpColors(t.color, BLACK)) {
					t.color = WHITE;
					// Turn left
					dir = static_cast<Direction>((dir + 3) % 4);
				}

				// Move forward
				switch (dir) {
				case UP:
					pos.y -= t.size;
					break;
				case RIGHT:
					pos.x += t.size;
					break;
				case DOWN:
					pos.y += t.size;
					break;
				case LEFT:
					pos.x -= t.size;
					break;
				}
				return; // Exit after processing the current tile
			}
		}
	}
}

bool start = false;

int main() {

	size_t screenWidth = 800;
	size_t screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Langton's ant");
	SetTargetFPS(60);
	
	rlImGuiSetup(true);

	Vector2 antPos = { screenWidth / 2.0f, screenHeight / 2.0f };
	Direction antDir = UP;

	const int tileSize = 16;
	const int rows = screenHeight / tileSize;
	const int cols = screenWidth / tileSize;
	std::vector<std::vector<Tile>> tiles(rows, std::vector<Tile>(cols));

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			tiles[i][j] = { { (float)j * tileSize, (float)i * tileSize }, tileSize, WHITE };
		}
	}

	while (!WindowShouldClose()) {
		if(start)
			UpdateLangtonsAnt(antPos, antDir, tiles);
		
		BeginDrawing();
		ClearBackground(WHITE);
		
		for (const auto& row : tiles) {
			for (const auto& tile : row) {
				DrawRectangleV(tile.position, { (float)tile.size, (float)tile.size}, tile.color);
			}
		}

		DrawRectangleV(antPos, { 16, 16 }, RED);
		
		rlImGuiBegin();			// starts the ImGui content mode. Make all ImGui calls after this
		
		
		ImGui::Begin("Properties");
		
		if (ImGui::Button("Start")) {
			start = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop")) {
			start = false;
		}

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();
	
	return 0;
}