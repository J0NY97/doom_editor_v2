#ifndef TEMP_H
# define TEMP_H

#define BMP_PATH "C:/Users/Jony/source/repos/map_editor_v2/temp_resources/"
#define BXPM_PATH "C:/Users/Jony/source/repos/map_editor_v2/temp_resources/"

enum e_player_action
{
	NONE,
	SHOOTING,
	CLICKING,
	SECTOR,
	EVENT_ACTION_AMOUNT
};

static const char	*g_event_action[EVENT_ACTION_AMOUNT] =
{
	"NONE",
	"SHOOT",
	"CLICK",
	"SECTOR"
};

enum e_event_type
{
	NONE_TYPE = 0,
	CEILING,
	FLOOR,
	STORE,
	HAZARD,
	AUDIO,
	SPAWN,
	LIGHT,
	EVENT_TYPE_AMOUNT
};

static const char	*g_event_type[EVENT_TYPE_AMOUNT] =
{
	"None",
	"Ceiling",
	"Floor",
	"Store",
	"Hazard",
	"Audio",
	"Spawn",
	"Light"
};

enum e_wsprite_action
{
	STATIC,
	LOOP,
	ACTION,
	SPRITE_TYPE_AMOUNT
};
static const char	*g_sprite_type[SPRITE_TYPE_AMOUNT] =
{
	"STATIC",
	"LOOP",
	"ACTION"
};

typedef struct s_id_and_path
{
	int			id;
	char		*path;
}				t_id_and_path;
# define MAP_TEXTURE_AMOUNT	21
static const t_id_and_path	g_map_textures[MAP_TEXTURE_AMOUNT] =
{	
	{0, BXPM_PATH"bullet_hole.bxpm"},
	{1, BXPM_PATH"vent.bxpm"},
	{2, BXPM_PATH"tile_floor.bxpm"},
	{3, BXPM_PATH"bars.bxpm"},
	{4, BXPM_PATH"wall_button.bxpm"},
	{5, BXPM_PATH"cog.bxpm"},
	{6, BXPM_PATH"lava.bxpm"},
	{7, BXPM_PATH"metal_wall.bxpm"},
	{8, BXPM_PATH"terminal.bxpm"},
	{9, BXPM_PATH"console.bxpm"},
	{10, BXPM_PATH"green_wall.bxpm"},
	{11, BXPM_PATH"radar.bxpm"},
	{12, BXPM_PATH"compwall.bxpm"},
	{13, BXPM_PATH"door.bxpm"},
	{14, BXPM_PATH"fiber_wall.bxpm"},
	{15, BXPM_PATH"rock.bxpm"},
	{16, BXPM_PATH"steel_tile1.bxpm"},
	{17, BXPM_PATH"steel_tile2.bxpm"},
	{18, BXPM_PATH"steel_tlight.bxpm"},
	{19, BXPM_PATH"wires.bxpm"},
	{20, BXPM_PATH"clock.bxpm"}
};


typedef struct s_entity_data
{
	char	*name;
	char	*path;
	int		tc[4];
}			t_entity_data;
# define ENTITY_AMOUNT 11
static const t_entity_data	g_entity_data[ENTITY_AMOUNT] =
{
	{
		.name = "Alfred",
		.path = BMP_PATH"alfred.bmp",
		.tc = {0, 0, 43, 47}
	},
	{
		.name = "Spooky",
		.path = BMP_PATH"spooky.bmp",
		.tc = {25, 193, 114, 184}
	},
	{
		.name = "Ghost",
		.path = BMP_PATH"ghost.bmp",
		.tc = {10, 0, 132, 150}
	},
	{
		.name = "Rift",
		.path = BMP_PATH"rift.bmp",
		.tc = {0, 0, 174, 315}
	},
	{
		.name = "Barrel",
		.path = BMP_PATH"objects.bmp",
		.tc = {174, 182, 35, 51}
	},
	{
		.name = "Lamp",
		.path = BMP_PATH"objects.bmp",
		.tc = {174, 7, 23, 84}
	},
	{
		.name = "Torch",
		.path = BMP_PATH"objects.bmp",
		.tc = {371, 209, 10, 47}
	},
	{
		.name = "MeatHook",
		.path = BMP_PATH"objects.bmp",
		.tc = {543, 5, 36, 91}
	},
	{
		.name = "CeilingLamp",
		.path = BMP_PATH"objects.bmp",
		.tc = {88, 70, 24, 56}
	},
	{
		.name = "Gargoyle",
		.path = BMP_PATH"objects.bmp",
		.tc = {229, 97, 58, 75}
	},
	{
		.name = "MedKit",
		.path = BMP_PATH"objects.bmp",
		.tc = {178, 242, 28, 18}
	}
};

#endif
