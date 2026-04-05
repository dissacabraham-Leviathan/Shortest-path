/*
 * ============================================================
 *  CIT Campus Navigator — Floyd-Warshall Shortest Path
 *  Coimbatore Institute of Technology
 *  Compile:  gcc -o cit_nav cit_campus_navigator.c
 *  Run:      ./cit_nav
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 50
#define INF       INT_MAX / 2

/* ── Node definitions ── */
typedef struct {
    int   id;
    char  name[64];
    int   x, y;         /* SVG coordinates (map units) */
} Node;

/* ── Graph ── */
static int  dist[MAX_NODES][MAX_NODES];
static int  nxt [MAX_NODES][MAX_NODES];
static int  N;          /* total nodes */

/* ── Campus nodes ── */
static Node nodes[] = {
    { 0, "Main Gate / Entry",     340,  60  },
    { 1, "Auditorium",            340, 120  },
    { 2, "Main Block",            360, 240  },
    { 3, "Mechanical LAB",        173, 302  },
    { 4, "IT Block",              185, 432  },
    { 5, "Library Block",         297, 430  },
    { 6, "MSC Block",             452, 412  },
    { 7, "CIT Polytechnic",       685, 428  },
    { 8, "Football Ground",       705, 175  },
    { 9, "Badminton Court",       805, 316  },
    {10, "OAT",                   620, 335  },
    {11, "Blue Shed",             305, 514  },
    {12, "Junk Yard",             162, 497  },
    {13, "Canteen",               165, 619  },
    {14, "Basketball Court",      280, 657  },
    {15, "Cricket Ground",        520, 650  },
    {16, "A Mess",                262, 787  },
    {17, "Dispensary",            282, 872  },
    {18, "Women's Hostel",        170, 950  },
    {19, "CIT Store",             195,1039  },
    {20, "Palani Hostel",         302,1022  },
    {21, "B Mess",                430, 985  },
    {22, "Thirumalai Hostel",     580, 970  },
    {23, "Velingiri Hostel",      705, 877  },
    {24, "Kailash Hostel",        812, 835  },
    {25, "Maruthamalai Hostel",   810, 927  },
    /* Road junctions (internal) */
    {26, "Junction-N-mid",        340, 310  },
    {27, "Junction-W-mid",        120, 310  },
    {28, "Junction-E-mid",        750, 310  },
    {29, "Junction-N-center",     530, 310  },
    {30, "Junction-mid-W",        120, 560  },
    {31, "Junction-mid-C",        340, 560  },
    {32, "Junction-mid-E",        750, 560  },
    {33, "Junction-low-W",        120, 780  },
    {34, "Junction-low-C",        340, 780  },
    {35, "Junction-low-E",        530, 780  },
    {36, "Junction-SE",           870, 780  },
    {37, "Junction-bottom-C",     530, 900  },
    {38, "Junction-bottom-W",     220,1060  },
    {39, "Junction-hostel-N",     640, 780  },
    {40, "Junction-hostel-S",     640,1060  },
    {41, "Junction-NE-road",      870, 500  },
    {42, "Junction-NE-top",       870, 160  },
    {43, "Junction-SE-bot",       870, 900  },
};

/* ── Edge list [from, to, weight] ── */
static int edges[][3] = {
    /* Vertical spine */
    {0,1,60},{1,2,120},{2,26,70},{26,31,250},{31,34,220},{34,38,280},
    /* Top horizontal */
    {27,26,220},{26,29,190},{29,28,220},{28,9,80},
    /* Outer perimeter */
    {0,8,200},{8,42,145},{42,41,340},{41,28,190},
    {0,29,190},{42,29,360},
    /* Mid horizontal */
    {30,31,220},{31,32,410},{32,36,10},{36,32,10},
    /* Low horizontal */
    {33,34,220},{34,35,190},{35,39,105},{39,36,230},
    {36,43,120},{43,37,10},{37,35,10},
    /* Secondary vertical left */
    {27,30,250},{30,33,220},{33,18,170},{18,38,110},
    /* Hostel verticals */
    {39,23,97},{39,40,280},{40,22,80},{22,37,70},
    /* Hostel east */
    {36,24,55},{24,25,92},
    /* Canteen spur */
    {30,13,100},
    /* Buildings to road */
    {3,27,53},{3,26,167},
    {4,30,25},{4,27,170},
    {5,26,120},{5,31,130},
    {6,29,100},{6,31,150},{6,32,300},
    {7,29,155},{7,10,155},{7,32,65},
    {10,29,100},{10,28,130},
    {11,31,46},{12,30,63},
    {13,30,59},{14,31,97},
    {15,31,180},{15,32,230},{15,35,130},
    {16,34,7},{17,34,92},
    {19,38,21},{20,38,82},
    {21,38,190},{21,37,85},
    {23,39,97},{23,43,45},
    {8,28,165},{9,28,55},
    {38,40,10},{37,43,10},
};

