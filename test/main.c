#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/quad.h"

void quad_print(quad_node_t *root, int depth)
{
	if (root == NULL) {
		for (int i = 0; i < depth; i++) printf("  ");
		printf("null node at depth %d\n", depth);
	}
	else if (root->nw == NULL && root->ne == NULL &&
      root->sw == NULL && root->se == NULL) {
		for (int i = 0; i < depth; i++) printf("  ");
		if (!point2_equals(INVALID_POINT, root->data))
			printf("leaf at depth %d: {%2.02f, %2.02f}\n", depth, root->data.x, root->data.y);
		else
			printf("leaf at depth %d: empty\n", depth);
	}
	else {
		for (int i = 0; i < depth; i++) printf("  ");
		printf("node at depth %d: \n", depth);

		quad_print(root->nw, depth+1);
		quad_print(root->ne, depth+1);
		quad_print(root->sw, depth+1);
		quad_print(root->se, depth+1);
	}
}

int main(int argc, char **argv)
{
	quad_node_t *root = NULL;
	aabb_t bbox = {{0, 0}, {32, 32}};

	printf("box: {{%.02f, %.02f}, {%.02f, %.02f}}\n", bbox.min.x, bbox.min.y, bbox.max.x, bbox.max.y);

	point2_t points[] = {
		{2.5, 3},
		{25, 31.8},
		{1, 2},
		{4, 5},
		{13, 17},
		{1, 31},
	};

	printf("huh\n");

	quad_create(bbox, &root);

	// printf("created: %d\n", root != NULL);

	quad_insert(points[0], root);
	printf("has point %d? %d\n", 0, quad_find(points[0], root));
	quad_insert(points[1], root);
	printf("has point %d? %d\n", 1, quad_find(points[1], root));
	quad_insert(points[2], root);
	printf("has point %d? %d\n", 2, quad_find(points[2], root));
	quad_insert(points[3], root);
	printf("has point %d? %d\n", 3, quad_find(points[3], root));
	quad_insert(points[4], root);
	printf("has point %d? %d\n", 4, quad_find(points[4], root));
	quad_insert(points[5], root);
	printf("has point %d? %d\n", 5, quad_find(points[5], root));

	quad_print(root, 0);

	printf("\n");

	printf("removed 0? %d\n", quad_remove(points[0], root));

	printf("\n");

	quad_print(root, 0);

	printf("\nhas point %d? %d\n", 0, quad_find(points[0], root));

	printf("\n");

	quad_remove(points[2], root);

	quad_print(root, 0);

	printf("\n");
	
	printf("has point %d? %d\n", 2, quad_find(points[2], root));

	printf("\n");

	quad_destroy(&root);

	printf("is tree destroyed? %d\n", root == NULL);

	return 0;
}