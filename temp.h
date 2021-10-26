#ifndef TEMP_H
# define TEMP_H

#define BMP_PATH "C:/Users/Jony/source/repos/map_editor_v2/temp_resources/"

enum e_player_action
{
	NONE,
	SHOOTING,
	CLICKING,
	SECTOR
};

enum e_event_type
{
	NONE_TYPE,
	CEILING,
	FLOOR,
	STORE,
	HAZARD,
	AUDIO,
	SPAWN,
	LIGHT
};

static const char	*g_event_type[LIGHT] =
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
	ACTION
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
