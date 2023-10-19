#ifndef POTION_TABLE
# define POTION_TABLE
# include "splendor_structs.h"

# define POTION_TYPES_COUNT 3
# define TABLE_0_COUNT 4
# define TABLE_1_COUNT 6
# define TABLE_2_COUNT 3
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
	{.id = 0, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="FIR"},
	{.id = 2, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="WAT"},
	{.id = 3, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="EAR"},
	{.id = 4, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="AIR"},

	{.id = 5, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="PSN"},
	{.id = 6, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="ACD"},
	{.id = 7, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="VOD"},
	{.id = 8, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="PLS"},
	{.id = 9, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="PLT"},
	{.id = 10, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="HAL"},

	{.id = 11, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="DEC"},
	{.id = 12, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="SLO"},
	{.id = 13, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="MAG"},

	{.id = 14, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="DTH"},
	{.id = 15, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="OBL"},
	{.id = 16, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="AGL"},
	{.id = 17, .fn = NULL, .essences = {0,0,0,1}, .potion_cost ={2, 5, 6}, .cost_count = 3, .name ="HRO"},

};

static table_arr potions_by_tier[4] = {
	{TABLE_0_COUNT, &potions_by_id[0]},
	{TABLE_1_COUNT, &potions_by_id[5]},
	{TABLE_2_COUNT, &potions_by_id[11]},
	{TABLE_3_COUNT, &potions_by_id[14]},
};

// static table_entry *_table0[TABLE_0_COUNT] = {
// 	&potions_by_id[0],
// 	&potions_by_id[2],
// 	&potions_by_id[3],
// 	&potions_by_id[4],
// };
// static table_entry *_table1[TABLE_1_COUNT] = {

// };
// static table_entry *_table2[TABLE_2_COUNT] = {

// };
// static table_entry *_table3[TABLE_3_COUNT] = {

// };



#endif
