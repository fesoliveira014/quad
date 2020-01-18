#include <quad.h>
#include <stdlib.h>

struct point2 INVALID_POINT = {-1.0f, -1.0f};

int point2_equals(struct point2 lhs, struct point2 rhs) 
{
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

static void quad_destroy_children(struct quad_node *root)
{
  if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL)
    return;

  if (root->nw != NULL) {
    quad_destroy_children(root->nw);
    free(root->nw);
    root->nw = NULL;
  }
  
  if (root->ne != NULL) {
    quad_destroy_children(root->ne);
    free(root->ne);
    root->ne = NULL;
  }

  if (root->sw != NULL) {
    quad_destroy_children(root->sw);
    free(root->sw);
    root->sw = NULL;
  }

  if (root->se != NULL) {
    quad_destroy_children(root->se);
    free(root->se);
    root->se = NULL;
  }
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

  quad_create(nw_bbox, &root->nw);
  quad_create(ne_bbox, &root->ne);
  quad_create(sw_bbox, &root->sw);
  quad_create(se_bbox, &root->se);
}

static void quad_subdivide_and_insert(struct quad_node *root)
{
  quad_subdivide(root);

  if (quad_insert_leaf(root->data, root->nw)) goto _end;
  if (quad_insert_leaf(root->data, root->ne)) goto _end;
  if (quad_insert_leaf(root->data, root->sw)) goto _end;
  if (quad_insert_leaf(root->data, root->se)) goto _end;

_end:
  root->data = INVALID_POINT;
}

static int quad_is_empty(struct quad_node *root)
{
  if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL &&
      point2_equals(root->data, INVALID_POINT)) {
    return 1;
  }

  if (!point2_equals(root->data, INVALID_POINT))
    return 0;

  if (root->nw != NULL && root->ne != NULL && 
      root->sw != NULL && root->se != NULL) {
    return (quad_is_empty(root->nw) && quad_is_empty(root->ne) && 
            quad_is_empty(root->sw) && quad_is_empty(root->se));
  }

  return -1; // this should never happen
}

static void quad_merge(struct quad_node *root)
{
  if (quad_is_empty(root)) {
    quad_destroy_children(root);
  }
  else {
    if(root->nw != NULL) {
      quad_merge(root->nw);
    }
    if(root->ne != NULL) {
      quad_merge(root->ne);
    }
    if(root->sw != NULL) {
      quad_merge(root->sw);
    }
    if(root->se != NULL) {
      quad_merge(root->se);
    }
  }
}

int aabb_contains(struct point2 p, struct aabb bbox) 
{
  if (p.x < bbox.min.x || p.y < bbox.min.y) return 0;
  if (p.x >= bbox.max.x || p.y >= bbox.max.y) return 0;

  return 1;
}

void quad_create(struct aabb bbox, struct quad_node **root)
{
  if (*root != NULL) return;

  struct quad_node *new = malloc(sizeof(struct quad_node));

  if (new == NULL) return;

  new->bbox.min = bbox.min;
  new->bbox.max = bbox.max;
  new->data = INVALID_POINT;

  new->ne = NULL;
  new->nw = NULL;
  new->se = NULL;
  new->sw = NULL;

  *root = new;
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

  return 0;
}

int quad_remove(struct point2 p, struct quad_node *root)
{
  if (aabb_contains(p, root->bbox)) {
    if (root->nw == NULL && root->ne == NULL &&
        root->sw == NULL && root->se == NULL &&
        point2_equals(root->data, p)) {
      root->data = INVALID_POINT;
      return 1;
    }
    else {
      if (quad_remove(p, root->nw)) {
        goto merge;
      }
      if (quad_remove(p, root->ne)) {
        goto merge;
      }
      if (quad_remove(p, root->sw)) {
        goto merge;
      }
      if (quad_remove(p, root->se)) {
        goto merge;
      }
    }
  }
  
  return 0;

merge:
  quad_merge(root);
  return 1;
}

int quad_find(struct point2 p, struct quad_node *root)
{
  if (!aabb_contains(p, root->bbox)) return 0;

  if (root->data.x == p.x && root->data.y == p.y) return 1;

  if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL) {
    return 0;
  }

  if (quad_find(p, root->nw)) return 1;
  if (quad_find(p, root->ne)) return 1;
  if (quad_find(p, root->sw)) return 1;
  if (quad_find(p, root->se)) return 1;

  return 0;
}

void quad_destroy(struct quad_node **root)
{
  quad_destroy_children(*root);
  free(*root);
  *root = NULL;
}
