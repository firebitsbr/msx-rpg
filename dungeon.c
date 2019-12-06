//------------------------------------------------------------------
// Dungeon Crawler for MSX2 using Fusion-C
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_graph2.h"

#include "main.h"
#include "dungeon.h"

//------------------------------------------------------------------
// Variables.
//------------------------------------------------------------------

unsigned const char dungeon_01[DUNGEON_SIZE*DUNGEON_SIZE] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

const int dir_translate_x[] = {0, 1, 0, -1};
const int dir_translate_y[] = {-1, 0, 1, 0};

char player_moves;

int player_pos_x;
int player_pos_y;

enum Direction player_dir;

char joy;
char trig;

//------------------------------------------------------------------
// Functions.
//------------------------------------------------------------------

void sf_draw_third_wall_far_left(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(THIRD_WALL_FAR_LEFT_X, THIRD_WALL_FAR_LEFT_Y,
					   THIRD_WALL_FAR_DX, THIRD_WALL_FAR_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_FAR_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_far_right(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(THIRD_WALL_FAR_RIGHT_X, THIRD_WALL_FAR_RIGHT_Y,
					   THIRD_WALL_FAR_DX, THIRD_WALL_FAR_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - THIRD_WALL_FAR_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_FAR_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_left(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(THIRD_WALL_LEFT_X, THIRD_WALL_LEFT_Y,
					   THIRD_WALL_DX, THIRD_WALL_DY,
					   DUNGEON_SCREEN_X + THIRD_WALL_OFFSET_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_right(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(THIRD_WALL_RIGHT_X, THIRD_WALL_RIGHT_Y,
					   THIRD_WALL_DX, THIRD_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - THIRD_WALL_DX - THIRD_WALL_OFFSET_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_far_left(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(SECOND_WALL_FAR_LEFT_X, SECOND_WALL_FAR_LEFT_Y,
					   SECOND_WALL_FAR_DX, SECOND_WALL_FAR_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_FAR_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_far_right(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(SECOND_WALL_FAR_RIGHT_X, SECOND_WALL_FAR_RIGHT_Y,
					   SECOND_WALL_FAR_DX, SECOND_WALL_FAR_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - SECOND_WALL_FAR_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_FAR_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_left(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(SECOND_WALL_LEFT_X, SECOND_WALL_LEFT_Y,
					   SECOND_WALL_DX, SECOND_WALL_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_right(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(SECOND_WALL_RIGHT_X, SECOND_WALL_RIGHT_Y,
					   SECOND_WALL_DX, SECOND_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - SECOND_WALL_DX, DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_first_wall_left(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(FIRST_WALL_LEFT_X, FIRST_WALL_LEFT_Y,
					   FIRST_WALL_DX, FIRST_WALL_DY,
					   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y,
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_first_wall_right(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(FIRST_WALL_RIGHT_X, FIRST_WALL_RIGHT_Y,
					   FIRST_WALL_DX, FIRST_WALL_DY,
					   DUNGEON_SCREEN_X + DUNGEON_SCREEN_DX - FIRST_WALL_DX, DUNGEON_SCREEN_Y,
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_third_wall_front(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(THIRD_FRONT_WALL_X, THIRD_FRONT_WALL_Y,
					   THIRD_FRONT_WALL_DX, THIRD_FRONT_WALL_DY,
					   DUNGEON_SCREEN_X + ((DUNGEON_SCREEN_DX - THIRD_FRONT_WALL_DX) * 0.5), DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - THIRD_FRONT_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

void sf_draw_second_wall_front(int x, int y)
{
	if (x >= 0 && x < DUNGEON_SIZE &&
		y >= 0 && y < DUNGEON_SIZE &&
		dungeon_01[x + y * DUNGEON_SIZE] == 1)
	{
		sf_screen_copy(SECOND_FRONT_WALL_X, SECOND_FRONT_WALL_Y,
					   SECOND_FRONT_WALL_DX, SECOND_FRONT_WALL_DY,
					   DUNGEON_SCREEN_X + ((DUNGEON_SCREEN_DX - SECOND_FRONT_WALL_DX) * 0.5), DUNGEON_SCREEN_Y + ((DUNGEON_SCREEN_DY - SECOND_FRONT_WALL_DY) * 0.5),
					   WL_PG, BF_PG, LOGICAL_TIMP);
	}
}

/*
 * This is the area to draw:
 *
 *  x x x x x
 *  x x x x x
 *    x o x
 *
 * horizontal order: far left, left, front, right, far right.
 * vertical order: third, second, first.
 */
void sf_draw_dungeon_walls(void)
{
	int x, y, xd, yd;

	int mult = 1;
	if (player_dir == North || player_dir == South)
	{
		mult = -1;
	}

	// Draw by distance.
	for (int dist = 2; dist > -1; dist--)
	{
		x = player_pos_x + (dist * dir_translate_x[player_dir]);
		y = player_pos_y + (dist * dir_translate_y[player_dir]);

		// Far left walls.
		xd = x - (2 * mult * dir_translate_y[player_dir]);
		yd = y - (2 * mult * dir_translate_x[player_dir]);
		if (dist == 2)
			sf_draw_third_wall_far_left(xd, yd);
		else if (dist == 1)
			sf_draw_second_wall_far_left(xd, yd);

		// Left walls.
		xd = x - (mult * dir_translate_y[player_dir]);
		yd = y - (mult * dir_translate_x[player_dir]);
		if (dist == 2)
			sf_draw_third_wall_left(xd, yd);
		else if (dist == 1)
			sf_draw_second_wall_left(xd, yd);
		else
			sf_draw_first_wall_left(xd, yd);

		// Far right walls.
		xd = x + (2 * mult * dir_translate_y[player_dir]);
		yd = y + (2 * mult * dir_translate_x[player_dir]);
		if (dist == 2)
			sf_draw_third_wall_far_right(xd, yd);
		else if (dist == 1)
			sf_draw_second_wall_far_right(xd, yd);

		// Right walls.
		xd = x + (mult * dir_translate_y[player_dir]);
		yd = y + (mult * dir_translate_x[player_dir]);
		if (dist == 2)
			sf_draw_third_wall_right(xd, yd);
		else if (dist == 1)
			sf_draw_second_wall_right(xd, yd);
		else
			sf_draw_first_wall_right(xd, yd);

		// Front walls.
		if (dist == 2)
			sf_draw_third_wall_front(x, y);
		else if (dist == 1)
			sf_draw_second_wall_front(x, y);
	}
}

void sf_draw_tiles_background(void)
{
	// TEST: Draw tiles background.
	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 26; y++)
		{
			sf_screen_copy(160, 0,
						   8, 8,
						   x * 8, y * 8,
						   BG_PG, BF_PG, opHMMM);
		}
	}
}

void sf_draw_avatars(void)
{
	sf_screen_copy(0, 120,
				   32, 32,
				   180, 1 * 32 + 4,
				   BG_PG, BF_PG, opHMMM);

	sf_screen_copy(32, 120,
				   32, 32,
				   180, 2 * 32 + 4 + 8,
				   BG_PG, BF_PG, opHMMM);

	sf_screen_copy(64, 120,
				   32, 32,
				   180, 3 * 32 + 4 + 8 + 8,
				   BG_PG, BF_PG, opHMMM);
}

void sf_draw_combat_menu(void)
{
	Rect(16, 159,
		 224, 167,
		 2, FILL_ALL);

	SetColors(15, 0, 0);
	PutText(20, 160, "ATTACK", LOGICAL_TIMP);

	SetColors(8, 0, 0);
	PutText(20, 160 + 9, "MAGIC", LOGICAL_IMP);
	PutText(20, 160 + 9 + 9, "DEFEND", LOGICAL_IMP);
	PutText(20, 160 + 9 + 9 + 9, "ITEMS", LOGICAL_IMP);

	// HP bars
	Rect(180 + 32 + 2, 1 * 32 + 4,
		 180 + 32 + 2 + 4, 1 * 32 + 4 + 32 - 1,
		 11, FILL_ALL);

	Rect(180 + 32 + 2, 2 * 32 + 4 + 8,
		 180 + 32 + 2 + 4, 2 * 32 + 4 + 32 + 8 - 1,
		 11, FILL_ALL);

	Rect(180 + 32 + 2, 3 * 32 + 4 + 8 + 8,
		 180 + 32 + 2 + 4, 3 * 32 + 4 + 32 + 8 + 8 - 1,
		 11, FILL_ALL);
}

// Draw the current dungeon room.
void sf_draw_dungeon_view(void)
{
	db_state = Updating;

	// #SAFFRON test.
	sf_draw_avatars();

	// Dungeon background.
	sf_screen_copy(0, 0,
				   DUNGEON_SCREEN_DX, DUNGEON_SCREEN_DY,
				   DUNGEON_SCREEN_X, DUNGEON_SCREEN_Y,
				   BG_PG, BF_PG, opHMMM);

	// Dungeon walls.
	sf_draw_dungeon_walls();

	// Enemies.
	// #TODO
	// sf_screen_copy (215,0, 41,64, 100,90, 3, 0, LOGICAL_TIMP);

	db_state = ReadyToTransfer;
}

void sf_draw_minimap(void)
{
	for (int x = 0; x < DUNGEON_SIZE; x++)
	{
		for (int y = 0; y < DUNGEON_SIZE; y++)
		{
			if (dungeon_01[x + y * DUNGEON_SIZE] == 1)
			{
				Pset(x + 32, y, 10, 0);
			}
			else if (player_pos_x == x && player_pos_y == y)
			{
				Pset(x + 32, y, 13, 0);
			}
		}
	}
}
void sf_move(int newPosX, int newPosY)
{
	if (newPosX > 0 && newPosX < DUNGEON_SIZE - 1 &&
		newPosY > 0 && newPosY < DUNGEON_SIZE - 1 &&
		dungeon_01[newPosX + newPosY * DUNGEON_SIZE] == 0)
	{
		player_pos_x = newPosX;
		player_pos_y = newPosY;

		player_moves = TRUE;
	}
}

void sf_rotate_left(void)
{
	if (--player_dir < North)
	{
		player_dir = West;
	}

	player_moves = TRUE;
}

void sf_rotate_right(void)
{
	if (++player_dir > West)
	{
		player_dir = North;
	}

	player_moves = TRUE;
}

// Reads joystick input, from keyboard's arrow keys and joystick port 1.
void sf_update_joy_dungeon_mode(void)
{
	for (int i = 0; i < 1; i++)
	{
		joy = JoystickRead(i);
		trig = TriggerRead(i);

		// #TODO
		switch (joy)
		{
		case JOY_UP:
			sf_move(player_pos_x + dir_translate_x[player_dir],
					player_pos_y + dir_translate_y[player_dir]);
			break;

		case JOY_DOWN:
			sf_move(player_pos_x - dir_translate_x[player_dir],
					player_pos_y - dir_translate_y[player_dir]);
			break;

		case JOY_LEFT:
			sf_rotate_left();
			break;

		case JOY_RIGHT:
			sf_rotate_right();
			break;
		}
	}
}

void sf_draw_screen_dungeon_mode(void)
{
	if (db_state == ReadyToTransfer)
	{
		db_state = Transfering;

		// Copy to actual screen.
		sf_screen_copy(0, 0,
					   256, 212,
					   0, 0,
					   BF_PG, 0, opYMMM);

		// Using this to check screen redrawing. Actual game text will be handled differently.
		SetColors(9, 0, 0);

		if (player_dir == North)
			PutText(80, 20, "North", LOGICAL_IMP);
		else if (player_dir == East)
			PutText(80, 20, "East", LOGICAL_IMP);
		else if (player_dir == South)
			PutText(80, 20, "South", LOGICAL_IMP);
		else
			PutText(80, 20, "West", LOGICAL_IMP);

		// #SAFFRON test.
		sf_draw_combat_menu();

		// Debug: draw minimap.
		//sf_draw_minimap ();

		// Debug: draw palette.
		//sf_draw_palette ();

		db_state = Finished;
	}
}

void sf_set_dungeon_mode(void)
{
	player_moves = FALSE;

	player_pos_x = 1;
	player_pos_y = 4;

	player_dir = North;

	sf_draw_dungeon_view();
}

void sf_update_dungeon_mode(void)
{
	sf_update_joy_dungeon_mode();

	// Update screen if needed.
	if (player_moves)
	{
		sf_draw_dungeon_view();

		player_moves = FALSE;
	}
}