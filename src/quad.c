#include <quad.h>
#include <stdlib.h>

static struct point2 INVALID_POINT = {-1.0f, -1.0f};

int point2_equals(struct point2 lhs, struct point2 rhs) 
{
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

int aabb_contains(struct point2 p, struct aabb bbox) 
{
  if (p.x < bbox.min.x || p.y < bbox.min.y) return 0;
  if (p.x >= bbox.max.x || p.y >= bbox.max.y) return 0;

  return 1;
}

void quad_create(struct aabb bbox, struct quad_node *root)
{
  if (root != NULL) return;

  struct quad_node *new = malloc(sizeof(struct quad_node));

  if (new == NULL) return;

  new->bbox = bbox;
  new->data = INVALID_POINT;

  new->ne = NULL;
  new->nw = NULL;
  new->se = NULL;
  new->sw = NULL;

  root = new;
}

static int quad_insert_leaf(struct point2 p, struct quad_node *leaf)
{
  if (leaf->nw != NULL || leaf->ne != NULL ||
      leaf->sw != NULL || leaf->se != NULL) 
    return 0;
  
  if (!aabb_contains(p, leaf->bbox)) return 0;
  
  leaf->data = p;

  return 1;
}

static void quad_subdivide(struct quad_node *root)
{
  struct aabb nw_bbox = {root->bbox.min, {root->bbox.max.x / 2, root->bbox.max.y / 2}};
  struct aabb ne_bbox = {{root->bbox.max.x / 2, root->bbox.min.y}, {root->bbox.max.x, root->bbox.max.y / 2}};
  struct aabb sw_bbox = {{root->bbox.min.x, root->bbox.max.y / 2}, {root->bbox.max.x / 2, root->bbox.max.y}};
  struct aabb se_bbox = {{root->bbox.max.x / 2, root->bbox.max.y / 2}, root->bbox.max};

  quad_create(nw_bbox, root->nw);
  quad_create(ne_bbox, root->ne);
  quad_create(sw_bbox, root->sw);
  quad_create(se_bbox, root->se);
}

static void quad_subdivide_and_insert(struct quad_node *root)
{
  quad_subdivide(root);

  if (quad_insert_leaf(root->data, root->nw)) return;
  if (quad_insert_leaf(root->data, root->ne)) return;
  if (quad_insert_leaf(root->data, root->sw)) return;
  if (quad_insert_leaf(root->data, root->se)) return;

  root->data = INVALID_POINT;
}

int quad_insert(struct point2 p, struct quad_node *root)
{
  if (!aabb_contains(p, root->bbox)) return 0;

  if (root->nw == NULL && root->ne == NULL && 
      root->sw == NULL && root->se == NULL && 
      point2_equals(root->data, INVALID_POINT)) {
    root->data = p;
    return 1;
  }

  if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL)
    quad_subdivide_and_insert(root);

  if (quad_insert(p, root->nw)) return 1;
  if (quad_insert(p, root->ne)) return 1;
  if (quad_insert(p, root->sw)) return 1;
  if (quad_insert(p, root->se)) return 1;
}

int quad_is_empty(struct quad_node *root)
{
  if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL &&
      point2_equals(root->data, INVALID_POINT))
    return 1;

  if (!point2_equals(root->data, INVALID_POINT))
    return 0;

  if (root->nw != NULL) {
    return (quad_is_empty(root->nw) || quad_is_empty(root->ne) || 
            quad_is_empty(root->sw) || quad_is_empty(root->se));
  }
}

int quad_remove(struct point2 p, struct quad_node *root)
{

}

int quad_find(struct point2 p, struct quad_node *root)
{

}

void quad_destroy(struct quad_node *root)
{

}
