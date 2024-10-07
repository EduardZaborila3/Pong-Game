#include <iostream>
#include <raylib.h>

using namespace std;

Color Blue = Color{ 74, 162, 209, 255 };
Color Dark_Blue = Color{ 13, 116, 172, 255 };
Color Light_Blue = Color{ 132, 210, 252, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0, cpu_score = 0;
bool start = false;

class Ball
{
public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, Yellow);
	}

	void Update() {
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0) {
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) { //Cpu wins
			cpu_score++;
			ResetBall();
		}

		if (x - radius <= 0) {
			player_score++;
			ResetBall();
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = { -1, 1 };
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}
};

class Paddle
{
protected:
	void LimitMovement() {
		if (y <= 0) {
			y = 0;
		}

		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}

public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
	}

	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y = y - speed;
		}

		if (IsKeyDown(KEY_DOWN)) {
			y = y + speed;
		}

		if (y <= 0) {
			y = 0;
		}

		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}

		LimitMovement();
	}
};

class cpuPaddle : public Paddle
{
public:
	void Update(int ball_y) {
		if (y + height / 2 > ball_y) {
			y = y - speed;
		}

		if (y + height / 2 <= ball_y) {
			y = y + speed;
		}

		LimitMovement();
	}
};

Ball ball;
Paddle player;
cpuPaddle cpu;

int main()
{
	cout << "Starting the game!" << '\n';
	const int screen_width = 1280;
	const int screen_height = 800;
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.width = 25;
	player.height = 120;
	player.x = screen_width - player.width - 10;
	player.y = screen_height / 2 - player.height / 2;
	player.speed = 6;

	cpu.width = 25;
	cpu.height = 120;
	cpu.x = 10;
	cpu.y = screen_height / 2 - cpu.height / 2;
	cpu.speed = 6;

	InitWindow(screen_width, screen_height, "My pong game!");

	while (WindowShouldClose() == false) {
		BeginDrawing();

		if (start == false) {
			ClearBackground(Dark_Blue);
			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Blue);
			DrawText("Press SPACE to start", screen_width / 2 - 180, screen_height / 2 - 200, 30, WHITE);
			DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
			ball.Draw();
			cpu.Draw();
			player.Draw();
			DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
			DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
			if (IsKeyPressed(KEY_SPACE)) {
				WaitTime(0.5);
				start = true;
			}
		}


		if (start == true) {
			//Updating
			ball.Update();
			player.Update();
			cpu.Update(ball.y);

			//Checking for collisions
			if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
				ball.speed_x *= -1;
			}

			if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
				ball.speed_x *= -1;
			}

			//Drawing
			ClearBackground(Dark_Blue);
			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Blue);
			DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Blue);
			DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
			ball.Draw();
			cpu.Draw();
			player.Draw();
			DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
			DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
		}

		

		EndDrawing();
	}

	CloseWindow();
	return 0;
}