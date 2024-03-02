#include <gvc.h>

/* Graphviz documentation simple.c example */
int main(int argc, char **argv)
{
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    gvc = gvContext();
    if (argc > 1)
        fp = fopen(argv[1], "r");
    else
        fp = stdin;
    g = agread(fp, 0);
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", stdout);
    gvFreeLayout(gvc, g);
    agclose(g);
    return (gvFreeContext(gvc));
}