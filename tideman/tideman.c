#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
int is_cyclic(int last_candidate, int first_candidate);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int k = i + 1; k < candidate_count; k++)
        {
            preferences[ranks[i]][ranks[k]] += 1;
        }

    }

}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //I'll come back after final project and try to merge sort this; For now, selection should do the work.
    int temp_highest_index, temp_highest_value;
    pair temp_value;

    for (int i = 0; i < pair_count; i++)
    {
        temp_highest_value = 0;
        temp_highest_index = -1;
        for (int j = i; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > temp_highest_value)
            {
                temp_highest_value = preferences[pairs[j].winner][pairs[j].loser];
                temp_highest_index = j;
            }
        }
        // DEBUG CHECK  printf("Pair %d over %d was position %i and now is %i\n", pairs[i].winner, pairs[i].loser, temp_highest_index, i);
        temp_value = pairs[i];
        pairs[i] = pairs[temp_highest_index];
        pairs[temp_highest_index] = temp_value;
    }

    for (int n = 0; n < pair_count; n++)
    {
        // DEBUG CHECK printf("Pair %d over %d Preference: %i\n", pairs[n].winner, pairs[n].loser, preferences[pairs[n].winner][pairs[n].loser]);
    }
}

int is_cyclic(int chain_end, int chain_start)
{
    // Base Case
    if (chain_end == chain_start)
    {
        // DEBUG CHECK printf("Can't lock: %s -> %s\n", candidates[chain_start], candidates[chain_end]);
        return 1;
    }

    // DEBUG CHECK printf("Start function: %s -> %s\n", candidates[chain_start], candidates[chain_end]);

    // Iterates over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[chain_end][i])
        {
            // Tests chain_end agains every i candidate for locked pairs (those are ordered by strength);
            // If the pair is chain_end -> i is locked, we follow to check if i leads to a cycle (if i wins against chain_start,
            // i.e. the winner of the original pair, leading to a cycle)
            // But if chain_end doesn't win against any i candidate, I can safely lock the pair (because there's no way
            // of creating a cycle as chain_end won't lead to winner again)
            //DEBUG CHECK printf("In locked: %s -> %s\n", candidates[chain_end], candidates[i]);
            if (is_cyclic(i, chain_start) != 0)
            {
                // DEBUG CHECK printf("Can't lock: %s -> %s\n", candidates[chain_start], candidates[chain_end]);
                return 1;
            }
        }
    }
    // DEBUG CHECK printf("Can lock: %s -> %s\n", candidates[chain_start], candidates[chain_end]);
    return 0;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (is_cyclic(pairs[i].loser, pairs[i].winner) == 0)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            // DEBUG CHECK printf("Locked %s -> %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int no_arrow;
    for (int winner = 0; winner < candidate_count; winner++)
    {
        no_arrow = 0;
        for (int k = 0; k < candidate_count; k++)
        {
            if (locked[k][winner] == false)
            {
                no_arrow++;
            }
        }
        if (no_arrow == candidate_count)
        {
            printf("%s\n", candidates[winner]);
        }
    }
    return;
}
