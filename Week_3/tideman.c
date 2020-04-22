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
bool vote(int rank, string name, int ranks[]) // "rank (0 + 1): Alice"; ranks = [1,0,2]
{
    for (int i = 0; i < candidate_count; i++) // i is index of candidate
    {
        if (strcmp(candidates[i], name) == 0) // == 0 means a match!
        {
            ranks[rank] = i;
            return true;
        }
    }
    // TODO
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) // ranks = [2, 0, 1] => Charlie, Alice, Bob
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
            // ranks[0]ranks[1]
            // ranks[0]ranks[2]
            // ranks[1]ranks[2]
        }
    }
    // print the preferences currently.
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i,", preferences[i][j]);
        }
        printf("\n");
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // check if there is a pair. insert pair[paircount], then paircount++;
    // pref[0][0]
    // pref[0][1]
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) // [0][1],[0][2],[1][2]
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

    //print pairs just to diagnose
    for (int i = 0; i < pair_count; i++)
    {
        printf("w: %i, l: %i\n", pairs[i].winner, pairs[i].loser);
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int max_strength = 0;
        int strongest = 0;

        for (int j = i; j < pair_count; j++)
        {
            int strength = preferences[pairs[j].winner][pairs[j].loser];
            if (strength > max_strength)
            {
                strongest = j;
                max_strength = strength;
            }
        }

        int temp_winner = pairs[strongest].winner;
        int temp_loser = pairs[strongest].loser;
        pairs[strongest].winner = pairs[i].winner;
        pairs[strongest].loser = pairs[i].loser;
        pairs[i].winner = temp_winner;
        pairs[i].loser = temp_loser;
    }

    printf("--sorted--\n");
    for (int i = 0; i < pair_count; i++)
    {
        int str = preferences[pairs[i].winner][pairs[i].loser];
        printf("w: %i, l: %i, str:%i\n", pairs[i].winner, pairs[i].loser, str);
    }
}

bool check_path(int from, int to)
{
    if (from == to)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[from][i])
        {
            if (check_path(i, to)) // (e.g. check B -> A) B -> C, B -> D -> A
            {
                return true;
            }
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // before locking [A][B], check if there is ANY path from B to A
        if (!check_path(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // winner needs to point at someone, and not be pointed at
    printf("\nPRINTING WINNER\n");

    for (int i = 0; i < candidate_count; i++)
    {
        bool winner = false;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                winner = true;
            }
        }

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner = false;
            }
        }

        if (winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

