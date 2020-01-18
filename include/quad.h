#ifndef _QUAD_H
#define _QUAD_H

typedef struct point2 {
  float x;
  float y;
} point2_t;

// typedef struct ipoint2 {
//   int x;
//   int y;
// } ipoint2_t;

typedef struct aabb {
  struct point2 min;
  struct point2 max;
} aabb_t;

typedef struct quad_node {
  struct quad_node *nw;
  struct quad_node *ne;
  struct quad_node *sw;
  struct quad_node *se;
  struct aabb bbox;
  struct point2 data;
} quad_node_t;

extern struct point2 INVALID_POINT;

int point2_equals(struct point2 lhs, struct point2 rhs);
int aabb_contains(struct point2 p, struct aabb bbox);

void quad_create(struct aabb bbox, struct quad_node **root);
int quad_insert(struct point2 p, struct quad_node *root);
int quad_remove(struct point2 p, struct quad_node *root);
int quad_find(struct point2 p, struct quad_node *root);
void quad_destroy(struct quad_node **root);

#endif