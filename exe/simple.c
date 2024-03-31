#include <gvc.h>

/* Graphviz documentation simple.c example */

/* $ ./simple file.dot > file.png */
int main(int argc, char **argv)
{
    /* malloc */
    GVC_t *gvc;
    Agraph_t *g;
    FILE *fp;
    /* openning */
    gvc = gvContext();
    if (argc > 1)
        fp = fopen(argv[1], "r");
    else
        fp = stdin;
    /* middle game */
    g = agread(fp, 0);    
    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "png", stdout);
    /* finish (free) */
    gvFreeLayout(gvc, g);
    agclose(g);
    /* done */
    return (gvFreeContext(gvc));
}