#ifndef POTION_TABLE
# define POTION_TABLE
# include "splendor_structs.h"

# define POTION_TYPES_COUNT 3
# define TABLE_0_COUNT 4
# define TABLE_1_COUNT 5
# define TABLE_2_COUNT 9
# define TABLE_3_COUNT 4

typedef void (potion_fn)(Context *);

typedef struct {
	uint8_t id;
	potion_fn *fn;
	uint8_t essences[ESSENCE_TYPES];
	uint8_t potion_cost[MAX_COST_TYPES];
	uint8_t cost_count;
	char *name;
}			table_entry;

typedef struct {
	uint8_t count;
	table_entry *entries;
}			table_arr;

static table_entry potions_by_id[TABLE_0_COUNT + TABLE_1_COUNT + TABLE_2_COUNT + TABLE_3_COUNT] = {
	{.id = 0, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={0, 0, 0}, .cost_count = 0, .name ="Potion of Fire"},
	{.id = 1, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={0, 0, 0}, .cost_count = 0, .name ="Potion of Water"},
	{.id = 2, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={0, 0, 0}, .cost_count = 0, .name ="Potion of Earth"},
	{.id = 3, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={0, 0, 0}, .cost_count = 0, .name ="Potion of Air"},

	{.id = 4, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 3, 0}, .cost_count = 2, .name ="Potion of Metal"},
	{.id = 5, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={3, 0, 0}, .cost_count = 2, .name ="Potion of Lightning"},
	{.id = 6, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={0, 1, 0}, .cost_count = 2, .name ="Potion of Vapor"},
	{.id = 7, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 0, 0}, .cost_count = 2, .name ="Potion of Lava"},
	{.id = 8, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={3, 1, 0}, .cost_count = 2, .name ="Potion of Rain"},

	{.id = 9, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={4, 1, 2}, .cost_count = 3, .name ="Potion of Poison"},
	{.id = 10, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={8, 7, 0}, .cost_count = 2, .name ="Potion of Acid"},
	{.id = 11, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={5, 3, 0}, .cost_count = 2, .name ="Potion of Void"},
	{.id = 12, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={7, 4, 0}, .cost_count = 2, .name ="Potion of Plasma"},
	{.id = 13, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={8, 2, 0}, .cost_count = 2, .name ="Potion of Plant"},
	{.id = 14, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={5, 6, 0}, .cost_count = 2, .name ="Potion of Halo"},
	{.id = 15, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={8, 4, 0}, .cost_count = 2, .name ="Potion of Decay"},
	{.id = 16, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={3, 6, 0}, .cost_count = 2, .name ="Potion of Slow"},
	{.id = 17, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={4, 5, 0}, .cost_count = 2, .name ="Potion of Magnetism"},

	{.id = 18, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={11, 10, 9}, .cost_count = 3, .name ="Potion of Death"},
	{.id = 19, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={11, 14, 12}, .cost_count = 3, .name ="Potion of Oblivion"},
	{.id = 20, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={11, 14, 13}, .cost_count = 3, .name ="Potion of Angel"},
	{.id = 21, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={11, 10, 4}, .cost_count = 3, .name ="Potion of Hero"},

};

static table_arr potions_by_tier[4] = {
	{TABLE_0_COUNT, &potions_by_id[0]},
	{TABLE_1_COUNT, &potions_by_id[4]},
	{TABLE_2_COUNT, &potions_by_id[9]},
	{TABLE_3_COUNT, &potions_by_id[18]},
};

#endif