/* ═══════════════════════════════════════
   Floyd-Warshall initialisation
═══════════════════════════════════════ */
void floyd_warshall_init(void)
{
    N = (int)(sizeof(nodes) / sizeof(nodes[0]));

    /* Initialise distance matrix */
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            dist[i][j] = (i == j) ? 0 : INF;
            nxt [i][j] = (i == j) ? i : -1;
        }

    /* Insert edges (undirected) */
    int ne = (int)(sizeof(edges) / sizeof(edges[0]));
    for (int e = 0; e < ne; e++) {
        int u = edges[e][0], v = edges[e][1], w = edges[e][2];
        if (w < dist[u][v]) { dist[u][v] = w; nxt[u][v] = v; }
        if (w < dist[v][u]) { dist[v][u] = w; nxt[v][u] = u; }
    }
}

/* ═══════════════════════════════════════
   Floyd-Warshall main loop  O(V³)
═══════════════════════════════════════ */
void floyd_warshall_run(void)
{
    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++) {
            if (dist[i][k] == INF) continue;       /* pruning */
            for (int j = 0; j < N; j++) {
                if (dist[k][j] == INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    nxt [i][j] = nxt [i][k];
                }
            }
        }
}

/* ═══════════════════════════════════════
   Reconstruct & print path
═══════════════════════════════════════ */
void print_path(int src, int dst)
{
    if (dist[src][dst] >= INF) {
        printf("  [No path exists]\n");
        return;
    }

    int path[MAX_NODES], len = 0;
    int cur = src;
    while (cur != dst) {
        path[len++] = cur;
        cur = nxt[cur][dst];
        if (len > MAX_NODES) { printf("  [Cycle detected]\n"); return; }
    }
    path[len++] = dst;

    printf("\n  Route (%d stops):\n", len);
    for (int i = 0; i < len; i++) {
        int id = path[i];
        /* skip junction nodes in display */
        if (strncmp(nodes[id].name, "Junction", 8) == 0) {
            printf("      ⤷  [road intersection]\n");
        } else {
            printf("  %s  %s\n", i == 0 ? "▶ START:" : i == len-1 ? "▶ END  :" : "  →     ", nodes[id].name);
        }
    }
    printf("\n  Total distance : %d map-units\n", dist[src][dst]);
}

/* ═══════════════════════════════════════
   List named locations
═══════════════════════════════════════ */
void list_locations(void)
{
    printf("\n  ┌─────────────────────────────────────────────┐\n");
    printf("  │  CIT Campus Locations                       │\n");
    printf("  ├──────┬──────────────────────────────────────┤\n");
    for (int i = 0; i < N; i++) {
        if (strncmp(nodes[i].name, "Junction", 8) == 0) continue;
        printf("  │  %2d  │  %-38s│\n", i, nodes[i].name);
    }
    printf("  └──────┴──────────────────────────────────────┘\n\n");
}

/* ═══════════════════════════════════════
   Interactive menu
═══════════════════════════════════════ */
int main(void)
{
    printf("\n");
    printf("  ╔══════════════════════════════════════════════╗\n");
    printf("  ║   Coimbatore Institute of Technology        ║\n");
    printf("  ║   Campus Navigator — Floyd-Warshall v1.0    ║\n");
    printf("  ╚══════════════════════════════════════════════╝\n\n");

    printf("  Initialising graph ...\n");
    floyd_warshall_init();
    printf("  Running Floyd-Warshall O(V³) on %d nodes ...\n", N);
    floyd_warshall_run();
    printf("  Done. All-pairs shortest paths computed.\n");

    while (1) {
        printf("\n  ════════════════════════════════\n");
        printf("  [1] List all locations\n");
        printf("  [2] Find shortest path\n");
        printf("  [3] Show distance matrix (named)\n");
        printf("  [0] Exit\n");
        printf("  Choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) { printf("\n  Goodbye!\n\n"); break; }

        if (choice == 1) {
            list_locations();

        } else if (choice == 2) {
            list_locations();
            int src, dst;
            printf("  Enter SOURCE id : ");
            if (scanf("%d", &src) != 1 || src < 0 || src >= N) { printf("  Invalid.\n"); continue; }
            printf("  Enter DEST   id : ");
            if (scanf("%d", &dst) != 1 || dst < 0 || dst >= N) { printf("  Invalid.\n"); continue; }

            printf("\n  FROM : %s\n", nodes[src].name);
            printf("  TO   : %s\n",   nodes[dst].name);
            print_path(src, dst);

        } else if (choice == 3) {
            printf("\n  Named-node distance matrix:\n\n");
            /* Header */
            printf("  %20s", "");
            for (int i = 0; i < N; i++)
                if (strncmp(nodes[i].name,"Junction",8)!=0)
                    printf(" %3d", i);
            printf("\n");
            for (int i = 0; i < N; i++) {
                if (strncmp(nodes[i].name,"Junction",8)==0) continue;
                printf("  [%2d] %-16s", i, nodes[i].name);
                for (int j = 0; j < N; j++) {
                    if (strncmp(nodes[j].name,"Junction",8)==0) continue;
                    if (dist[i][j] >= INF) printf("   ∞");
                    else printf(" %3d", dist[i][j] > 999 ? 999 : dist[i][j]);
                }
                printf("\n");
            }
        } else {
            printf("  Unknown option.\n");
        }
    }
    return 0;
}