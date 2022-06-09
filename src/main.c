#include "uhero.h"
#include "core/logger.h"
#include "res/texture.h"

#include <SDL2/SDL_scancode.h>

#include <math.h>

struct game_s
{
	texture_t diffuse;
	sprite_t sprite;
	float scale;
	vec2_t pos;
};

result_t game_load(uhero_t* hero, void* user)
{
	struct game_s* game = (struct game_s*)user;

	result_t res = texture_loadFromFile(&(game->diffuse), "assets/logo.png");
	game->sprite = (sprite_t) {
		._tex = &(game->diffuse),
		.clip = vec4(0, 0, game->diffuse.width, game->diffuse.height)
	};
	game->scale = 128.0f / game->diffuse.width;

	game->pos = vec2(hero->window.width * 0.5, hero->window.height * 0.5);
	return res;
}

void game_clear(uhero_t* hero, void* user)
{
	struct game_s* game = (struct game_s*)user;

	texture_delete(&(game->diffuse));
}

void game_update(uhero_t* hero, void* user)
{
	struct game_s* game = (struct game_s*)user;

	game->scale = 0.25f + fabs(sin(hero->time)) * 0.5f;
	game->pos = vec2(hero->input.mouse_x,
		hero->window.height - hero->input.mouse_y
	);
	keystate_t escape = hero->input.keys[SDL_SCANCODE_ESCAPE];
	if (UHKEY_RELEASED == escape)
	{
		uhero_requestExit(hero);
		return;
	}

	const input_t* ip = &(hero->input);
	keycode_t s = SDL_SCANCODE_S;
	if (ip_keyPressed(ip, s))
		INFO("S pressed\n");
	if (ip_keyReleased(ip, s))
		INFO("S released\n");
	keystate_t state = hero->input.keys[s];
	INFO("state: %d\r", state);
	if (ip_keyDown(ip, s))
	{
		game->pos = vec2(64, 64);
	}
	if (ip_keyUp(ip, s))
	{
		game->pos = vec2(hero->window.width / 2, 0);
	}
}

void game_render(uhero_t* hero, void* user)
{
	struct game_s* game = (struct game_s*)user;

	render2d_t* r2d = &(hero->r2d);

	texture_bindSlost(&(game->diffuse), 0);

	render2d_begin(r2d);
	for (int i = 0; i < 4; i++)
	{
		render2d_drawSpriteEx(r2d, &(game->sprite),
			vec2(game->pos.x + i * game->diffuse.width * game->scale,
				game->pos.y
			),
			game->scale, 0.0f
		);
	}
	render2d_end(r2d);
}

int main(int argc, char** args)
{
	uhero_t engine = {};
	result_t res = uhero_init(&engine, argc, args);
	if (UH_SUCCESS != res)
	{
		ERROR("Failed to create UHeroEngine\n");
		return 0;
	}

	INFO("Setup complete\n");
	LOG("------------------------------------------------------------------------------\n");

	struct game_s game = {};
	uhero_setLevel(&engine, &game,
		game_load, game_clear, game_update, game_render
	);
	
	while (!uhero_exitRequested(&engine))
	{
		uhero_tick(&engine);
	}

	LOG("------------------------------------------------------------------------------\n");
	INFO("Exiting\n");

	uhero_quit(&engine);
	return 0;
}